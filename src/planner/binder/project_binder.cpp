//
// Created by JinHai on 2022/8/12.
//

#include "common/utility/infinity_assert.h"
#include "function/function_set.h"
#include "project_binder.h"

namespace infinity {

std::shared_ptr<BaseExpression>
ProjectBinder::BuildExpression(const hsql::Expr &expr, const std::shared_ptr<BindContext> &bind_context_ptr) {
    std::string expr_name = expr.getName();

    // If the expr is coming from group by lists.
    if (bind_context_ptr->group_by_name_.contains(expr_name)) {
        auto group_by_expr_ptr = bind_context_ptr->group_by_name_[expr_name];

        group_by_expr_ptr->source_position_
                = SourcePosition(bind_context_ptr->binding_context_id_, ExprSourceType::kGroupBy);

        return group_by_expr_ptr;
    }

    // If the expr is coming from aggregate function list
    if(bind_context_ptr->aggregate_by_name_.contains(expr_name)) {
        auto agg_expr_ptr = bind_context_ptr->aggregate_by_name_[expr_name];

        agg_expr_ptr->source_position_
                = SourcePosition(bind_context_ptr->binding_context_id_, ExprSourceType::kAggregate);

        return agg_expr_ptr;
    }

    return ExpressionBinder::BuildExpression(expr, bind_context_ptr);
}

std::shared_ptr<BaseExpression>
ProjectBinder::BuildFuncExpr(const hsql::Expr &expr, const std::shared_ptr<BindContext>& bind_context_ptr) {

    std::shared_ptr<FunctionSet> function_set_ptr = FunctionSet::GetFunctionSet(expr);
    if(function_set_ptr->type_ == FunctionType::kAggregate) {
        if(this->binding_agg_func_) {
            PlannerError("Aggregate function is called in another aggregate function.");
        } else {
            this->binding_agg_func_ = true;
        }
    }
    auto func_expr_ptr = ExpressionBinder::BuildFuncExpr(expr, bind_context_ptr);

    if(function_set_ptr->type_ == FunctionType::kAggregate) {
        std::string expr_name = expr.getName();
        bind_context_ptr->aggregate_by_name_[expr_name] = func_expr_ptr;
        func_expr_ptr->source_position_
            = SourcePosition(bind_context_ptr->binding_context_id_, ExprSourceType::kAggregate);
        this->binding_agg_func_ = false;
    }

    return func_expr_ptr;
}

}