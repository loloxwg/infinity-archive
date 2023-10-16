//
// Created by jinhai on 22-12-25.
//

#include "parser/definition/table_def.h"
#include "base_test.h"
#include "main/infinity.h"

class TableDefTest : public BaseTest {};

TEST_F(TableDefTest, test1) {
    using namespace infinity;

    Vector<SharedPtr<ColumnDef>> columns;

    i64 column_id = 0;
    {
        HashSet<ConstraintType> constraints;
        constraints.insert(ConstraintType::kUnique);
        constraints.insert(ConstraintType::kNotNull);
        auto column_def_ptr = MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "c1", constraints);
        columns.emplace_back(column_def_ptr);
    }
    {
        HashSet<ConstraintType> constraints;
        constraints.insert(ConstraintType::kPrimaryKey);
        auto column_def_ptr = MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kVarchar)), "c2", constraints);
        columns.emplace_back(column_def_ptr);
    }

    TableDef table_def(MakeShared<String>("default"), MakeShared<String>("t1"), columns);

    EXPECT_EQ(*table_def.table_name(), "t1");
    EXPECT_EQ(table_def.column_count(), 2);
    EXPECT_EQ(table_def.GetColIdByName("c1"), 0);
    EXPECT_EQ(table_def.GetColIdByName("c2"), 1);
    std::cout << table_def.ToString() << std::endl;
}

TEST_F(TableDefTest, ReadWrite) {
    using namespace infinity;

    Vector<SharedPtr<ColumnDef>> columns;

    i64 column_id = 0;
    {
        HashSet<ConstraintType> constraints;
        constraints.insert(ConstraintType::kUnique);
        constraints.insert(ConstraintType::kNotNull);
        auto column_def_ptr = MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(DataType(LogicalType::kTinyInt)), "c1", constraints);
        columns.emplace_back(column_def_ptr);
    }
    {
        HashSet<ConstraintType> constraints;
        constraints.insert(ConstraintType::kPrimaryKey);
        auto column_def_ptr =
            MakeShared<ColumnDef>(column_id++, MakeShared<DataType>(LogicalType::kVarchar, VarcharInfo::Make(8196)), "c2", constraints);
        columns.emplace_back(column_def_ptr);
    }

    TableDef table_def(MakeShared<String>("default"), MakeShared<String>("t1"), columns);
    std::cout << "table_def: " << table_def.ToString() << std::endl;

    int32_t exp_size = table_def.GetSizeInBytes();
    std::vector<char> buf(exp_size, char(0));
    char *buf_beg = buf.data();
    char *ptr = buf_beg;
    table_def.WriteAdv(ptr);
    EXPECT_EQ(ptr - buf_beg, exp_size);

    ptr = buf_beg;
    int32_t maxbytes = exp_size;
    SharedPtr<TableDef> table_def2 = table_def.ReadAdv(ptr, maxbytes);
    std::cout << "table_def2: " << table_def2->ToString() << std::endl;
    EXPECT_EQ(ptr - buf_beg, exp_size);
    EXPECT_NE(table_def2, nullptr);
    EXPECT_EQ(*table_def2, table_def);
}
