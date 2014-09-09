#ifndef NEG_EXPRESSION_H_INCLUDED
#define NEG_EXPRESSION_H_INCLUDED

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
class NegExpression final : public Expression
{
    void calcType(); // in expressions.cpp
    NegExpression(Location location, std::shared_ptr<Expression> arg)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg};
        calcType();
    }
public:
    static std::shared_ptr<NegExpression> make(Location location, std::shared_ptr<Expression> arg)
    {
        return std::shared_ptr<NegExpression>(new NegExpression(location, arg));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // NEG_EXPRESSION_H_INCLUDED
