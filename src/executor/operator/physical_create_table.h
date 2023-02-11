//
// Created by JinHai on 2022/7/28.
//

#pragma once

#include "storage/table_def.h"
#include "executor/physical_operator.h"

#include <memory>

namespace infinity {

class PhysicalCreateTable : public PhysicalOperator {
public:
    explicit
    PhysicalCreateTable(SharedPtr<String> schema_name,
                        SharedPtr<TableDef> table_def_ptr,
                        u64 table_index,
                        uint64_t id);

    explicit
    PhysicalCreateTable(SharedPtr<String> schema_name,
                        const SharedPtr<PhysicalOperator>& input,
                        u64 table_index,
                        uint64_t id);

    ~PhysicalCreateTable() override = default;

    void
    Init() override;

    void
    Execute(SharedPtr<QueryContext>& query_context) override;

    inline u64
    table_index() const {
        return table_index_;
    }

private:
    SharedPtr<TableDef> table_def_ptr_{};
    SharedPtr<String> schema_name_{};
    u64 table_index_{};

};

}
