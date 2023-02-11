//
// Created by JinHai on 2022/9/12.
//

#pragma once

#include "table_ref.h"

namespace infinity {

class BindContext;

class CrossProductTableRef: public TableRef {
public:
    explicit
    CrossProductTableRef(u64 table_index, String alias) :
            TableRef(TableRefType::kCrossProduct, table_index, alias) {}

    SharedPtr<BindContext> left_bind_context_{};
    SharedPtr<BindContext> right_bind_context_{};

    SharedPtr<TableRef> left_table_ref_{};
    SharedPtr<TableRef> right_table_ref_{};
};

}