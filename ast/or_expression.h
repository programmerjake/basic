#ifndef OR_EXPRESSION_H_INCLUDED
#define OR_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include <memory>
#include <functional>
#include <cwchar>
#include <string>
#include <initializer_list>

namespace AST
{
class OrExpression final : public Expression
{
    void calcType(std::function<void(const std::wstring &)> handleError); // in expressions.cpp
    OrExpression(std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2, std::function<void(const std::wstring &)> handleError)
        : Expression(nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1, arg2};
        calcType(handleError);
    }
public:
    static std::shared_ptr<OrExpression> make(std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2, std::function<void(const std::wstring &)> handleError)
    {
        return std::shared_ptr<OrExpression>(new OrExpression(arg1, arg2, handleError));
    }
    virtual std::shared_ptr<Base> dup() const override
    {
        return std::shared_ptr<OrExpression>(new OrExpression(*this));
    }
};
}

#endif // OR_EXPRESSION_H_INCLUDED