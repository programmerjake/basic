#ifndef AND_EXPRESSION_H_INCLUDED
#define AND_EXPRESSION_H_INCLUDED

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
class AndExpression final : public Expression
{
    void calcType(); // in expressions.cpp
    AndExpression(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1, arg2};
        calcType();
    }
public:
    static std::shared_ptr<AndExpression> make(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
    {
        return std::shared_ptr<AndExpression>(new AndExpression(location, arg1, arg2));
    }
};
}

#endif // AND_EXPRESSION_H_INCLUDED
