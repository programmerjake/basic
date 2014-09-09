#ifndef UNARY_PLUS_EXPRESSION_H_INCLUDED
#define UNARY_PLUS_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "location.h"
#include <memory>
#include <functional>
#include <cwchar>
#include <string>
#include <initializer_list>

namespace AST
{
class UnaryPlusExpression final : public Expression
{
    void calcType(); // in expressions.cpp
    UnaryPlusExpression(Location location, std::shared_ptr<Expression> arg)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg};
        calcType();
    }
public:
    static std::shared_ptr<UnaryPlusExpression> make(Location location, std::shared_ptr<Expression> arg)
    {
        return std::shared_ptr<UnaryPlusExpression>(new UnaryPlusExpression(location, arg));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
    #warning finish
};
}

#endif // UNARY_PLUS_EXPRESSION_H_INCLUDED
