#ifndef CAST_EXPRESSION_H_INCLUDED
#define CAST_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "error.h"
#include <memory>
#include <functional>
#include <cwchar>
#include <string>

namespace AST
{
class CastExpression final : public Expression
{
public:
    static void checkCast(Location location, std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType, bool isImplicit)
    {
        if(!expression->type()->canCastTo(newType, isImplicit))
            throw ParserError(location, (isImplicit ? L"can't implicitly cast " : L"can't cast ") + expression->type()->toString() + L" to " + newType->toString());
    }
private:
    bool isImplicit_;
    CastExpression(Location location, std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType, bool isImplicit, bool checkForErrors)
        : Expression(location, newType), isImplicit_(isImplicit)
    {
        if(checkForErrors)
            checkCast(location, expression, newType, isImplicit);
        argsRef().assign(1, expression);
    }
public:
    bool isImplicit() const
    {
        return isImplicit_;
    }
    static std::shared_ptr<CastExpression> make(Location location, std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType, bool isImplicit, bool checkForErrors = true)
    {
        return std::shared_ptr<CastExpression>(new CastExpression(location, expression, newType, isImplicit, checkForErrors));
    }
    static std::shared_ptr<Expression> castImplicit(std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType)
    {
        if(*expression->type() == *newType)
            return expression;
        return make(expression->location(), expression, newType, true);
    }
    static std::shared_ptr<Expression> castImplicitNotChecked(std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType)
    {
        if(*expression->type() == *newType)
            return expression;
        return make(expression->location(), expression, newType, false);
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // CAST_EXPRESSION_H_INCLUDED
