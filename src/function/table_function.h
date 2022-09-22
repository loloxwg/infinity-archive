//
// Created by JinHai on 2022/9/14.
//

#pragma once
#include "function.h"
#include "storage/data_type.h"
#include <vector>

namespace infinity {
class TableFunction : public Function {
public:
    explicit TableFunction(std::string name,
                           std::vector<LogicalType> argument_types);

private:
    std::vector<LogicalType> argument_types_;
};
}
