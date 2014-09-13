#ifndef CALL_EXPRESSION_H_INCLUDED
#define CALL_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "location.h"
#include <memory>
#include <initializer_list>

namespace AST
{
class CallExpression final : public Expression
{
    void calcType(); // in expressions.cpp
    CallExpression(Location location, std::shared_ptr<Expression> procedure, const std::vector<std::shared_ptr<Expression>> &args)
        : Expression(location, nullptr)
    {
        argsRef().reserve(args.size() + 1);
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{procedure};
        argsRef().insert(argsRef().end(), args.begin(), args.end());
        calcType();
    }
public:
    static std::shared_ptr<CallExpression> make(Location location, std::shared_ptr<Expression> procedure, const std::vector<std::shared_ptr<Expression>> &args)
    {
        return std::shared_ptr<CallExpression>(new CallExpression(location, procedure, args));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // CALL_EXPRESSION_H_INCLUDED
