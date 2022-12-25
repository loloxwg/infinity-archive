//
// Created by JinHai on 2022/12/11.
//

#include <gtest/gtest.h>
#include "base_test.h"
#include "common/column_vector/column_vector.h"
#include "common/types/value.h"
#include "main/logger.h"
#include "main/stats/global_resource_usage.h"
#include "common/types/info/varchar_info.h"

class ColumnVectorMixedTest : public BaseTest {
    void
    SetUp() override {
        infinity::Logger::Initialize();
        infinity::GlobalResourceUsage::Init();
    }

    void
    TearDown() override {
        infinity::Logger::Shutdown();
        EXPECT_EQ(infinity::GlobalResourceUsage::GetObjectCount(), 0);
        EXPECT_EQ(infinity::GlobalResourceUsage::GetRawMemoryCount(), 0);
        infinity::GlobalResourceUsage::UnInit();
    }
};

TEST_F(ColumnVectorMixedTest, mixed_integer_a) {
    using namespace infinity;

    DataType data_type(LogicalType::kMixed);
    ColumnVector col_mixed(data_type, ColumnVectorType::kFlat);
    col_mixed.Initialize();

    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    auto tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_integer1 = MixedType::MakeInteger(i);
        Value v = Value::MakeMixedData(mixed_integer1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_integer1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kInteger);
        auto* integer_mixed_ptr = (IntegerMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(integer_mixed_ptr->value, i);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reserve(DEFAULT_VECTOR_SIZE* 2);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kInteger);
        auto* integer_mixed_ptr = (IntegerMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(integer_mixed_ptr->value, i);
    }

    EXPECT_EQ(col_mixed.tail_index_, DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.capacity(), 2* DEFAULT_VECTOR_SIZE);

    for(i64 i = DEFAULT_VECTOR_SIZE; i < 2 * DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_integer1 = MixedType::MakeInteger(i);
        Value v = Value::MakeMixedData(mixed_integer1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_integer1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kInteger);
        auto* integer_mixed_ptr = (IntegerMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(integer_mixed_ptr->value, i);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reset();
    EXPECT_EQ(col_mixed.capacity(), 0);
    EXPECT_EQ(col_mixed.tail_index_, 0);
//    EXPECT_EQ(col_mixed.data_type_size_, 0);
    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.initialized, false);

    // ====
    col_mixed.Initialize();
    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);
    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_integer1 = MixedType::MakeInteger(i);
        Value v = Value::MakeMixedData(mixed_integer1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_integer1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kInteger);
        auto* integer_mixed_ptr = (IntegerMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(integer_mixed_ptr->value, i);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }
}

TEST_F(ColumnVectorMixedTest, mixed_float_a) {
    using namespace infinity;

    DataType data_type(LogicalType::kMixed);
    ColumnVector col_mixed(data_type, ColumnVectorType::kFlat);
    col_mixed.Initialize();

    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    auto tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_float1 = MixedType::MakeFloat(static_cast<f64>(i));
        Value v = Value::MakeMixedData(mixed_float1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_float1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kFloat);
        auto* float_mixed_ptr = (FloatMixedType*)(&vx.value_.mixed_value);
        EXPECT_FLOAT_EQ(float_mixed_ptr->value, static_cast<f64>(i));
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reserve(DEFAULT_VECTOR_SIZE* 2);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kFloat);
        auto* float_mixed_ptr = (FloatMixedType*)(&vx.value_.mixed_value);
        EXPECT_FLOAT_EQ(float_mixed_ptr->value, static_cast<f64>(i));
    }

    EXPECT_EQ(col_mixed.tail_index_, DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.capacity(), 2* DEFAULT_VECTOR_SIZE);
    for(i64 i = DEFAULT_VECTOR_SIZE; i < 2 * DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_float1 = MixedType::MakeFloat(static_cast<f64>(i));
        Value v = Value::MakeMixedData(mixed_float1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_float1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kFloat);
        auto* float_mixed_ptr = (FloatMixedType*)(&vx.value_.mixed_value);
        EXPECT_FLOAT_EQ(float_mixed_ptr->value, static_cast<f64>(i));
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reset();
    EXPECT_EQ(col_mixed.capacity(), 0);
    EXPECT_EQ(col_mixed.tail_index_, 0);
//    EXPECT_EQ(col_mixed.data_type_size_, 0);
    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.initialized, false);

    // ====
    col_mixed.Initialize();
    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);
    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_float1 = MixedType::MakeFloat(static_cast<f64>(i));
        Value v = Value::MakeMixedData(mixed_float1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_float1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kFloat);
        auto* float_mixed_ptr = (FloatMixedType*)(&vx.value_.mixed_value);
        EXPECT_FLOAT_EQ(float_mixed_ptr->value, static_cast<f64>(i));
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }
}

TEST_F(ColumnVectorMixedTest, mixed_short_str_a) {
    using namespace infinity;

    DataType data_type(LogicalType::kMixed);
    ColumnVector col_mixed(data_type, ColumnVectorType::kFlat);
    col_mixed.Initialize();

    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    auto tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        String str = std::to_string(i);
        MixedT mixed_str1 = MixedType::MakeString(str);
        Value v = Value::MakeMixedData(mixed_str1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_str1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kShortStr);
        auto* short_str_mixed_ptr = (ShortStrMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(short_str_mixed_ptr->length, str.size());
        EXPECT_STREQ(short_str_mixed_ptr->ptr, str.c_str());
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reserve(DEFAULT_VECTOR_SIZE* 2);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        String str = std::to_string(i);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kShortStr);
        auto* short_str_mixed_ptr = (ShortStrMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(short_str_mixed_ptr->length, str.size());
        EXPECT_STREQ(short_str_mixed_ptr->ptr, str.c_str());
    }

    EXPECT_EQ(col_mixed.tail_index_, DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.capacity(), 2* DEFAULT_VECTOR_SIZE);
    for(i64 i = DEFAULT_VECTOR_SIZE; i < 2 * DEFAULT_VECTOR_SIZE; ++ i) {
        String str = std::to_string(i);
        MixedT mixed_str1 = MixedType::MakeString(str);
        Value v = Value::MakeMixedData(mixed_str1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_str1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kShortStr);
        auto* short_str_mixed_ptr = (ShortStrMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(short_str_mixed_ptr->length, str.size());
        EXPECT_STREQ(short_str_mixed_ptr->ptr, str.c_str());
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reset();
    EXPECT_EQ(col_mixed.capacity(), 0);
    EXPECT_EQ(col_mixed.tail_index_, 0);
//    EXPECT_EQ(col_mixed.data_type_size_, 0);
    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.initialized, false);

    // ====
    col_mixed.Initialize();
    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);
    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        String str = std::to_string(i);
        MixedT mixed_str1 = MixedType::MakeString(str);
        Value v = Value::MakeMixedData(mixed_str1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_str1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kShortStr);
        auto* short_str_mixed_ptr = (ShortStrMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(short_str_mixed_ptr->length, str.size());
        EXPECT_STREQ(short_str_mixed_ptr->ptr, str.c_str());
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }
}

TEST_F(ColumnVectorMixedTest, mixed_long_str_a) {
    using namespace infinity;

    DataType data_type(LogicalType::kMixed);
    ColumnVector col_mixed(data_type, ColumnVectorType::kFlat);
    col_mixed.Initialize();

    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    auto tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        String str(20, 'a' + i % 26);
        String header(5, 'a' + i % 26);
        MixedT mixed_str1 = MixedType::MakeString(str);
        Value v = Value::MakeMixedData(mixed_str1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_str1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kLongStr);
        auto* long_str_mixed_ptr = (LongStrMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(long_str_mixed_ptr->length, str.size());

        String value_long_str(long_str_mixed_ptr->ptr, long_str_mixed_ptr->length);
        EXPECT_STREQ(value_long_str.c_str(), str.c_str());

        String value_header(long_str_mixed_ptr->header, BaseMixedType::LONG_STR_HEADER);
        EXPECT_STREQ(value_header.c_str(), header.c_str());
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reserve(DEFAULT_VECTOR_SIZE* 2);


    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        String str(20, 'a' + i % 26);
        String header(5, 'a' + i % 26);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kLongStr);
        auto* long_str_mixed_ptr = (LongStrMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(long_str_mixed_ptr->length, str.size());

        String value_long_str(long_str_mixed_ptr->ptr, long_str_mixed_ptr->length);
        EXPECT_STREQ(value_long_str.c_str(), str.c_str());

        String value_header(long_str_mixed_ptr->header, BaseMixedType::LONG_STR_HEADER);
        EXPECT_STREQ(value_header.c_str(), header.c_str());
    }

    EXPECT_EQ(col_mixed.tail_index_, DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.capacity(), 2* DEFAULT_VECTOR_SIZE);
    for(i64 i = DEFAULT_VECTOR_SIZE; i < 2 * DEFAULT_VECTOR_SIZE; ++ i) {
        String str(20, 'a' + i % 26);
        String header(5, 'a' + i % 26);
        MixedT mixed_str1 = MixedType::MakeString(str);
        Value v = Value::MakeMixedData(mixed_str1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_str1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kLongStr);
        auto* long_str_mixed_ptr = (LongStrMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(long_str_mixed_ptr->length, str.size());

        String value_long_str(long_str_mixed_ptr->ptr, long_str_mixed_ptr->length);
        EXPECT_STREQ(value_long_str.c_str(), str.c_str());

        String value_header(long_str_mixed_ptr->header, BaseMixedType::LONG_STR_HEADER);
        EXPECT_STREQ(value_header.c_str(), header.c_str());
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reset();

    EXPECT_EQ(col_mixed.capacity(), 0);
    EXPECT_EQ(col_mixed.tail_index_, 0);
//    EXPECT_EQ(col_mixed.data_type_size_, 0);
    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_EQ(col_mixed.buffer_->heap_mgr_, nullptr);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.initialized, false);

    // ====
    col_mixed.Initialize();
    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);
    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        String str(20, 'a' + i % 26);
        String header(5, 'a' + i % 26);
        MixedT mixed_str1 = MixedType::MakeString(str);
        Value v = Value::MakeMixedData(mixed_str1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_str1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kLongStr);
        auto* long_str_mixed_ptr = (LongStrMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(long_str_mixed_ptr->length, str.size());

        String value_long_str(long_str_mixed_ptr->ptr, long_str_mixed_ptr->length);
        EXPECT_STREQ(value_long_str.c_str(), str.c_str());

        String value_header(long_str_mixed_ptr->header, BaseMixedType::LONG_STR_HEADER);
        EXPECT_STREQ(value_header.c_str(), header.c_str());
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }
}

TEST_F(ColumnVectorMixedTest, mixed_null_a) {
    using namespace infinity;

    DataType data_type(LogicalType::kMixed);
    ColumnVector col_mixed(data_type, ColumnVectorType::kFlat);
    col_mixed.Initialize();

    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    auto tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_null = MixedType::MakeNull();
        Value v = Value::MakeMixedData(mixed_null);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_null);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kNull);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reserve(DEFAULT_VECTOR_SIZE* 2);


    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kNull);
    }

    EXPECT_EQ(col_mixed.tail_index_, DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.capacity(), 2* DEFAULT_VECTOR_SIZE);
    for(i64 i = DEFAULT_VECTOR_SIZE; i < 2 * DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_null = MixedType::MakeNull();
        Value v = Value::MakeMixedData(mixed_null);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_null);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kNull);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reset();

    EXPECT_EQ(col_mixed.capacity(), 0);
    EXPECT_EQ(col_mixed.tail_index_, 0);
//    EXPECT_EQ(col_mixed.data_type_size_, 0);
    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.initialized, false);

    // ====
    col_mixed.Initialize();
    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);
    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_null = MixedType::MakeNull();
        Value v = Value::MakeMixedData(mixed_null);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_null);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kNull);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }
}

TEST_F(ColumnVectorMixedTest, mixed_missing_a) {
    using namespace infinity;

    DataType data_type(LogicalType::kMixed);
    ColumnVector col_mixed(data_type, ColumnVectorType::kFlat);
    col_mixed.Initialize();

    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    auto tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_missing = MixedType::MakeMissing();
        Value v = Value::MakeMixedData(mixed_missing);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_missing);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kMissing);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reserve(DEFAULT_VECTOR_SIZE* 2);


    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kMissing);
    }

    EXPECT_EQ(col_mixed.tail_index_, DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.capacity(), 2* DEFAULT_VECTOR_SIZE);
    for(i64 i = DEFAULT_VECTOR_SIZE; i < 2 * DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_missing = MixedType::MakeMissing();
        Value v = Value::MakeMixedData(mixed_missing);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_missing);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kMissing);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reset();

    EXPECT_EQ(col_mixed.capacity(), 0);
    EXPECT_EQ(col_mixed.tail_index_, 0);
//    EXPECT_EQ(col_mixed.data_type_size_, 0);
    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.initialized, false);

    // ====
    col_mixed.Initialize();
    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);
    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        MixedT mixed_missing = MixedType::MakeMissing();
        Value v = Value::MakeMixedData(mixed_missing);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_missing);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kMissing);
        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }
}

TEST_F(ColumnVectorMixedTest, mixed_tuple_a) {
    using namespace infinity;

    DataType data_type(LogicalType::kMixed);
    ColumnVector col_mixed(data_type, ColumnVectorType::kFlat);
    col_mixed.Initialize();

    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    auto tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {

        MixedType mixed_tuple1 = MixedType::MakeTuple(7);
        mixed_tuple1.InsertIntegerIntoTuple("key1", i);
        f64 fvalue = static_cast<f64>(i) + static_cast<f64>(i) / 10.0f;
        mixed_tuple1.InsertFloatIntoTuple("key2", fvalue);

        String short_str(10, 'a' + i % 26);
        mixed_tuple1.InsertStringIntoTuple("key3", short_str);

        String long_str(20, 'a' + i % 26);
        mixed_tuple1.InsertStringIntoTuple("key4", long_str);
        mixed_tuple1.InsertNullIntoTuple("key5");

        // Key6: tuple with 5 trivial type inside
        {
            MixedType mixed_tuple_key6 = MixedType::MakeTuple(5);
            // Key61: integer
            mixed_tuple_key6.InsertIntegerIntoTuple("key61", i);
            // Key62: float
            mixed_tuple_key6.InsertFloatIntoTuple("key62", fvalue);
            // Key63: short string
            mixed_tuple_key6.InsertStringIntoTuple("key63", short_str);
            // Key64: long string
            mixed_tuple_key6.InsertStringIntoTuple("key64", long_str);
            // Key65: null
            mixed_tuple_key6.InsertNullIntoTuple("key65");

            mixed_tuple1.CopyIntoTuple("key6", mixed_tuple_key6);
        }

        // Key7: array with 5 trivial type inside
        {
            MixedType mixed_array7 = MixedType::MakeArray(5);
            // Integer
            mixed_array7.InsertIntegerIntoArray(i, 0);
            // Float
            mixed_array7.InsertFloatIntoArray(fvalue, 1);
            // Short str
            mixed_array7.InsertStringIntoArray(short_str, 2);
            // Long str
            mixed_array7.InsertStringIntoArray(long_str, 3);
            // Null
            mixed_array7.InsertNullIntoArray(4);
            mixed_tuple1.CopyIntoTuple("key7", mixed_array7);
        }

        Value v = Value::MakeMixedData(mixed_tuple1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_tuple1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kTuple);

        const MixedT& tuple_ref = vx.value_.mixed_value;
        auto* tuple_mixed_ptr = (TupleMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(tuple_mixed_ptr->count, 7);
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key1");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
            auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
            EXPECT_EQ(integer_value_ptr->value, i);
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key2");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kFloat);
            auto *float_value_ptr = (FloatMixedType *) (value1_ptr);
            EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key3");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kShortStr);
            auto *short_str_value_ptr = (ShortStrMixedType *) (value1_ptr);

            String result(short_str_value_ptr->ptr, short_str_value_ptr->length);
            EXPECT_STREQ(result.c_str(), short_str.c_str());
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key4");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kLongStr);
            auto *long_str_value_ptr = (LongStrMixedType *) (value1_ptr);

            String result(long_str_value_ptr->ptr, long_str_value_ptr->length);
            EXPECT_STREQ(result.c_str(), long_str.c_str());
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key5");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kNull);
        }

        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key6");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kTuple);
            auto *tuple_value_ptr = (TupleMixedType *) (value1_ptr);
            EXPECT_EQ(tuple_value_ptr->count, 5);
            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key61");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kInteger);
                auto *nested_integer_value_ptr = (IntegerMixedType *) (nested_value_ptr);
                EXPECT_EQ(nested_integer_value_ptr->value, i);
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key62");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kFloat);
                auto *nested_float_value_ptr = (FloatMixedType *) (nested_value_ptr);
                EXPECT_FLOAT_EQ(nested_float_value_ptr->value, fvalue);
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key63");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kShortStr);
                auto *nested_short_str_ptr = (ShortStrMixedType *) (nested_value_ptr);

                String result(nested_short_str_ptr->ptr, nested_short_str_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key64");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kLongStr);
                auto *nested_long_str_ptr = (LongStrMixedType *) (nested_value_ptr);

                String result(nested_long_str_ptr->ptr, nested_long_str_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key65");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kNull);
            }
        }

        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key7");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kArray);
            auto *array_value_ptr = (ArrayMixedType *) (value1_ptr);
            EXPECT_EQ(array_value_ptr->count, 5);

            {
                auto *array_value1_ptr = value1_ptr->GetByIndex(0);
                EXPECT_EQ(array_value1_ptr->type, MixedValueType::kInteger);
                auto *integer_value_ptr = (IntegerMixedType *) (array_value1_ptr);
                EXPECT_EQ(integer_value_ptr->value, i);
            }
            {
                auto *array_value2_ptr = value1_ptr->GetByIndex(1);
                EXPECT_EQ(array_value2_ptr->type, MixedValueType::kFloat);
                auto *float_value_ptr = (FloatMixedType *) (array_value2_ptr);
                EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
            }
            {
                auto *array_value3_ptr = value1_ptr->GetByIndex(2);
                EXPECT_EQ(array_value3_ptr->type, MixedValueType::kShortStr);
                auto *short_str_value_ptr = (ShortStrMixedType *) (array_value3_ptr);

                String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }
            // Long str
            {
                auto *array_value4_ptr = value1_ptr->GetByIndex(3);
                EXPECT_EQ(array_value4_ptr->type, MixedValueType::kLongStr);
                auto *long_str_value_ptr = (LongStrMixedType *) (array_value4_ptr);

                String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }
            // Null
            {
                auto *array_value5_ptr = value1_ptr->GetByIndex(4);
                EXPECT_EQ(array_value5_ptr->type, MixedValueType::kNull);
            }
        }

        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reserve(DEFAULT_VECTOR_SIZE* 2);


    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        f64 fvalue = static_cast<f64>(i) + static_cast<f64>(i) / 10.0f;
        String short_str(10, 'a' + i % 26);
        String long_str(20, 'a' + i % 26);

        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kTuple);

        const MixedT& tuple_ref = vx.value_.mixed_value;
        auto* tuple_mixed_ptr = (TupleMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(tuple_mixed_ptr->count, 7);
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key1");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
            auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
            EXPECT_EQ(integer_value_ptr->value, i);
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key2");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kFloat);
            auto *float_value_ptr = (FloatMixedType *) (value1_ptr);
            EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key3");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kShortStr);
            auto *short_str_value_ptr = (ShortStrMixedType *) (value1_ptr);

            String result(short_str_value_ptr->ptr, short_str_value_ptr->length);
            EXPECT_STREQ(result.c_str(), short_str.c_str());
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key4");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kLongStr);
            auto *long_str_value_ptr = (LongStrMixedType *) (value1_ptr);

            String result(long_str_value_ptr->ptr, long_str_value_ptr->length);
            EXPECT_STREQ(result.c_str(), long_str.c_str());
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key5");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kNull);
        }

        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key6");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kTuple);
            auto *tuple_value_ptr = (TupleMixedType *) (value1_ptr);
            EXPECT_EQ(tuple_value_ptr->count, 5);
            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key61");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kInteger);
                auto *nested_integer_value_ptr = (IntegerMixedType *) (nested_value_ptr);
                EXPECT_EQ(nested_integer_value_ptr->value, i);
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key62");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kFloat);
                auto *nested_float_value_ptr = (FloatMixedType *) (nested_value_ptr);
                EXPECT_FLOAT_EQ(nested_float_value_ptr->value, fvalue);
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key63");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kShortStr);
                auto *nested_short_str_ptr = (ShortStrMixedType *) (nested_value_ptr);

                String result(nested_short_str_ptr->ptr, nested_short_str_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key64");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kLongStr);
                auto *nested_long_str_ptr = (LongStrMixedType *) (nested_value_ptr);

                String result(nested_long_str_ptr->ptr, nested_long_str_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key65");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kNull);
            }
        }

        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key7");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kArray);
            auto *array_value_ptr = (ArrayMixedType *) (value1_ptr);
            EXPECT_EQ(array_value_ptr->count, 5);

            {
                auto *array_value1_ptr = value1_ptr->GetByIndex(0);
                EXPECT_EQ(array_value1_ptr->type, MixedValueType::kInteger);
                auto *integer_value_ptr = (IntegerMixedType *) (array_value1_ptr);
                EXPECT_EQ(integer_value_ptr->value, i);
            }
            {
                auto *array_value2_ptr = value1_ptr->GetByIndex(1);
                EXPECT_EQ(array_value2_ptr->type, MixedValueType::kFloat);
                auto *float_value_ptr = (FloatMixedType *) (array_value2_ptr);
                EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
            }
            {
                auto *array_value3_ptr = value1_ptr->GetByIndex(2);
                EXPECT_EQ(array_value3_ptr->type, MixedValueType::kShortStr);
                auto *short_str_value_ptr = (ShortStrMixedType *) (array_value3_ptr);

                String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }
            // Long str
            {
                auto *array_value4_ptr = value1_ptr->GetByIndex(3);
                EXPECT_EQ(array_value4_ptr->type, MixedValueType::kLongStr);
                auto *long_str_value_ptr = (LongStrMixedType *) (array_value4_ptr);

                String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }
            // Null
            {
                auto *array_value5_ptr = value1_ptr->GetByIndex(4);
                EXPECT_EQ(array_value5_ptr->type, MixedValueType::kNull);
            }
        }
    }


    EXPECT_EQ(col_mixed.tail_index_, DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.capacity(), 2* DEFAULT_VECTOR_SIZE);
    for(i64 i = DEFAULT_VECTOR_SIZE; i < 2 * DEFAULT_VECTOR_SIZE; ++ i) {

        MixedType mixed_tuple1 = MixedType::MakeTuple(7);
        mixed_tuple1.InsertIntegerIntoTuple("key1", i);
        f64 fvalue = static_cast<f64>(i) + static_cast<f64>(i) / 10.0f;
        mixed_tuple1.InsertFloatIntoTuple("key2", fvalue);

        String short_str(10, 'a' + i % 26);
        mixed_tuple1.InsertStringIntoTuple("key3", short_str);

        String long_str(20, 'a' + i % 26);
        mixed_tuple1.InsertStringIntoTuple("key4", long_str);
        mixed_tuple1.InsertNullIntoTuple("key5");

        // Key6: tuple with 5 trivial type inside
        {
            MixedType mixed_tuple_key6 = MixedType::MakeTuple(5);
            // Key61: integer
            mixed_tuple_key6.InsertIntegerIntoTuple("key61", i);
            // Key62: float
            mixed_tuple_key6.InsertFloatIntoTuple("key62", fvalue);
            // Key63: short string
            mixed_tuple_key6.InsertStringIntoTuple("key63", short_str);
            // Key64: long string
            mixed_tuple_key6.InsertStringIntoTuple("key64", long_str);
            // Key65: null
            mixed_tuple_key6.InsertNullIntoTuple("key65");

            mixed_tuple1.CopyIntoTuple("key6", mixed_tuple_key6);
        }

        // Key7: array with 5 trivial type inside
        {
            MixedType mixed_array7 = MixedType::MakeArray(5);
            // Integer
            mixed_array7.InsertIntegerIntoArray(i, 0);
            // Float
            mixed_array7.InsertFloatIntoArray(fvalue, 1);
            // Short str
            mixed_array7.InsertStringIntoArray(short_str, 2);
            // Long str
            mixed_array7.InsertStringIntoArray(long_str, 3);
            // Null
            mixed_array7.InsertNullIntoArray(4);
            mixed_tuple1.CopyIntoTuple("key7", mixed_array7);
        }

        Value v = Value::MakeMixedData(mixed_tuple1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_tuple1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kTuple);

        const MixedT& tuple_ref = vx.value_.mixed_value;
        auto* tuple_mixed_ptr = (TupleMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(tuple_mixed_ptr->count, 7);
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key1");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
            auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
            EXPECT_EQ(integer_value_ptr->value, i);
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key2");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kFloat);
            auto *float_value_ptr = (FloatMixedType *) (value1_ptr);
            EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key3");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kShortStr);
            auto *short_str_value_ptr = (ShortStrMixedType *) (value1_ptr);

            String result(short_str_value_ptr->ptr, short_str_value_ptr->length);
            EXPECT_STREQ(result.c_str(), short_str.c_str());
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key4");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kLongStr);
            auto *long_str_value_ptr = (LongStrMixedType *) (value1_ptr);

            String result(long_str_value_ptr->ptr, long_str_value_ptr->length);
            EXPECT_STREQ(result.c_str(), long_str.c_str());
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key5");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kNull);
        }

        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key6");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kTuple);
            auto *tuple_value_ptr = (TupleMixedType *) (value1_ptr);
            EXPECT_EQ(tuple_value_ptr->count, 5);
            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key61");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kInteger);
                auto *nested_integer_value_ptr = (IntegerMixedType *) (nested_value_ptr);
                EXPECT_EQ(nested_integer_value_ptr->value, i);
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key62");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kFloat);
                auto *nested_float_value_ptr = (FloatMixedType *) (nested_value_ptr);
                EXPECT_FLOAT_EQ(nested_float_value_ptr->value, fvalue);
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key63");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kShortStr);
                auto *nested_short_str_ptr = (ShortStrMixedType *) (nested_value_ptr);

                String result(nested_short_str_ptr->ptr, nested_short_str_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key64");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kLongStr);
                auto *nested_long_str_ptr = (LongStrMixedType *) (nested_value_ptr);

                String result(nested_long_str_ptr->ptr, nested_long_str_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key65");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kNull);
            }
        }

        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key7");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kArray);
            auto *array_value_ptr = (ArrayMixedType *) (value1_ptr);
            EXPECT_EQ(array_value_ptr->count, 5);

            {
                auto *array_value1_ptr = value1_ptr->GetByIndex(0);
                EXPECT_EQ(array_value1_ptr->type, MixedValueType::kInteger);
                auto *integer_value_ptr = (IntegerMixedType *) (array_value1_ptr);
                EXPECT_EQ(integer_value_ptr->value, i);
            }
            {
                auto *array_value2_ptr = value1_ptr->GetByIndex(1);
                EXPECT_EQ(array_value2_ptr->type, MixedValueType::kFloat);
                auto *float_value_ptr = (FloatMixedType *) (array_value2_ptr);
                EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
            }
            {
                auto *array_value3_ptr = value1_ptr->GetByIndex(2);
                EXPECT_EQ(array_value3_ptr->type, MixedValueType::kShortStr);
                auto *short_str_value_ptr = (ShortStrMixedType *) (array_value3_ptr);

                String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }
            // Long str
            {
                auto *array_value4_ptr = value1_ptr->GetByIndex(3);
                EXPECT_EQ(array_value4_ptr->type, MixedValueType::kLongStr);
                auto *long_str_value_ptr = (LongStrMixedType *) (array_value4_ptr);

                String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }
            // Null
            {
                auto *array_value5_ptr = value1_ptr->GetByIndex(4);
                EXPECT_EQ(array_value5_ptr->type, MixedValueType::kNull);
            }
        }

        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reset();

    EXPECT_EQ(col_mixed.capacity(), 0);
    EXPECT_EQ(col_mixed.tail_index_, 0);
//    EXPECT_EQ(col_mixed.data_type_size_, 0);
    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_EQ(col_mixed.buffer_->heap_mgr_, nullptr);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.initialized, false);

    // ====
    col_mixed.Initialize();
    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);
    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {

        MixedType mixed_tuple1 = MixedType::MakeTuple(7);
        mixed_tuple1.InsertIntegerIntoTuple("key1", i);
        f64 fvalue = static_cast<f64>(i) + static_cast<f64>(i) / 10.0f;
        mixed_tuple1.InsertFloatIntoTuple("key2", fvalue);

        String short_str(10, 'a' + i % 26);
        mixed_tuple1.InsertStringIntoTuple("key3", short_str);

        String long_str(20, 'a' + i % 26);
        mixed_tuple1.InsertStringIntoTuple("key4", long_str);
        mixed_tuple1.InsertNullIntoTuple("key5");

        // Key6: tuple with 5 trivial type inside
        {
            MixedType mixed_tuple_key6 = MixedType::MakeTuple(5);
            // Key61: integer
            mixed_tuple_key6.InsertIntegerIntoTuple("key61", i);
            // Key62: float
            mixed_tuple_key6.InsertFloatIntoTuple("key62", fvalue);
            // Key63: short string
            mixed_tuple_key6.InsertStringIntoTuple("key63", short_str);
            // Key64: long string
            mixed_tuple_key6.InsertStringIntoTuple("key64", long_str);
            // Key65: null
            mixed_tuple_key6.InsertNullIntoTuple("key65");

            mixed_tuple1.CopyIntoTuple("key6", mixed_tuple_key6);
        }

        // Key7: array with 5 trivial type inside
        {
            MixedType mixed_array7 = MixedType::MakeArray(5);
            // Integer
            mixed_array7.InsertIntegerIntoArray(i, 0);
            // Float
            mixed_array7.InsertFloatIntoArray(fvalue, 1);
            // Short str
            mixed_array7.InsertStringIntoArray(short_str, 2);
            // Long str
            mixed_array7.InsertStringIntoArray(long_str, 3);
            // Null
            mixed_array7.InsertNullIntoArray(4);
            mixed_tuple1.CopyIntoTuple("key7", mixed_array7);
        }

        Value v = Value::MakeMixedData(mixed_tuple1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_tuple1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kTuple);

        const MixedT& tuple_ref = vx.value_.mixed_value;
        auto* tuple_mixed_ptr = (TupleMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(tuple_mixed_ptr->count, 7);
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key1");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
            auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
            EXPECT_EQ(integer_value_ptr->value, i);
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key2");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kFloat);
            auto *float_value_ptr = (FloatMixedType *) (value1_ptr);
            EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key3");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kShortStr);
            auto *short_str_value_ptr = (ShortStrMixedType *) (value1_ptr);

            String result(short_str_value_ptr->ptr, short_str_value_ptr->length);
            EXPECT_STREQ(result.c_str(), short_str.c_str());
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key4");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kLongStr);
            auto *long_str_value_ptr = (LongStrMixedType *) (value1_ptr);

            String result(long_str_value_ptr->ptr, long_str_value_ptr->length);
            EXPECT_STREQ(result.c_str(), long_str.c_str());
        }
        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key5");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kNull);
        }

        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key6");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kTuple);
            auto *tuple_value_ptr = (TupleMixedType *) (value1_ptr);
            EXPECT_EQ(tuple_value_ptr->count, 5);
            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key61");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kInteger);
                auto *nested_integer_value_ptr = (IntegerMixedType *) (nested_value_ptr);
                EXPECT_EQ(nested_integer_value_ptr->value, i);
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key62");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kFloat);
                auto *nested_float_value_ptr = (FloatMixedType *) (nested_value_ptr);
                EXPECT_FLOAT_EQ(nested_float_value_ptr->value, fvalue);
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key63");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kShortStr);
                auto *nested_short_str_ptr = (ShortStrMixedType *) (nested_value_ptr);

                String result(nested_short_str_ptr->ptr, nested_short_str_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key64");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kLongStr);
                auto *nested_long_str_ptr = (LongStrMixedType *) (nested_value_ptr);

                String result(nested_long_str_ptr->ptr, nested_long_str_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }

            {
                auto* nested_value_ptr = value1_ptr->GetFromTuple("key65");
                EXPECT_EQ(nested_value_ptr->type, MixedValueType::kNull);
            }
        }

        {
            auto *value1_ptr = tuple_ref.GetFromTuple("key7");
            EXPECT_EQ(value1_ptr->type, MixedValueType::kArray);
            auto *array_value_ptr = (ArrayMixedType *) (value1_ptr);
            EXPECT_EQ(array_value_ptr->count, 5);

            {
                auto *array_value1_ptr = value1_ptr->GetByIndex(0);
                EXPECT_EQ(array_value1_ptr->type, MixedValueType::kInteger);
                auto *integer_value_ptr = (IntegerMixedType *) (array_value1_ptr);
                EXPECT_EQ(integer_value_ptr->value, i);
            }
            {
                auto *array_value2_ptr = value1_ptr->GetByIndex(1);
                EXPECT_EQ(array_value2_ptr->type, MixedValueType::kFloat);
                auto *float_value_ptr = (FloatMixedType *) (array_value2_ptr);
                EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
            }
            {
                auto *array_value3_ptr = value1_ptr->GetByIndex(2);
                EXPECT_EQ(array_value3_ptr->type, MixedValueType::kShortStr);
                auto *short_str_value_ptr = (ShortStrMixedType *) (array_value3_ptr);

                String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }
            // Long str
            {
                auto *array_value4_ptr = value1_ptr->GetByIndex(3);
                EXPECT_EQ(array_value4_ptr->type, MixedValueType::kLongStr);
                auto *long_str_value_ptr = (LongStrMixedType *) (array_value4_ptr);

                String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }
            // Null
            {
                auto *array_value5_ptr = value1_ptr->GetByIndex(4);
                EXPECT_EQ(array_value5_ptr->type, MixedValueType::kNull);
            }
        }

        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }
}

TEST_F(ColumnVectorMixedTest, mixed_array_a) {
    using namespace infinity;

    DataType data_type(LogicalType::kMixed);
    ColumnVector col_mixed(data_type, ColumnVectorType::kFlat);
    col_mixed.Initialize();

    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    auto tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        f64 fvalue = static_cast<f64>(i) + static_cast<f64>(i) / 10.0f;
        String short_str(10, 'a' + i % 26);
        String long_str(20, 'a' + i % 26);

        MixedType mixed_array1 = MixedType::MakeArray(7);
        EXPECT_EQ(mixed_array1.type, MixedValueType::kArray);
        // 1. Integer
        mixed_array1.InsertIntegerIntoArray(i, 0);
        // 2. Float
        mixed_array1.InsertFloatIntoArray(fvalue, 1);
        // 3. Short str
        mixed_array1.InsertStringIntoArray(short_str, 2);
        // 4. Long str
        mixed_array1.InsertStringIntoArray(long_str, 3);
        // 5. Null
        mixed_array1.InsertNullIntoArray(4);
        // 6. Tuple
        {
            MixedType mixed_tuple5 = MixedType::MakeTuple(5);
            EXPECT_EQ(mixed_tuple5.type, MixedValueType::kTuple);
            // Key51: integer
            mixed_tuple5.InsertIntegerIntoTuple("key51", i);
            // Key52: float
            mixed_tuple5.InsertFloatIntoTuple("key52", fvalue);
            // Key53: short string
            mixed_tuple5.InsertStringIntoTuple("key53", short_str);
            // Key54: long string
            mixed_tuple5.InsertStringIntoTuple("key54", long_str);
            // Key55: null
            mixed_tuple5.InsertNullIntoTuple("key55");

            mixed_array1.CopyIntoArray(mixed_tuple5, 5);
        }

        {
            MixedType nested_array = MixedType::MakeArray(5);
            // Integer
            nested_array.InsertIntegerIntoArray(i, 0);
            // Float
            nested_array.InsertFloatIntoArray(fvalue, 1);
            // Short str
            nested_array.InsertStringIntoArray(short_str, 2);
            // Long str
            nested_array.InsertStringIntoArray(long_str, 3);
            // Null
            nested_array.InsertNullIntoArray(4);

            mixed_array1.CopyIntoArray(nested_array, 6);
        }

        Value v = Value::MakeMixedData(mixed_array1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_array1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kArray);

        const MixedT& array_ref = vx.value_.mixed_value;
        auto* array_mixed_ptr = (ArrayMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(array_mixed_ptr->count, 7);

        {
            // 1. Integer
            {
                auto *value1_ptr = array_ref.GetByIndex(0);
                EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                EXPECT_EQ(integer_value_ptr->value, i);
            }
            // 2. Float
            {
                auto *value2_ptr = array_ref.GetByIndex(1);
                EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
            }
            // 3. Short str
            {
                auto *value3_ptr = array_ref.GetByIndex(2);
                EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }
            // 4. Long str
            {
                auto *value4_ptr = array_ref.GetByIndex(3);
                EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }
            // 5. Null
            {
                auto *value5_ptr = array_ref.GetByIndex(4);
                EXPECT_EQ(value5_ptr->type, MixedValueType::kNull);
            }

            // 6. Tuple
            {
                auto *value6_ptr = mixed_array1.GetByIndex(5);
                {
                    EXPECT_EQ(value6_ptr->type, MixedValueType::kTuple);
                    {
                        auto *tuple_ptr5 = (TupleMixedType *) (value6_ptr);
                        EXPECT_EQ(tuple_ptr5->count, 5);
                    }
                    // Key51: integer
                    {
                        auto *value1_ptr = value6_ptr->GetFromTuple("key51");
                        EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                        auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                        EXPECT_EQ(integer_value_ptr->value, i);
                    }
                    // Key52: float
                    {
                        auto *value2_ptr = value6_ptr->GetFromTuple("key52");
                        EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                        auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                        EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
                    }
                    // Key53: short string
                    {
                        auto *value3_ptr = value6_ptr->GetFromTuple("key53");
                        EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                        auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                        String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), short_str.c_str());
                    }
                    // Key54: long string
                    {
                        auto *value4_ptr = value6_ptr->GetFromTuple("key54");
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                        auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                        String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), long_str.c_str());
                    }

                    // Key55: null
                    {
                        auto *value5_ptr = value6_ptr->GetFromTuple("key55");
                        EXPECT_EQ(value5_ptr->type, MixedValueType::kNull);
                    }
                }
            }

            // 7. Array
            {
                auto *value7_ptr = array_ref.GetByIndex(6);
                {
                    EXPECT_EQ(value7_ptr->type, MixedValueType::kArray);
                    {
                        auto *nested_array_ptr1 = (ArrayMixedType *) (value7_ptr);
                        EXPECT_EQ(nested_array_ptr1->count, 5);
                    }
                    // Integer
                    {
                        auto *value1_ptr = value7_ptr->GetByIndex(0);
                        EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                        auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                        EXPECT_EQ(integer_value_ptr->value, i);
                    }
                    // Float
                    {
                        auto *value2_ptr = value7_ptr->GetByIndex(1);
                        EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                        auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                        EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
                    }
                    // Short str
                    {
                        auto *value3_ptr = value7_ptr->GetByIndex(2);
                        EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                        auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                        String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), short_str.c_str());
                    }
                    // Long str
                    {
                        auto *value4_ptr = value7_ptr->GetByIndex(3);
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                        auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                        String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), long_str.c_str());
                    }

                    // Null
                    {
                        auto *value4_ptr = value7_ptr->GetByIndex(4);
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kNull);
                    }
                }
            }
        }

        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reserve(DEFAULT_VECTOR_SIZE* 2);

    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        f64 fvalue = static_cast<f64>(i) + static_cast<f64>(i) / 10.0f;
        String short_str(10, 'a' + i % 26);
        String long_str(20, 'a' + i % 26);

        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kArray);

        const MixedT& array_ref = vx.value_.mixed_value;
        auto* array_mixed_ptr = (ArrayMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(array_mixed_ptr->count, 7);

        {
            // 1. Integer
            {
                auto *value1_ptr = array_ref.GetByIndex(0);
                EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                EXPECT_EQ(integer_value_ptr->value, i);
            }
            // 2. Float
            {
                auto *value2_ptr = array_ref.GetByIndex(1);
                EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
            }
            // 3. Short str
            {
                auto *value3_ptr = array_ref.GetByIndex(2);
                EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }
            // 4. Long str
            {
                auto *value4_ptr = array_ref.GetByIndex(3);
                EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }
            // 5. Null
            {
                auto *value5_ptr = array_ref.GetByIndex(4);
                EXPECT_EQ(value5_ptr->type, MixedValueType::kNull);
            }

            // 6. Tuple
            {
                auto *value6_ptr = array_ref.GetByIndex(5);
                {
                    EXPECT_EQ(value6_ptr->type, MixedValueType::kTuple);
                    {
                        auto *tuple_ptr5 = (TupleMixedType *) (value6_ptr);
                        EXPECT_EQ(tuple_ptr5->count, 5);
                    }
                    // Key51: integer
                    {
                        auto *value1_ptr = value6_ptr->GetFromTuple("key51");
                        EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                        auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                        EXPECT_EQ(integer_value_ptr->value, i);
                    }
                    // Key52: float
                    {
                        auto *value2_ptr = value6_ptr->GetFromTuple("key52");
                        EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                        auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                        EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
                    }
                    // Key53: short string
                    {
                        auto *value3_ptr = value6_ptr->GetFromTuple("key53");
                        EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                        auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                        String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), short_str.c_str());
                    }
                    // Key54: long string
                    {
                        auto *value4_ptr = value6_ptr->GetFromTuple("key54");
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                        auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                        String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), long_str.c_str());
                    }

                    // Key55: null
                    {
                        auto *value5_ptr = value6_ptr->GetFromTuple("key55");
                        EXPECT_EQ(value5_ptr->type, MixedValueType::kNull);
                    }
                }
            }

            // 7. Array
            {
                auto *value7_ptr = array_ref.GetByIndex(6);
                {
                    EXPECT_EQ(value7_ptr->type, MixedValueType::kArray);
                    {
                        auto *nested_array_ptr1 = (ArrayMixedType *) (value7_ptr);
                        EXPECT_EQ(nested_array_ptr1->count, 5);
                    }
                    // Integer
                    {
                        auto *value1_ptr = value7_ptr->GetByIndex(0);
                        EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                        auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                        EXPECT_EQ(integer_value_ptr->value, i);
                    }
                    // Float
                    {
                        auto *value2_ptr = value7_ptr->GetByIndex(1);
                        EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                        auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                        EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
                    }
                    // Short str
                    {
                        auto *value3_ptr = value7_ptr->GetByIndex(2);
                        EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                        auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                        String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), short_str.c_str());
                    }
                    // Long str
                    {
                        auto *value4_ptr = value7_ptr->GetByIndex(3);
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                        auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                        String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), long_str.c_str());
                    }

                    // Null
                    {
                        auto *value4_ptr = value7_ptr->GetByIndex(4);
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kNull);
                    }
                }
            }
        }

    }


    EXPECT_EQ(col_mixed.tail_index_, DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.capacity(), 2* DEFAULT_VECTOR_SIZE);
    for(i64 i = DEFAULT_VECTOR_SIZE; i < 2 * DEFAULT_VECTOR_SIZE; ++ i) {
        f64 fvalue = static_cast<f64>(i) + static_cast<f64>(i) / 10.0f;
        String short_str(10, 'a' + i % 26);
        String long_str(20, 'a' + i % 26);

        MixedType mixed_array1 = MixedType::MakeArray(7);
        EXPECT_EQ(mixed_array1.type, MixedValueType::kArray);
        // 1. Integer
        mixed_array1.InsertIntegerIntoArray(i, 0);
        // 2. Float
        mixed_array1.InsertFloatIntoArray(fvalue, 1);
        // 3. Short str
        mixed_array1.InsertStringIntoArray(short_str, 2);
        // 4. Long str
        mixed_array1.InsertStringIntoArray(long_str, 3);
        // 5. Null
        mixed_array1.InsertNullIntoArray(4);
        // 6. Tuple
        {
            MixedType mixed_tuple5 = MixedType::MakeTuple(5);
            EXPECT_EQ(mixed_tuple5.type, MixedValueType::kTuple);
            // Key51: integer
            mixed_tuple5.InsertIntegerIntoTuple("key51", i);
            // Key52: float
            mixed_tuple5.InsertFloatIntoTuple("key52", fvalue);
            // Key53: short string
            mixed_tuple5.InsertStringIntoTuple("key53", short_str);
            // Key54: long string
            mixed_tuple5.InsertStringIntoTuple("key54", long_str);
            // Key55: null
            mixed_tuple5.InsertNullIntoTuple("key55");

            mixed_array1.CopyIntoArray(mixed_tuple5, 5);
        }

        {
            MixedType nested_array = MixedType::MakeArray(5);
            // Integer
            nested_array.InsertIntegerIntoArray(i, 0);
            // Float
            nested_array.InsertFloatIntoArray(fvalue, 1);
            // Short str
            nested_array.InsertStringIntoArray(short_str, 2);
            // Long str
            nested_array.InsertStringIntoArray(long_str, 3);
            // Null
            nested_array.InsertNullIntoArray(4);

            mixed_array1.CopyIntoArray(nested_array, 6);
        }

        Value v = Value::MakeMixedData(mixed_array1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_array1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kArray);

        const MixedT& array_ref = vx.value_.mixed_value;
        auto* array_mixed_ptr = (ArrayMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(array_mixed_ptr->count, 7);

        {
            // 1. Integer
            {
                auto *value1_ptr = array_ref.GetByIndex(0);
                EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                EXPECT_EQ(integer_value_ptr->value, i);
            }
            // 2. Float
            {
                auto *value2_ptr = array_ref.GetByIndex(1);
                EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
            }
            // 3. Short str
            {
                auto *value3_ptr = array_ref.GetByIndex(2);
                EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }
            // 4. Long str
            {
                auto *value4_ptr = array_ref.GetByIndex(3);
                EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }
            // 5. Null
            {
                auto *value5_ptr = array_ref.GetByIndex(4);
                EXPECT_EQ(value5_ptr->type, MixedValueType::kNull);
            }

            // 6. Tuple
            {
                auto *value6_ptr = mixed_array1.GetByIndex(5);
                {
                    EXPECT_EQ(value6_ptr->type, MixedValueType::kTuple);
                    {
                        auto *tuple_ptr5 = (TupleMixedType *) (value6_ptr);
                        EXPECT_EQ(tuple_ptr5->count, 5);
                    }
                    // Key51: integer
                    {
                        auto *value1_ptr = value6_ptr->GetFromTuple("key51");
                        EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                        auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                        EXPECT_EQ(integer_value_ptr->value, i);
                    }
                    // Key52: float
                    {
                        auto *value2_ptr = value6_ptr->GetFromTuple("key52");
                        EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                        auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                        EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
                    }
                    // Key53: short string
                    {
                        auto *value3_ptr = value6_ptr->GetFromTuple("key53");
                        EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                        auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                        String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), short_str.c_str());
                    }
                    // Key54: long string
                    {
                        auto *value4_ptr = value6_ptr->GetFromTuple("key54");
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                        auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                        String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), long_str.c_str());
                    }

                    // Key55: null
                    {
                        auto *value5_ptr = value6_ptr->GetFromTuple("key55");
                        EXPECT_EQ(value5_ptr->type, MixedValueType::kNull);
                    }
                }
            }

            // 7. Array
            {
                auto *value7_ptr = array_ref.GetByIndex(6);
                {
                    EXPECT_EQ(value7_ptr->type, MixedValueType::kArray);
                    {
                        auto *nested_array_ptr1 = (ArrayMixedType *) (value7_ptr);
                        EXPECT_EQ(nested_array_ptr1->count, 5);
                    }
                    // Integer
                    {
                        auto *value1_ptr = value7_ptr->GetByIndex(0);
                        EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                        auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                        EXPECT_EQ(integer_value_ptr->value, i);
                    }
                    // Float
                    {
                        auto *value2_ptr = value7_ptr->GetByIndex(1);
                        EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                        auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                        EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
                    }
                    // Short str
                    {
                        auto *value3_ptr = value7_ptr->GetByIndex(2);
                        EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                        auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                        String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), short_str.c_str());
                    }
                    // Long str
                    {
                        auto *value4_ptr = value7_ptr->GetByIndex(3);
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                        auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                        String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), long_str.c_str());
                    }

                    // Null
                    {
                        auto *value4_ptr = value7_ptr->GetByIndex(4);
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kNull);
                    }
                }
            }
        }

        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }

    col_mixed.Reset();

    EXPECT_EQ(col_mixed.capacity(), 0);
    EXPECT_EQ(col_mixed.tail_index_, 0);
//    EXPECT_EQ(col_mixed.data_type_size_, 0);
    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_EQ(col_mixed.buffer_->heap_mgr_, nullptr);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.initialized, false);

    // ====
    col_mixed.Initialize();
    EXPECT_THROW(col_mixed.SetDataType(DataType(LogicalType::kMixed)), TypeException);
    EXPECT_THROW(col_mixed.SetVectorType(ColumnVectorType::kFlat), TypeException);

    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(col_mixed.Size(), 0);
    EXPECT_THROW(col_mixed.ToString(), TypeException);
    EXPECT_THROW(col_mixed.GetValue(0), TypeException);
    EXPECT_EQ(col_mixed.tail_index_, 0);
    EXPECT_EQ(col_mixed.data_type_size_, 16);
    EXPECT_NE(col_mixed.data_ptr_, nullptr);
    EXPECT_EQ(col_mixed.vector_type(), ColumnVectorType::kFlat);
    EXPECT_EQ(col_mixed.data_type(), data_type);
    EXPECT_EQ(col_mixed.buffer_->buffer_type_, VectorBufferType::kStandard);

    EXPECT_NE(col_mixed.buffer_, nullptr);
    EXPECT_NE(col_mixed.nulls_ptr_, nullptr);
    EXPECT_TRUE(col_mixed.initialized);
    col_mixed.Reserve(DEFAULT_VECTOR_SIZE - 1);
    tmp_ptr = col_mixed.data_ptr_;
    EXPECT_EQ(col_mixed.capacity(), DEFAULT_VECTOR_SIZE);
    EXPECT_EQ(tmp_ptr, col_mixed.data_ptr_);
    for(i64 i = 0; i < DEFAULT_VECTOR_SIZE; ++ i) {
        f64 fvalue = static_cast<f64>(i) + static_cast<f64>(i) / 10.0f;
        String short_str(10, 'a' + i % 26);
        String long_str(20, 'a' + i % 26);

        MixedType mixed_array1 = MixedType::MakeArray(7);
        EXPECT_EQ(mixed_array1.type, MixedValueType::kArray);
        // 1. Integer
        mixed_array1.InsertIntegerIntoArray(i, 0);
        // 2. Float
        mixed_array1.InsertFloatIntoArray(fvalue, 1);
        // 3. Short str
        mixed_array1.InsertStringIntoArray(short_str, 2);
        // 4. Long str
        mixed_array1.InsertStringIntoArray(long_str, 3);
        // 5. Null
        mixed_array1.InsertNullIntoArray(4);
        // 6. Tuple
        {
            MixedType mixed_tuple5 = MixedType::MakeTuple(5);
            EXPECT_EQ(mixed_tuple5.type, MixedValueType::kTuple);
            // Key51: integer
            mixed_tuple5.InsertIntegerIntoTuple("key51", i);
            // Key52: float
            mixed_tuple5.InsertFloatIntoTuple("key52", fvalue);
            // Key53: short string
            mixed_tuple5.InsertStringIntoTuple("key53", short_str);
            // Key54: long string
            mixed_tuple5.InsertStringIntoTuple("key54", long_str);
            // Key55: null
            mixed_tuple5.InsertNullIntoTuple("key55");

            mixed_array1.CopyIntoArray(mixed_tuple5, 5);
        }

        {
            MixedType nested_array = MixedType::MakeArray(5);
            // Integer
            nested_array.InsertIntegerIntoArray(i, 0);
            // Float
            nested_array.InsertFloatIntoArray(fvalue, 1);
            // Short str
            nested_array.InsertStringIntoArray(short_str, 2);
            // Long str
            nested_array.InsertStringIntoArray(long_str, 3);
            // Null
            nested_array.InsertNullIntoArray(4);

            mixed_array1.CopyIntoArray(nested_array, 6);
        }

        Value v = Value::MakeMixedData(mixed_array1);
        EXPECT_EQ(v.GetValue<MixedT>(), mixed_array1);

        col_mixed.AppendValue(v);
        Value vx = col_mixed.GetValue(i);
        EXPECT_EQ(vx.type().type(), LogicalType::kMixed);
        EXPECT_EQ(vx.value_.mixed_value.type, MixedValueType::kArray);

        const MixedT& array_ref = vx.value_.mixed_value;
        auto* array_mixed_ptr = (ArrayMixedType*)(&vx.value_.mixed_value);
        EXPECT_EQ(array_mixed_ptr->count, 7);

        {
            // 1. Integer
            {
                auto *value1_ptr = array_ref.GetByIndex(0);
                EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                EXPECT_EQ(integer_value_ptr->value, i);
            }
            // 2. Float
            {
                auto *value2_ptr = array_ref.GetByIndex(1);
                EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
            }
            // 3. Short str
            {
                auto *value3_ptr = array_ref.GetByIndex(2);
                EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), short_str.c_str());
            }
            // 4. Long str
            {
                auto *value4_ptr = array_ref.GetByIndex(3);
                EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                EXPECT_STREQ(result.c_str(), long_str.c_str());
            }
            // 5. Null
            {
                auto *value5_ptr = array_ref.GetByIndex(4);
                EXPECT_EQ(value5_ptr->type, MixedValueType::kNull);
            }

            // 6. Tuple
            {
                auto *value6_ptr = mixed_array1.GetByIndex(5);
                {
                    EXPECT_EQ(value6_ptr->type, MixedValueType::kTuple);
                    {
                        auto *tuple_ptr5 = (TupleMixedType *) (value6_ptr);
                        EXPECT_EQ(tuple_ptr5->count, 5);
                    }
                    // Key51: integer
                    {
                        auto *value1_ptr = value6_ptr->GetFromTuple("key51");
                        EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                        auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                        EXPECT_EQ(integer_value_ptr->value, i);
                    }
                    // Key52: float
                    {
                        auto *value2_ptr = value6_ptr->GetFromTuple("key52");
                        EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                        auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                        EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
                    }
                    // Key53: short string
                    {
                        auto *value3_ptr = value6_ptr->GetFromTuple("key53");
                        EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                        auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                        String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), short_str.c_str());
                    }
                    // Key54: long string
                    {
                        auto *value4_ptr = value6_ptr->GetFromTuple("key54");
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                        auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                        String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), long_str.c_str());
                    }

                    // Key55: null
                    {
                        auto *value5_ptr = value6_ptr->GetFromTuple("key55");
                        EXPECT_EQ(value5_ptr->type, MixedValueType::kNull);
                    }
                }
            }

            // 7. Array
            {
                auto *value7_ptr = array_ref.GetByIndex(6);
                {
                    EXPECT_EQ(value7_ptr->type, MixedValueType::kArray);
                    {
                        auto *nested_array_ptr1 = (ArrayMixedType *) (value7_ptr);
                        EXPECT_EQ(nested_array_ptr1->count, 5);
                    }
                    // Integer
                    {
                        auto *value1_ptr = value7_ptr->GetByIndex(0);
                        EXPECT_EQ(value1_ptr->type, MixedValueType::kInteger);
                        auto *integer_value_ptr = (IntegerMixedType *) (value1_ptr);
                        EXPECT_EQ(integer_value_ptr->value, i);
                    }
                    // Float
                    {
                        auto *value2_ptr = value7_ptr->GetByIndex(1);
                        EXPECT_EQ(value2_ptr->type, MixedValueType::kFloat);
                        auto *float_value_ptr = (FloatMixedType *) (value2_ptr);
                        EXPECT_FLOAT_EQ(float_value_ptr->value, fvalue);
                    }
                    // Short str
                    {
                        auto *value3_ptr = value7_ptr->GetByIndex(2);
                        EXPECT_EQ(value3_ptr->type, MixedValueType::kShortStr);
                        auto *short_str_value_ptr = (ShortStrMixedType *) (value3_ptr);

                        String result = String(short_str_value_ptr->ptr, short_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), short_str.c_str());
                    }
                    // Long str
                    {
                        auto *value4_ptr = value7_ptr->GetByIndex(3);
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kLongStr);
                        auto *long_str_value_ptr = (LongStrMixedType *) (value4_ptr);

                        String result = String(long_str_value_ptr->ptr, long_str_value_ptr->length);
                        EXPECT_STREQ(result.c_str(), long_str.c_str());
                    }

                    // Null
                    {
                        auto *value4_ptr = value7_ptr->GetByIndex(4);
                        EXPECT_EQ(value4_ptr->type, MixedValueType::kNull);
                    }
                }
            }
        }

        EXPECT_THROW(col_mixed.GetValue(i + 1), TypeException);
    }
}
