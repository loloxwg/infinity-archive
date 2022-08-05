//
// Created by JinHai on 2022/7/28.
//

#pragma once

#include <cstdint>

enum class PhysicalOperatorType: uint8_t {
    kInvalid = 0,
    kAggregate,
    kUnionAll,
    kTableScan,
    kFilter,
    kIndexScan,

    // Join
    kJoinHash,
    kJoinNestedLoop,
    kJoinSortMerge,
    kJoinIndex,
    kProduct,

    // Result modifier
    kLimit,
    kTop,
    kProjection,
    kSort,

    // DML
    kDelete,
    kUpdate,
    kInsert,
    kImport,
    kExport,

    // DDL
    kAlter,
    kCreateTable,
    kCreateView,
    kDropTable,
    kDropView,

    // misc
    kExplain,
    kPreparedPlan,
};