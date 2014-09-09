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
    void calcType(); // in expressions.cpp
    OrExpression(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1, arg2};
        calcType();
    }
public:
    static std::shared_ptr<OrExpression> make(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
    {
        return std::shared_ptr<OrExpression>(new OrExpression(location, arg1, arg2));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // OR_EXPRESSION_H_INCLUDED