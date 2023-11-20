// Copyright(C) 2023 InfiniFlow, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "thrift_server.h"

#include <thrift/TToString.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <memory>
#include <stdexcept>

#include "infinity_thrift/InfinityService.h"
#include "infinity_thrift/infinity_types.h"

import infinity;
import stl;
import infinity_exception;
import logger;
import query_result;
import column_vector;
import data_block;
import value;
import third_party;

using namespace ::infinity_thrift_rpc;

namespace infinity {

class InfinityServiceHandler : virtual public infinity_thrift_rpc::InfinityServiceIf {
public:
    InfinityServiceHandler() = default;

    void Connect(infinity_thrift_rpc::CommonResponse &response) override {
        auto infinity = infinity::Infinity::RemoteConnect();
        if (infinity == nullptr) {
            response.success = false;
            response.error_msg = "Connect failed";
        } else {
            infinity_session_map_mutex_.lock();
            infinity_session_map_.emplace(infinity->GetSessionId(), infinity);
            infinity_session_map_mutex_.unlock();
            response.success = true;
        }
    }

    void Disconnect(infinity_thrift_rpc::CommonResponse &_return, const infinity_thrift_rpc::CommonRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        if (infinity == nullptr) {
            _return.success = false;
            _return.error_msg = "Disconnect failed";
        } else {
            auto session_id = infinity->GetSessionId();
            infinity->RemoteDisconnect();
            infinity_session_map_mutex_.lock();
            infinity_session_map_.erase(session_id);
            infinity_session_map_mutex_.unlock();
            _return.success = true;
        }
    }

    void CreateDatabase(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::CreateDatabaseRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto result = infinity->CreateDatabase(request.db_name, (const CreateDatabaseOptions &)request.option);
        ProcessCommonResult(response, result);
    }

    void DropDatabase(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::DropDatabaseRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto result = infinity->DropDatabase(request.db_name, (const DropDatabaseOptions &)request.option);
        ProcessCommonResult(response, result);
    }

    void CreateTable(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::CreateTableRequest &request) override {
        Vector<ColumnDef *> column_defs;

        for (auto &proto_column_def : request.column_defs) {
            auto column_def = GetColumnDefFromProto(proto_column_def);
            column_defs.emplace_back(column_def);
        }

        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        CreateTableOptions create_table_opts;
        Assert<NetworkException>(database != nullptr, "Database is null", __FILE_NAME__, __LINE__);

        auto result = database->CreateTable(request.table_name, column_defs, Vector<TableConstraint *>(), create_table_opts);
        ProcessCommonResult(response, result);
    }

    void DropTable(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::DropTableRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        auto result = database->DropTable(request.table_name, (const DropTableOptions &)request.option);

        ProcessCommonResult(response, result);
    }

    void Insert(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::InsertRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        auto table = database->GetTable(request.table_name);
        auto columns = new Vector<String>();
        columns->reserve(request.column_names.size());

        for (auto &column : request.column_names) {
            columns->emplace_back(column);
        }

        auto values = new Vector<Vector<ParsedExpr *> *>();
        values->reserve(request.fields.size());

        for (auto &value : request.fields) {
            auto value_list = new Vector<ParsedExpr *>();
            value_list->reserve(value.parse_exprs.size());
            for (auto &expr : value.parse_exprs) {
                auto parsed_expr = GetConstantFromProto(*expr.type.constant_expr);
                value_list->emplace_back(parsed_expr);
            }
            values->emplace_back(value_list);
        }

        auto result = table->Insert(columns, values);

        ProcessCommonResult(response, result);
    }

    void Import(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::ImportRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        auto table = database->GetTable(request.table_name);
        auto result = table->Import(request.file_path, (const ImportOptions &)request.import_option);

        ProcessCommonResult(response, result);
    }

    void Select(infinity_thrift_rpc::SelectResponse &response, const infinity_thrift_rpc::SelectRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        auto table = database->GetTable(request.table_name);

        Vector<ParsedExpr *> *output_columns;
        output_columns = new Vector<ParsedExpr *>();
        output_columns->reserve(request.select_list.size());

        for (auto &expr : request.select_list) {
            auto parsed_expr = GetColumnExprFromProto(*expr.type.column_expr);
            output_columns->emplace_back(parsed_expr);
        }

        Vector<Pair<ParsedExpr *, ParsedExpr *>> vector_expr{};

        Vector<Pair<ParsedExpr *, ParsedExpr *>> fts_expr{};

        ParsedExpr *filter = GetParsedExprFromProto(request.where_expr);

        // TODO:
        //    ParsedExpr *offset;
        //    offset = new ParsedExpr();
        //    ParsedExpr *limit;
        //    limit = new ConstantExpr (0);
        auto result = table->Search(vector_expr, fts_expr, filter, output_columns, nullptr, nullptr);

        if (result.IsOk()) {
            auto data_block_count = result.result_table_->DataBlockCount();
            auto column_count = result.result_table_->ColumnCount();

            Vector<SharedPtr<ColumnVector>> all_column_vectors;
            all_column_vectors.reserve(column_count);
            SizeT all_row_count = 0;
            for (SizeT block_idx = 0; block_idx < data_block_count; ++block_idx) {
                auto data_block = result.result_table_->GetDataBlockById(block_idx);
                auto row_count = data_block->row_count();
                all_row_count += row_count;

                for (SizeT col_index = 0; col_index < column_count; ++col_index) {
                    auto column_vector = data_block->column_vectors[col_index];

                    if (block_idx == 0) {
                        all_column_vectors.emplace_back(column_vector);
                    } else {
                        all_column_vectors[col_index]->AppendWith(*column_vector.get());
                    }
                }
            }

            for (auto &column_vector : all_column_vectors) {
                auto size = column_vector->data_type()->Size() * all_row_count;
                char *dst = new char[size];
                memcpy(dst, column_vector->data(), size);

                infinity_thrift_rpc::ColumnField columnField;
                columnField.__set_column_vector(dst);
                columnField.__set_column_type(DataTypeToProtoColumnType(column_vector->data_type()));
                response.column_fields.emplace_back(columnField);
            }

            auto table_def = result.result_table_->definition_ptr_;

            for (SizeT col_index = 0; col_index < column_count; ++col_index) {
                auto column_def = table_def->columns()[col_index];
                infinity_thrift_rpc::ColumnDef proto_column_def;
                proto_column_def.__set_id(column_def->id());
                proto_column_def.__set_name(column_def->name());
                infinity_thrift_rpc::DataType proto_data_type;
                proto_column_def.__set_data_type(*DataTypeToProtoDataType(column_def->type()));
                response.column_defs.emplace_back(proto_column_def);
            }
            response.__set_success(true);
        } else {
            response.__set_success(false);
            response.__set_error_msg(result.ErrorStr());
        }
    }

    void ListDatabase(infinity_thrift_rpc::ListDatabaseResponse &response, const infinity_thrift_rpc::ListDatabaseRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto result = infinity->ListDatabases();

        if (result.IsOk()) {
            SharedPtr<DataBlock> data_block = result.result_table_->GetDataBlockById(0);
            auto row_count = data_block->row_count();

            for (int i = 0; i < row_count; ++i) {
                Value value = data_block->GetValue(0, i);
                if (value.value_.varchar.IsInlined()) {
                    String prefix = String(value.value_.varchar.prefix, value.value_.varchar.length);
                    response.db_names.emplace_back(prefix);
                } else {
                    String whole_str = String(value.value_.varchar.ptr, value.value_.varchar.length);
                    response.db_names.emplace_back(whole_str);
                }
            }

            response.__set_success(true);
        } else {
            response.__set_success(false);
            response.__set_error_msg(result.ErrorStr());
        }
    }

    void ListTable(infinity_thrift_rpc::ListTableResponse &response, const infinity_thrift_rpc::ListTableRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        auto result = database->ListTables();
        if (result.IsOk()) {
            SharedPtr<DataBlock> data_block = result.result_table_->GetDataBlockById(0);

            auto row_count = data_block->row_count();
            for (int i = 0; i < row_count; ++i) {
                Value value = data_block->GetValue(0, i);
                if (value.value_.varchar.IsInlined()) {
                    String prefix = String(value.value_.varchar.prefix, value.value_.varchar.length);
                    response.table_names.emplace_back(prefix);
                } else {
                    String whole_str = String(value.value_.varchar.ptr, value.value_.varchar.length);
                    response.table_names.emplace_back(whole_str);
                }
            }

            response.__set_success(true);
        } else {
            response.__set_success(false);
            response.__set_error_msg(result.ErrorStr());
        }
    }

    void DescribeDatabase(infinity_thrift_rpc::DescribeDatabaseResponse &response,
                          const infinity_thrift_rpc::DescribeDatabaseRequest &request) override {
        // Your implementation goes here
        printf("DescribeDatabase\n");
    }

    void DescribeTable(infinity_thrift_rpc::DescribeTableResponse &response, const infinity_thrift_rpc::DescribeTableRequest &request) override {
        // Your implementation goes here
        printf("DescribeTable\n");
    }

    void GetDatabase(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::GetDatabaseRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        if (database != nullptr) {
            response.__set_success(true);
        } else {
            response.__set_success(false);
            response.__set_error_msg("Database not found");
        }
    }

    void GetTable(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::GetTableRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        auto table = database->GetTable(request.table_name);

        if (table != nullptr) {
            response.__set_success(true);
        } else {
            response.__set_success(false);
            response.__set_error_msg("Database not found");
        }
    }

    void CreateIndex(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::CreateIndexRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        auto table = database->GetTable(request.table_name);
        auto column_names = new Vector<String>();

        column_names->reserve(request.column_names.size());
        for (auto &column_name : request.column_names) {
            column_names->emplace_back(column_name);
        }

        String method_type = request.method_type;
        auto *index_para_list = new Vector<InitParameter *>();

        for (auto &index_para : request.index_para_list) {
            auto init_parameter = new InitParameter();
            init_parameter->para_name_ = index_para.para_name;
            init_parameter->para_value_ = index_para.para_value;
            index_para_list->emplace_back(init_parameter);
        }

        auto result = table->CreateIndex(request.index_name, column_names, method_type, index_para_list, (CreateIndexOptions &)request.option);

        ProcessCommonResult(response, result);
    }

    void DropIndex(infinity_thrift_rpc::CommonResponse &response, const infinity_thrift_rpc::DropIndexRequest &request) override {
        auto infinity = GetInfinityBySessionID(request.session_id);
        auto database = infinity->GetDatabase(request.db_name);
        auto table = database->GetTable(request.table_name);
        QueryResult result = table->DropIndex(request.index_name);

        ProcessCommonResult(response, result);
    }

private:
    Mutex infinity_session_map_mutex_{};
    HashMap<u64, SharedPtr<Infinity>> infinity_session_map_{};

private:
    SharedPtr<Infinity> GetInfinityBySessionID(i64 session_id) {
        auto it = infinity_session_map_.find(session_id);
        if (it == infinity_session_map_.end()) {
            Error<NetworkException>("session id not found", __FILE_NAME__, __LINE__);
        }
        return it->second;
    }

    static void ProcessCommonResult(infinity_thrift_rpc::CommonResponse &response, const QueryResult &result) {
        if (result.IsOk()) {
            response.__set_success(true);
        } else {
            response.__set_success(false);
            response.__set_error_msg(result.ErrorStr());
        }
    }

    static ColumnDef *GetColumnDefFromProto(const infinity_thrift_rpc::ColumnDef &column_def) {
        auto column_def_data_type_ptr = GetColumnTypeFromProto(column_def.data_type);
        auto constraints = HashSet<ConstraintType>();

        for (auto constraint : column_def.constraints) {
            auto type = GetConstraintTypeFromProto(constraint);
            constraints.insert(type);
        }

        const auto &column_def_name = column_def.name;
        auto col_def = new ColumnDef(column_def.id, column_def_data_type_ptr, column_def_name, constraints);
        return col_def;
    }

    static SharedPtr<DataType> GetColumnTypeFromProto(const infinity_thrift_rpc::DataType &type) {
        switch (type.logic_type) {
            case infinity_thrift_rpc::LogicType::Boolean:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kBoolean);
            case infinity_thrift_rpc::LogicType::TinyInt:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kTinyInt);
            case infinity_thrift_rpc::LogicType::SmallInt:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kSmallInt);
            case infinity_thrift_rpc::LogicType::Integer:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kInteger);
            case infinity_thrift_rpc::LogicType::BigInt:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kBigInt);
            case infinity_thrift_rpc::LogicType::HugeInt:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kHugeInt);
            case infinity_thrift_rpc::LogicType::Decimal:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kDecimal);
            case infinity_thrift_rpc::LogicType::Float:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kFloat);
            case infinity_thrift_rpc::LogicType::Double:
                return MakeShared<infinity::DataType>(infinity::LogicalType::kDouble);
            case infinity_thrift_rpc::LogicType::Embedding: {
                auto embedding_type = GetEmbeddingDataTypeFromProto(type.physical_type.embedding_type.element_type);
                auto embedding_info = EmbeddingInfo::Make(embedding_type, type.physical_type.embedding_type.dimension);
                return MakeShared<infinity::DataType>(infinity::LogicalType::kEmbedding, embedding_info);
            };
            default:
                Error<TypeException>("Invalid data type", __FILE_NAME__, __LINE__);
        }
    }

    static ConstraintType GetConstraintTypeFromProto(infinity_thrift_rpc::Constraint::type constraint) {
        switch (constraint) {
            case infinity_thrift_rpc::Constraint::NotNull:
                return ConstraintType::kNotNull;
            case infinity_thrift_rpc::Constraint::Null:
                return ConstraintType::kNull;
            case infinity_thrift_rpc::Constraint::PrimaryKey:
                return ConstraintType::kPrimaryKey;
            case infinity_thrift_rpc::Constraint::Unique:
                return ConstraintType::kUnique;
            default:
                Error<TypeException>("Invalid constraint type", __FILE_NAME__, __LINE__);
        }
    }

    static EmbeddingDataType GetEmbeddingDataTypeFromProto(const infinity_thrift_rpc::ElementType::type &type) {
        switch (type) {
            case infinity_thrift_rpc::ElementType::ElementBit:
                return EmbeddingDataType::kElemBit;
            case infinity_thrift_rpc::ElementType::ElementInt8:
                return EmbeddingDataType::kElemInt8;
            case infinity_thrift_rpc::ElementType::ElementInt16:
                return EmbeddingDataType::kElemInt16;
            case infinity_thrift_rpc::ElementType::ElementInt32:
                return EmbeddingDataType::kElemInt32;
            case infinity_thrift_rpc::ElementType::ElementInt64:
                return EmbeddingDataType::kElemInt64;
            case infinity_thrift_rpc::ElementType::ElementFloat32:
                return EmbeddingDataType::kElemFloat;
            case infinity_thrift_rpc::ElementType::ElementFloat64:
                return EmbeddingDataType::kElemDouble;
            default:
                Error<TypeException>("Invalid embedding data type", __FILE_NAME__, __LINE__);
        }
    }

    static ConstantExpr *GetConstantFromProto(const infinity_thrift_rpc::ConstantExpr &expr) {
        switch (expr.literal_type) {
            case infinity_thrift_rpc::LiteralType::Boolean: {
                auto parsed_expr = new ConstantExpr(LiteralType::kBoolean);
                parsed_expr->bool_value_ = expr.bool_value;
                return parsed_expr;
            }
            case infinity_thrift_rpc::LiteralType::Double: {
                auto parsed_expr = new ConstantExpr(LiteralType::kDouble);
                parsed_expr->double_value_ = expr.f64_value;
                return parsed_expr;
            }
            case infinity_thrift_rpc::LiteralType::String: {
                auto parsed_expr = new ConstantExpr(LiteralType::kString);
                parsed_expr->str_value_ = strdup(expr.str_value.c_str());
                return parsed_expr;
            }
            case infinity_thrift_rpc::LiteralType::Int64: {
                auto parsed_expr = new ConstantExpr(LiteralType::kInteger);
                parsed_expr->integer_value_ = expr.i64_value;
                return parsed_expr;
            }
            case infinity_thrift_rpc::LiteralType::Null: {
                auto parsed_expr = new ConstantExpr(LiteralType::kNull);
                return parsed_expr;
            }
            case infinity_thrift_rpc::LiteralType::IntegerArray: {
                auto parsed_expr = new ConstantExpr(LiteralType::kIntegerArray);
                parsed_expr->long_array_.reserve(expr.i64_array_value.size());
                for (auto &value : expr.i64_array_value) {
                    parsed_expr->long_array_.emplace_back(value);
                }
            }
            case infinity_thrift_rpc::LiteralType::DoubleArray: {
                auto parsed_expr = new ConstantExpr(LiteralType::kDoubleArray);
                parsed_expr->double_array_.reserve(expr.f64_array_value.size());
                for (auto &value : expr.f64_array_value) {
                    parsed_expr->double_array_.emplace_back(value);
                }
            }
            default:
                Error<TypeException>("Invalid constant type", __FILE_NAME__, __LINE__);
        }
    }

    static ColumnExpr *GetColumnExprFromProto(const infinity_thrift_rpc::ColumnExpr &column_expr) {
        auto parsed_expr = new ColumnExpr();

        for (auto &column_name : column_expr.column_name) {
            parsed_expr->names_.emplace_back(column_name);
        }

        parsed_expr->star_ = column_expr.star;
        return parsed_expr;
    }

    static FunctionExpr *GetFunctionExprFromProto(const infinity_thrift_rpc::FunctionExpr &function_expr) {
        auto *parsed_expr = new FunctionExpr();
        parsed_expr->func_name_ = function_expr.function_name;
        Vector<ParsedExpr *> *arguments;
        arguments = new Vector<ParsedExpr *>();
        arguments->reserve(function_expr.arguments.size());

        for (auto &args : function_expr.arguments) {
            arguments->emplace_back(GetParsedExprFromProto(args));
        }

        parsed_expr->arguments_ = arguments;
        return parsed_expr;
    }

    static ParsedExpr *GetParsedExprFromProto(const infinity_thrift_rpc::ParsedExpr &expr) {
        if (expr.type.column_expr != nullptr) {
            auto parsed_expr = GetColumnExprFromProto(*expr.type.column_expr);
            return parsed_expr;
        } else if (expr.type.constant_expr != nullptr) {
            auto parsed_expr = GetConstantFromProto(*expr.type.constant_expr);
            return parsed_expr;
        } else if (expr.type.function_expr != nullptr) {
            auto parsed_expr = GetFunctionExprFromProto(*expr.type.function_expr);
            return parsed_expr;
        }
    }

    static infinity_thrift_rpc::ColumnType::type DataTypeToProtoColumnType(const SharedPtr<DataType> &data_type) {
        switch (data_type->type()) {
            case LogicalType::kBoolean:
                return infinity_thrift_rpc::ColumnType::ColumnBool;
            case LogicalType::kTinyInt:
                return infinity_thrift_rpc::ColumnType::ColumnInt8;
            case LogicalType::kSmallInt:
                return infinity_thrift_rpc::ColumnType::ColumnInt16;
            case LogicalType::kInteger:
                return infinity_thrift_rpc::ColumnType::ColumnInt32;
            case LogicalType::kBigInt:
                return infinity_thrift_rpc::ColumnType::ColumnInt64;
            case LogicalType::kFloat:
                return infinity_thrift_rpc::ColumnType::ColumnFloat32;
            case LogicalType::kDouble:
                return infinity_thrift_rpc::ColumnType::ColumnFloat64;
            case LogicalType::kVarchar:
                return infinity_thrift_rpc::ColumnType::ColumnVarchar;
            case LogicalType::kEmbedding:
                return infinity_thrift_rpc::ColumnType::ColumnEmbedding;
            default:
                Error<TypeException>("Invalid data type", __FILE_NAME__, __LINE__);
        }
    }

    infinity_thrift_rpc::DataType *DataTypeToProtoDataType(const SharedPtr<DataType> &data_type) {
        switch (data_type->type()) {
            case LogicalType::kBoolean: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::Boolean);
                return data_type_proto;
            }
            case LogicalType::kTinyInt: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::TinyInt);
                return data_type_proto;
            }
            case LogicalType::kSmallInt: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::SmallInt);
                return data_type_proto;
            }
            case LogicalType::kInteger: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::Integer);
                return data_type_proto;
            }
            case LogicalType::kBigInt: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::BigInt);
                return data_type_proto;
            }
            case LogicalType::kFloat: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::Float);
                return data_type_proto;
            }
            case LogicalType::kDouble: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::Double);
                return data_type_proto;
            }
            case LogicalType::kVarchar: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                infinity_thrift_rpc::VarcharType varchar_type;
                auto varchar_info = static_cast<VarcharInfo *>(data_type->type_info().get());
                varchar_type.__set_dimension(varchar_info->dimension());
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::Varchar);
                infinity_thrift_rpc::PhysicalType physical_type;
                physical_type.__set_varchar_type(varchar_type);
                data_type_proto->__set_physical_type(physical_type);
                return data_type_proto;
            }
            case LogicalType::kEmbedding: {
                auto *data_type_proto = new infinity_thrift_rpc::DataType();
                infinity_thrift_rpc::EmbeddingType embedding_type;
                auto embedding_info = static_cast<EmbeddingInfo *>(data_type->type_info().get());
                embedding_type.__set_dimension(embedding_info->Dimension());
                data_type_proto->__set_logic_type(infinity_thrift_rpc::LogicType::Embedding);
                infinity_thrift_rpc::PhysicalType physical_type;
                physical_type.__set_embedding_type(embedding_type);
                data_type_proto->__set_physical_type(physical_type);
                return data_type_proto;
            }
            default:
                Error<TypeException>("Invalid data type", __FILE_NAME__, __LINE__);
        }
    }
};

class InfinityServiceCloneFactory : virtual public infinity_thrift_rpc::InfinityServiceIfFactory {
public:
    ~InfinityServiceCloneFactory() override = default;
    InfinityServiceIf *getHandler(const ::apache::thrift::TConnectionInfo &connInfo) override {
        SharedPtr<TSocket> sock = std::dynamic_pointer_cast<TSocket>(connInfo.transport);
        LOG_TRACE(Format("Incoming connection, SocketInfo: {}, PeerHost: {}, PeerAddress: {}, PeerPort: {}",
                         sock->getSocketInfo(),
                         sock->getPeerHost(),
                         sock->getPeerAddress(),
                         sock->getPeerPort()));
        return new InfinityServiceHandler;
    }
    void releaseHandler(infinity_thrift_rpc::InfinityServiceIf *handler) override { delete handler; }
};

// Thrift server

void ThreadedThriftServer::Init(i32 port_no) {

    server = MakeUnique<TThreadedServer>(MakeShared<InfinityServiceProcessorFactory>(MakeShared<InfinityServiceCloneFactory>()),
                                         MakeShared<TServerSocket>(port_no), // port
                                         MakeShared<TBufferedTransportFactory>(),
                                         MakeShared<TBinaryProtocolFactory>());
}

void ThreadedThriftServer::Start() { server->serve(); }

void ThreadedThriftServer::Shutdown() { server->stop(); }

void PoolThriftServer::Init(i32 port_no, i32 pool_size) {

    SharedPtr<ThreadFactory> threadFactory = MakeShared<ThreadFactory>();

    SharedPtr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(pool_size);
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    server = MakeUnique<TThreadPoolServer>(MakeShared<InfinityServiceProcessorFactory>(MakeShared<InfinityServiceCloneFactory>()),
                                           MakeShared<TServerSocket>(port_no),
                                           MakeShared<TBufferedTransportFactory>(),
                                           MakeShared<TBinaryProtocolFactory>(),
                                           threadManager);
}

void PoolThriftServer::Start() { server->serve(); }

void PoolThriftServer::Shutdown() { server->stop(); }

} // namespace infinity