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
        : Statement(location), condition(CastExpression::castImplicit(condition, TypeBoolean::getInstance())), body(body)
    {
    }
public:
    static std::shared_ptr<WhileStatement> make(Location location, std::shared_ptr<Expression> condition, std::shared_ptr<CodeBlock> body)
    {
        return std::shared_ptr<WhileStatement>(new WhileStatement(location, condition, body));
    }
};
}

#endif // WHILE_STATEMENT_H_INCLUDED
