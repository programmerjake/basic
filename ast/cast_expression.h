#ifndef CAST_EXPRESSION_H_INCLUDED
#define CAST_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include <memory>
#include <functional>
#include <cwchar>
#include <string>

namespace AST
{
class CastExpression final : public Expression
{
private:
    bool isImplicit_;
    CastExpression(std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType, bool isImplicit, std::function<void(const std::wstring &)> handleError)
        : Expression(newType), isImplicit_(isImplicit)
    {
        if(!expression->type()->canCastTo(newType, isImplicit))
            handleError((isImplicit ? L"can't implicitly cast " : L"can't cast ") + expression->type()->toString() + L" to " + newType->toString());
        argsRef().assign(1, expression);
    }
public:
    bool isImplicit() const
    {
        return isImplicit_;
    }
    static std::shared_ptr<CastExpression> make(std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType, bool isImplicit, std::function<void(const std::wstring &)> handleError)
    {
        return std::shared_ptr<CastExpression>(new CastExpression(expression, newType, isImplicit, handleError));
    }
    static std::shared_ptr<Expression> castImplicit(std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType, std::function<void(const std::wstring &)> handleError)
    {
        if(*expression->type() == *newType)
            return expression;
        return make(expression, newType, true, handleError);
    }
    static std::shared_ptr<Expression> castImplicitNotChecked(std::shared_ptr<Expression> expression, std::shared_ptr<const Type> newType)
    {
        return castImplicit(expression, newType, [](const std::wstring &){});
    }
    virtual std::shared_ptr<Base> dup() const override
    {
        return make(std::static_pointer_cast<Expression>(args()[0]->dup()), type(), isImplicit(), [](const std::wstring &){});
    }
};
}

#endif // CAST_EXPRESSION_H_INCLUDED
