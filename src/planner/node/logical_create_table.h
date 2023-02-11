//
// Created by JinHai on 2022/7/23.
//

#pragma once

#include <utility>

#include "planner/logical_node.h"
#include "storage/table_definition.h"

namespace infinity {

class LogicalCreateTable : public LogicalNode {
public:
    static inline SharedPtr<LogicalCreateTable>
    Make(const SharedPtr<String>& schema_name,
         const SharedPtr<TableDef>& table_def_ptr,
         u64 table_index) {
        return MakeShared<LogicalCreateTable>(schema_name, table_def_ptr, table_index);
    }

public:
    LogicalCreateTable(SharedPtr<String> schema_name,
                       SharedPtr<TableDef> table_def_ptr,
                       u64 table_index)
        : LogicalNode(LogicalNodeType::kCreateTable),
          schema_name_(std::move(schema_name)),
          table_definitions_(std::move(table_def_ptr)),
          table_index_(table_index)
          {}

    String 
    ToString(i64& space) final;

    [[nodiscard]] SharedPtr<TableDef>
    table_definitions() const {
        return table_definitions_;
    }

    [[nodiscard]] SharedPtr<String>
    schema_name() const {
        return schema_name_;
    }

    [[nodiscard]] inline u64
    table_index() const {
        return table_index_;
    }

private:
    SharedPtr<String> schema_name_{};
    SharedPtr<TableDef> table_definitions_{};
    u64 table_index_{};
};

}

