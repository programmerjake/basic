#ifndef IDIV_EXPRESSION_H_INCLUDED
#define IDIV_EXPRESSION_H_INCLUDED

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
class IDivExpression final : public Expression
{
    void calcType(); // in expressions.cpp
    IDivExpression(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1, arg2};
        calcType();
    }
public:
    static std::shared_ptr<IDivExpression> make(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
    {
        return std::shared_ptr<IDivExpression>(new IDivExpression(location, arg1, arg2));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
    #warning finish
};
}

#endif // IDIV_EXPRESSION_H_INCLUDED
