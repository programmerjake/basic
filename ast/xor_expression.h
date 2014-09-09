#ifndef XOR_EXPRESSION_H_INCLUDED
#define XOR_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include <memory>
#include <functional>
#include <cwchar>
#include <string>
#include <initializer_list>

namespace AST
{
class XorExpression final : public Expression
{
    void calcType(); // in expressions.cpp
    XorExpression(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1, arg2};
        calcType();
    }
public:
    static std::shared_ptr<XorExpression> make(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
    {
        return std::shared_ptr<XorExpression>(new XorExpression(location, arg1, arg2));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // XOR_EXPRESSION_H_INCLUDED
