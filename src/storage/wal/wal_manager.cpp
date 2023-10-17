//
// Created by jinhai on 23-5-16.
//

#include "wal_manager.h"
#include "common/utility/exception.h"
#include "main/logger.h"
#include "storage/storage.h"
#include <exception>
#include <filesystem>
#include <iterator>
#include <vector>

namespace infinity {

using namespace std;
namespace fs = std::filesystem;

WalManager::WalManager(Storage *storage,
                       const string &wal_path,
                       u64 wal_size_threshold,
                       u64 full_checkpoint_interval_sec,
                       u64 delta_checkpoint_interval_sec,
                       u64 delta_checkpoint_interval_wal_bytes)
    : storage_(storage), wal_path_(wal_path), wal_size_threshold_(wal_size_threshold), full_checkpoint_interval_sec_(full_checkpoint_interval_sec),
      delta_checkpoint_interval_sec_(delta_checkpoint_interval_sec), delta_checkpoint_interval_wal_bytes_(delta_checkpoint_interval_wal_bytes),
      running_(false) {}

WalManager::~WalManager() {
    Stop();
    que_.clear();
    que2_.clear();
}

void WalManager::Start() {
    bool expected = false;
    bool changed = running_.compare_exchange_strong(expected, true);
    if (!changed)
        return;
    fs::path wal_dir = fs::path(wal_path_).parent_path();
    if (!fs::exists(wal_dir)) {
        fs::create_directory(wal_dir);
    }
    // TODO: recovery from wal checkpoint
    ofs_ = std::ofstream(wal_path_, std::ios::app | std::ios::binary);
    if (!ofs_.is_open()) {
        throw Exception("Failed to open wal file: " + wal_path_);
    }
    auto seconds_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch());
    int64_t now = seconds_since_epoch.count();
    full_ckp_when_ = now;
    delta_ckp_when_ = now;
    max_commit_ts_ = 0;
    wal_size_ = 0;
    flush_thread_ = std::thread([this] { Flush(); });
    checkpoint_thread_ = std::thread([this] { Checkpoint(); });
}

void WalManager::Stop() {
    bool expected = true;
    bool changed = running_.compare_exchange_strong(expected, false);
    if (!changed) {
        LOG_INFO("WalManager::Stop already stopped");
        return;
    }

    LOG_INFO("WalManager::Stop begin to stop txn manager");
    // Notify txn manager to stop.
    TxnManager *txn_mgr = storage_->txn_manager();
    txn_mgr->Stop();

    // pop all the entries in the queue. and notify the condition variable.
    std::lock_guard guard(mutex_);
    for (const auto & entry : que_) {
        auto wal_entry = que_.front();
        Txn *txn = txn_mgr->GetTxn(wal_entry->txn_id);
        if (txn != nullptr) {
            txn->CancelCommitTxnBottom();
        }
    }
    que_.clear();

    // Wait for checkpoint thread to stop.
    LOG_INFO("WalManager::Stop checkpoint thread join");
    checkpoint_thread_.join();

    // Wait for flush thread to stop
    LOG_INFO("WalManager::Stop flush thread join");
    flush_thread_.join();

    ofs_.close();
}

// Session request to persist an entry. Assuming txn_id of the entry has
// been initialized.
int WalManager::PutEntry(std::shared_ptr<WalEntry> entry) {
    if (!running_.load()) {
        return -1;
    }
    int rc = 0;
    mutex_.lock();
    if (running_.load()) {
        que_.push_back(entry);
        rc = -1;
    }
    mutex_.unlock();
    return rc;
}

void WalManager::SetWalState(TxnTimeStamp max_commit_ts, int64_t wal_size){
    mutex2_.lock();
    this->max_commit_ts_ = max_commit_ts;
    this->wal_size_ = wal_size;
    mutex2_.unlock();
}
void WalManager::GetWalState(TxnTimeStamp& max_commit_ts, int64_t& wal_size){
    mutex2_.lock();
    max_commit_ts = this->max_commit_ts_;
    wal_size = this->wal_size_;
    mutex2_.unlock();
}

// Flush is scheduled regularly. It collects a batch of transactions, sync
// wal and do parallel committing. Each sync cost ~1s. Each checkpoint cost
// ~10s. So it's necessary to sync for a batch of transactions, and to
// checkpoint for a batch of sync.
void WalManager::Flush() {
    LOG_TRACE("WalManager::Flush mainloop begin");
    while (running_.load()) {
        TxnTimeStamp max_commit_ts = 0;
        int64_t wal_size = 0;
        mutex_.lock();
        que_.swap(que2_);
        mutex_.unlock();
        if (que2_.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        GetWalState(max_commit_ts, wal_size);
        for (const auto &entry : que2_) {
            // Empty WalEntry (read-only transactions) shouldn't go into WalManager.
            StorageAssert(!entry->cmds.empty(), fmt::format("WalEntry of txn_id {} commands is empty", entry->txn_id));
            int32_t exp_size = entry->GetSizeInBytes();
            vector<char> buf(exp_size);
            char *ptr = buf.data();
            entry->WriteAdv(ptr);
            int32_t act_size = ptr - buf.data();
            if (exp_size != act_size)
                LOG_ERROR("WalManager::Flush WalEntry estimated size {} differ "
                          "with the actual one {}",
                          exp_size,
                          act_size);
            ofs_.write(buf.data(), ptr - buf.data());
            LOG_TRACE("WalManager::Flush done writing wal for txn_id {}, commit_ts {}", entry->txn_id, entry->commit_ts);
            if (entry->cmds[0]->GetType() != WalCommandType::CHECKPOINT){
                max_commit_ts = entry->commit_ts;
                wal_size += act_size;
            }
        }
        ofs_.flush();
        TxnManager *txn_mgr = storage_->txn_manager();
        for (const auto &entry : que2_) {
            // Commit sequentially so they get visible in the same order with wal.
            Txn *txn = txn_mgr->GetTxn(entry->txn_id);
            if (txn != nullptr) {
                txn->CommitTxnBottom();
            }
        }
        que2_.clear();

        // Check if the wal file is too large.
        try {
            auto file_size = fs::file_size(wal_path_);
            if (file_size > wal_size_threshold_) {
                this->SwapWalFile(max_commit_ts);
            }
        } catch (std::exception &e) {
            LOG_WARN(e.what());
        } catch (...) {
            LOG_WARN("WalManager::Flush threads get exception");
            return;
        }
        SetWalState(max_commit_ts, wal_size);
    }
    LOG_TRACE("WalManager::Flush mainloop end");
}

/*****************************************************************************
 * CHECKPOINT WAL FILE
 *****************************************************************************/

// Do checkpoint for transactions which's lsn no larger than the given one.
void WalManager::Checkpoint() {
    LOG_TRACE("WalManager::Checkpoint mainloop begin");
    while (running_.load()) {
        auto seconds_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch());
        int64_t now = seconds_since_epoch.count();
        TxnTimeStamp max_commit_ts;
        int64_t wal_size;
        GetWalState(max_commit_ts, wal_size);
        bool is_full_checkpoint = false;
        bool is_delta_checkpoint = false;
        if (now - full_ckp_when_ > full_checkpoint_interval_sec_ && wal_size != full_ckp_wal_size_) {
            is_full_checkpoint = true;
        } else if ((now - delta_ckp_when_ > delta_checkpoint_interval_sec_ && wal_size != delta_ckp_wal_size_) || wal_size - delta_ckp_wal_size_ > delta_checkpoint_interval_wal_bytes_) {
            is_delta_checkpoint = true;
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        TxnManager *txn_mgr = nullptr;
        Txn *txn = nullptr;
        try {
            txn_mgr = storage_->txn_manager();
            txn = txn_mgr->CreateTxn();
            txn->BeginTxn();
            LOG_INFO(fmt::format("created transaction for checkpoint, txn_id: {}, begin_ts: {}, max_commit_ts {}", txn->TxnID(), txn->BeginTS(), max_commit_ts));
            txn->Checkpoint(max_commit_ts, is_full_checkpoint);
            txn->CommitTxn();

            ckp_commit_ts_ = max_commit_ts;
            delta_ckp_when_ = now;
            delta_ckp_wal_size_ = wal_size;
            if (is_full_checkpoint) {
                full_ckp_when_ = now;
                full_ckp_wal_size_ = wal_size;
            }
            LOG_INFO("WalManager::Checkpoint {} done for commit_ts <= {}", is_full_checkpoint?"full":"delta", max_commit_ts);
            RecycleWalFile();
        } catch (std::exception &e) {
            LOG_ERROR(fmt::format("WalManager::Checkpoint failed: {}", e.what()));
        }
    }
    LOG_TRACE("WalManager::Checkpoint mainloop end");
}

/**
 * @brief Swap the wal file to a new one.
 * We will swap a new wal file when the current wal file is too large.
 * Just rename the current wal file to a new one, and create a new wal file with
 * the original name. So we only focus on the current wal file: wal.log When
 * replaying the wal file, we will just start with the wal.log file.
 * @param max_commit_ts The max commit timestamp of the transactions in the
 * current wal file.
 */
void WalManager::SwapWalFile(const TxnTimeStamp max_commit_ts) {
    if (ofs_.is_open()) {
        ofs_.close();
    }

    fs::path old_file_path = fs::path(wal_path_);

    String new_file_path = old_file_path.string() + '.' + std::to_string(max_commit_ts);
    LOG_INFO("WAL NEW PATH: {}", new_file_path.c_str());

    // Rename the current wal file to a new one.
    fs::rename(wal_path_, new_file_path);

    // Create a new wal file with the original name.
    ofs_ = std::ofstream(wal_path_, std::ios::app | std::ios::binary);
    if (!ofs_.is_open()) {
        throw StorageException("Failed to open wal file: " + wal_path_);
    }
}

/*****************************************************************************
 * REPLAY WAL FILE
 *****************************************************************************/
/**
 * @brief Replay the wal file.
 *  wal format: wal.log.<max_commit_ts>
 *  the wal.log file is the current wal file.
 *  the most recent wal.log file is the current wal file.
 *
 *  time min -------------------------------------------------------------------------------------------------------------------------------> time max
 *  ================ =========================  =================================================================================================
 *  |   wal.log.1  | |       wal.log.3       |  |                                wal.log                                                        |
 *  ---------------- -------------------------  --------------------------------------------------------------------------------------------------
 *  |[entry｜entry]| |  [entry｜entry｜entry] ｜ | [entry｜entry｜entry| <checkpoint{ max_commit_ts ; catalog_path }>｜entry?(bad checksum)｜entry]|
 *  ================ =========================  ==================================================================================================
 *                                                  ⬆️     ｜                                          ⬆️-------- ⬅️ ---------------------- ⬅️ phase 1:
 find the checkpoint entry
 *                                                  ｜      ｜                      ⬇️
 *                                                  ｜------｜------- ⬅️ -----------｜     phase 2: find the first entry which commit_ts low equal (<=)
 the max_commit_ts
 *                                                          ｜
 *                                                          ➡️ ------ |       ...  (jump checkpoint entry) ...       | continue => end
 *
 *                                                                    phase 3: replay the entries by the order of the wal.log
 *                                                                    - case 1: the entry is a checkpoint entry, jump it.
 *                                                                    - case 2: the entry is a normal entry, replay it.
 *                                                                    - case 3: the entry is a bad entry, stop replaying and end.



 *  phase 1(⬅️): - find the checkpoint entry in the wal.log file. ( Attention: From back to front find the first checkpoint entry)
 *           - get the max commit timestamp of the checkpoint entry.
 *           - get the catalog path of the checkpoint entry.
 *
 *  phase 2(⬅️): - find the first entry which commit_ts low equal (<=) the max_commit_ts
 *  phase 3(➡️): - replay the entries by the order of the wal.log
 * @return int64_t The max commit timestamp of the transactions in the wal file.
 *
 */
int64_t WalManager::ReplayWalFile() {
    // if the wal directory does not exist, just return 0.
    if (!fs::exists(wal_path_)) {
        return 0;
    }
    // if the wal file is empty, just return 0.
    if (fs::file_size(wal_path_) == 0) {
        return 0;
    }
    // if the wal file is not empty, we will replay the wal file.
    // 1. First , we will get all wal files store to wal_list_.
    // wal_list_ is a vector of string, each string is a wal file path.
    // wal_list_ is sorted by the suffix of the wal file path.
    // e.g. wal_list_ = {wal.log.1, wal.log.2, wal.log.3}
    LOG_INFO("Wal path: {}", wal_path_.c_str());
    for (const auto &entry : fs::directory_iterator(fs::path(wal_path_).parent_path())) {
        if (entry.is_regular_file()) {
            wal_list_.push_back(entry.path().string());
        }
    }
    std::sort(wal_list_.begin(), wal_list_.end(), [](const String &a, const String &b) {
        return a.substr(a.find_last_of('.') + 1) > b.substr(b.find_last_of('.') + 1);
    });

    // log the wal files.
    for (const auto &wal_file : wal_list_) {
        LOG_INFO("List wal file: {}", wal_file.c_str());
    }
    LOG_INFO("List wal file size: {}", wal_list_.size());
    i64 max_commit_ts = 0;
    String catalog_path;
    Vector<SharedPtr<WalEntry>> replay_entries;
    for (const auto &wal_file : wal_list_) {

        WalEntryIterator iterator(wal_file);
        iterator.Init();

        // phase 1: find the max commit ts and catalog path
        LOG_INFO("Phase 1: find the max commit ts and catalog path")
        while (iterator.Next()) {
            auto wal_entry = iterator.GetEntry();
            replay_entries.push_back(wal_entry);
            if (wal_entry->ISCheckPoint()) {
                std::tie(max_commit_ts, catalog_path) = wal_entry->GetCheckpointInfo();
                LOG_INFO("Checkpoint max commit ts: {}", max_commit_ts);
                LOG_INFO("Catalog Path: {}", catalog_path);
                break;
            }
        }

        // phase 2: by the max commit ts, find the entries to replay
        LOG_INFO("Phase 2: by the max commit ts, find the entries to replay")
        while (iterator.Next()) {
            auto wal_entry = iterator.GetEntry();
            if (wal_entry->commit_ts > max_commit_ts && !wal_entry->ISCheckPoint()) {
                replay_entries.push_back(wal_entry);
            }
        }
    }

    // phase 3: replay the entries
    LOG_INFO("Phase 3: replay the entries")
    std::reverse(replay_entries.begin(), replay_entries.end());
    i64 last_commit_ts = 0;
    for (const auto &entry : replay_entries) {
        if (entry->commit_ts > last_commit_ts) {
            last_commit_ts = entry->commit_ts;
        }
        if (entry->ISCheckPoint()) {
            continue;
        }
        LOG_INFO("Wal entry commit ts: {}", entry->commit_ts);
        for (const auto &cmd : entry->cmds) {
            LOG_INFO("  Wal cmd: {}", WalCommandTypeToString(cmd->GetType()).c_str());
            cmd->Replay(storage_, entry->txn_id, entry->commit_ts);
        }
    }

    return last_commit_ts;
}
/*****************************************************************************
 * GC WAL FILE
 *****************************************************************************/

/**
 * @brief Gc the old wal files.
 * Only delete the wal.log.* files. the wal.log file is current wal file.
 * Check if the wal.log.* files are too old.
 * if * is little than the max_commit_ts, we will delete it.
 */
void WalManager::RecycleWalFile() {
    // Gc old wal files.
    LOG_INFO("WalManager::Checkpoint begin to gc wal files")
    if (fs::exists(wal_path_)) {
        for (const auto &entry : fs::directory_iterator(fs::path(wal_path_).parent_path())) {
            if (entry.is_regular_file() && entry.path().string().find("wal.log.") != std::string::npos) {
                auto suffix = entry.path().string().substr(entry.path().string().find_last_of('.') + 1);
                if (std::stoll(suffix) < ckp_commit_ts_) {
                    fs::remove(entry.path());
                    LOG_TRACE("WalManager::Checkpoint delete wal file: {}", entry.path().string().c_str());
                }
            }
        }
    }
    LOG_INFO("WalManager::Checkpoint end to gc wal files");
}

} // namespace infinity