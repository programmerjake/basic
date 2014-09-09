#ifndef ASSIGN_STATEMENT_H_INCLUDED
#define ASSIGN_STATEMENT_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "ast/cast_expression.h"
#include "ast/statement.h"
#include <memory>
#include <cwchar>
#include <string>
#include "error.h"

namespace AST
{
class AssignStatement final : public Statement
{
public:
    std::shared_ptr<Expression> lhs, rhs;
private:
    AssignStatement(Location location, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : Statement(location), lhs(lhs), rhs(rhs)
    {
        if(!lhs->type()->isLValue())
            throw ParserError(location, L"can not assign to a rvalue");
        rhs = CastExpression::castImplicit(rhs, lhs->type()->toRValue());
    }
public:
    static std::shared_ptr<AssignStatement> make(Location location, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
    {
        return std::shared_ptr<AssignStatement>(new AssignStatement(location, lhs, rhs));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};
}

#endif // ASSIGN_STATEMENT_H_INCLUDED
