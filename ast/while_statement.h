#ifndef WHILE_STATEMENT_H_INCLUDED
#define WHILE_STATEMENT_H_INCLUDED

#include "ast/expression.h"
#include "ast/statement.h"
#include "ast/code_block.h"
#include "ast/cast_expression.h"
#include "ast/type_builtin.h"
#include <memory>

namespace AST
{
class WhileStatement final : public Statement
{
public:
    std::shared_ptr<Expression> condition;
    std::shared_ptr<CodeBlock> body;
private:
    WhileStatement(Location location, std::shared_ptr<Expression> condition, std::shared_ptr<CodeBlock> body)
        : Statement(location), condition(condition ? CastExpression::castImplicit(condition, TypeBoolean::getInstance()) : nullptr), body(body)
    {
    }
public:
    static std::shared_ptr<WhileStatement> make(Location location, std::shared_ptr<Expression> condition, std::shared_ptr<CodeBlock> body)
    {
        return std::shared_ptr<WhileStatement>(new WhileStatement(location, condition, body));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
    void setCondition(std::shared_ptr<Expression> condition)
    {
        this->condition = CastExpression::castImplicit(condition, TypeBoolean::getInstance());
    }
};
}

#endif // WHILE_STATEMENT_H_INCLUDED
