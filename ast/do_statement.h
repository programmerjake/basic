#ifndef DO_STATEMENT_H_INCLUDED
#define DO_STATEMENT_H_INCLUDED

#include "ast/expression.h"
#include "ast/statement.h"
#include "ast/code_block.h"
#include "ast/cast_expression.h"
#include "ast/type_builtin.h"
#include <memory>

namespace AST
{
class DoStatement final : public Statement
{
public:
    std::shared_ptr<Expression> condition;
    enum class ConditionType
    {
        None,
        DoWhile,
        DoUntil,
        LoopWhile,
        LoopUntil
    };
    ConditionType conditionType;
    std::shared_ptr<CodeBlock> body;
private:
    DoStatement(Location location, std::shared_ptr<Expression> condition, ConditionType conditionType, std::shared_ptr<CodeBlock> body)
        : Statement(location), condition(condition ? CastExpression::castImplicit(condition, TypeBoolean::getInstance()) : nullptr), conditionType(conditionType), body(body)
    {
    }
public:
    static std::shared_ptr<DoStatement> make(Location location, std::shared_ptr<Expression> condition, ConditionType conditionType, std::shared_ptr<CodeBlock> body)
    {
        return std::shared_ptr<DoStatement>(new DoStatement(location, condition, conditionType, body));
    }
};
}

#endif // DO_STATEMENT_H_INCLUDED
