#pragma once

#include "common/types/alias/containers.h"
#include "common/types/alias/smart_ptr.h"
#include "common/types/alias/strings.h"
#include "common/utility/infinity_assert.h"
#include "common/types/complex/row_id.h"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <roaring/roaring.hh>

namespace infinity {

class TableDef;

class IndexDef;

class DataBlock;

enum class WalCommandType : uint8_t {
    INVALID = 0,
    // -----------------------------
    // Catalog
    // -----------------------------
    CREATE_DATABASE = 1,
    DROP_DATABASE = 2,
    CREATE_TABLE = 3,
    DROP_TABLE = 4,
    ALTER_INFO = 5,
    CREATE_INDEX = 6,

    // -----------------------------
    // Data
    // -----------------------------
    IMPORT = 20,
    APPEND = 21,
    DELETE = 22,
    // -----------------------------
    // Flush
    // -----------------------------
    CHECKPOINT = 99,
};

// WalCommandType -> String
static String WalCommandTypeToString(WalCommandType type) {
    switch (type) {
        case WalCommandType::INVALID:
            return "INVALID";
        case WalCommandType::CREATE_DATABASE:
            return "CREATE_DATABASE";
        case WalCommandType::DROP_DATABASE:
            return "DROP_DATABASE";
        case WalCommandType::CREATE_TABLE:
            return "CREATE_TABLE";
        case WalCommandType::DROP_TABLE:
            return "DROP_TABLE";
        case WalCommandType::ALTER_INFO:
            return "ALTER_INFO";
        case WalCommandType::IMPORT:
            return "IMPORT";
        case WalCommandType::APPEND:
            return "APPEND";
        case WalCommandType::DELETE:
            return "DELETE";
        case WalCommandType::CHECKPOINT:
            return "CHECKPOINT";
        default:
            StorageError("Not supported wal command type");
    }
}

struct WalCmd {
    virtual ~WalCmd() = default;

    virtual WalCommandType GetType() = 0;

    virtual bool operator==(const WalCmd &other) const { return typeid(*this) == typeid(other); }
    bool operator!=(const WalCmd &other) const { return !(*this == other); }
    // Estimated serialized size in bytes
    [[nodiscard]] virtual int32_t GetSizeInBytes() const = 0;
    // Write to a char buffer
    virtual void WriteAdv(char *&ptr) const = 0;
    // Read from a serialized version
    static SharedPtr<WalCmd> ReadAdv(char *&ptr, int32_t max_bytes);
};

struct WalCmdCreateDatabase : public WalCmd {
    explicit WalCmdCreateDatabase(const String &db_name_) : db_name(db_name_) {}

    WalCommandType GetType() override { return WalCommandType::CREATE_DATABASE; }
    bool operator==(const WalCmd &other) const override {
        auto other_cmd = dynamic_cast<const WalCmdCreateDatabase *>(&other);
        return other_cmd != nullptr && db_name == other_cmd->db_name;
    }
    [[nodiscard]] int32_t GetSizeInBytes() const override;
    void WriteAdv(char *&buf) const override;

    String db_name;
};

struct WalCmdDropDatabase : public WalCmd {
    explicit WalCmdDropDatabase(const String &db_name_) : db_name(db_name_) {}

    WalCommandType GetType() override { return WalCommandType::DROP_DATABASE; }
    bool operator==(const WalCmd &other) const override {
        auto other_cmd = dynamic_cast<const WalCmdDropDatabase *>(&other);
        return other_cmd != nullptr && db_name == other_cmd->db_name;
    }
    [[nodiscard]] int32_t GetSizeInBytes() const override;
    void WriteAdv(char *&buf) const override;

    String db_name;
};

struct WalCmdCreateTable : public WalCmd {
    WalCmdCreateTable(const String &db_name_, const SharedPtr<TableDef> &table_def_) : db_name(db_name_), table_def(table_def_) {}

    WalCommandType GetType() override { return WalCommandType::CREATE_TABLE; }
    bool operator==(const WalCmd &other) const override;
    [[nodiscard]] int32_t GetSizeInBytes() const override;
    void WriteAdv(char *&buf) const override;

    String db_name;
    SharedPtr<TableDef> table_def;
};

struct WalCmdCreateIndex : public WalCmd {
    WalCmdCreateIndex(const String &db_name, const String &table_name, SharedPtr<IndexDef> index_def)
        : db_name_(db_name), table_name_(table_name), index_def_(index_def) {}

    virtual WalCommandType GetType() override { return WalCommandType::CREATE_INDEX; }

    virtual bool operator==(const WalCmd &other) const override;

    virtual int32_t GetSizeInBytes() const override;

    virtual void WriteAdv(char *&buf) const override;

    String db_name_{};
    String table_name_{};
    SharedPtr<IndexDef> index_def_{};
};

struct WalCmdDropTable : public WalCmd {
    WalCmdDropTable(const String &db_name_, const String &table_name_) : db_name(db_name_), table_name(table_name_) {}

    WalCommandType GetType() override { return WalCommandType::DROP_TABLE; }
    bool operator==(const WalCmd &other) const override {
        auto other_cmd = dynamic_cast<const WalCmdDropTable *>(&other);
        return other_cmd != nullptr && db_name == other_cmd->db_name && table_name == other_cmd->table_name;
    }
    [[nodiscard]] int32_t GetSizeInBytes() const override;
    void WriteAdv(char *&buf) const override;

    String db_name;
    String table_name;
};

struct WalCmdImport : public WalCmd {
    WalCmdImport(const String &db_name_, const String &table_name_, const String &segment_dir_)
        : db_name(db_name_), table_name(table_name_), segment_dir(segment_dir_) {}

    WalCommandType GetType() override { return WalCommandType::IMPORT; }
    bool operator==(const WalCmd &other) const override;
    [[nodiscard]] int32_t GetSizeInBytes() const override;
    void WriteAdv(char *&buf) const override;

    String db_name;
    String table_name;
    String segment_dir;
};

struct WalCmdAppend : public WalCmd {
    WalCmdAppend(const String &db_name_, const String &table_name_, const SharedPtr<DataBlock> &block_)
        : db_name(db_name_), table_name(table_name_), block(block_) {}

    WalCommandType GetType() override { return WalCommandType::APPEND; }
    bool operator==(const WalCmd &other) const override;
    [[nodiscard]] int32_t GetSizeInBytes() const override;
    void WriteAdv(char *&buf) const override;

    String db_name;
    String table_name;
    SharedPtr<DataBlock> block;
};

struct WalCmdDelete : public WalCmd {
    WalCmdDelete(const String &db_name_, const String &table_name_, const Vector<RowID> &row_ids_)
        : db_name(db_name_), table_name(table_name_), row_ids(row_ids_) {}

    WalCommandType GetType() override { return WalCommandType::DELETE; }
    bool operator==(const WalCmd &other) const override;
    [[nodiscard]] int32_t GetSizeInBytes() const override;
    void WriteAdv(char *&buf) const override;

    String db_name;
    String table_name;
    Vector<RowID> row_ids;
};

struct WalCmdCheckpoint : public WalCmd {
    WalCmdCheckpoint(int64_t max_commit_ts_, String catalog_path) : max_commit_ts_(max_commit_ts_), catalog_path_(catalog_path) {}

    WalCommandType GetType() override { return WalCommandType::CHECKPOINT; }
    bool operator==(const WalCmd &other) const override;
    [[nodiscard]] int32_t GetSizeInBytes() const override;
    void WriteAdv(char *&buf) const override;

    int64_t max_commit_ts_;
    String catalog_path_;
};

struct WalEntryHeader {
    i32 size; // size of payload, including the header, round to multi
    // of 4. There's 4 bytes pad just after the payload storing
    // the same value to assist backward iterating.
    u32 checksum; // crc32 of the entry, including the header and the
    // payload. User shall populate it before writing to wal.
    i64 txn_id;    // txn id of the entry
    i64 commit_ts; // commit timestamp of the txn
};

struct WalEntry : WalEntryHeader {
    bool operator==(const WalEntry &other) const;

    bool operator!=(const WalEntry &other) const;

    // Estimated serialized size in bytes, ensured be no less than Write
    // requires, allowed be larger.
    [[nodiscard]] int32_t GetSizeInBytes() const;

    // Write to a char buffer
    void WriteAdv(char *&ptr) const;
    // Read from a serialized version
    static SharedPtr<WalEntry> ReadAdv(char *&ptr, int32_t max_bytes);

    Vector<SharedPtr<WalCmd>> cmds;

    [[nodiscard]] Pair<i64, String> GetCheckpointInfo() const;

    bool ISCheckPoint() const;
};

class WalEntryIterator {
public:
    explicit WalEntryIterator(String wal) : wal_(std::move(wal)), entry_index_(0), entries_() {}

    void Init();

    bool Next();

    [[nodiscard]] SharedPtr<WalEntry> GetEntry();

private:
    String wal_{};
    std::streamsize wal_size_{};
    char *ptr_{};
    SizeT entry_index_{};
    Vector<SharedPtr<WalEntry>> entries_{};
};

} // namespace infinity
