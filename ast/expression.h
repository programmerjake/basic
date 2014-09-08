#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

#include "ast/statement.h"
#include "location.h"
#include "ast/type.h"
#include <memory>
#include <vector>

namespace AST
{
class Expression : public Statement
{
    std::shared_ptr<const Type> type_;
    std::vector<std::shared_ptr<Expression>> args_;
protected:
    Expression(const Expression &rt)
        : Statement(rt), type_(rt.type_), args_(rt.args_)
    {
    }
    std::vector<std::shared_ptr<Expression>> &argsRef()
    {
        return args_;
    }
    void type(std::shared_ptr<const Type> v)
    {
        type_ = v;
    }
public:
    const std::shared_ptr<const Type> &type() const
    {
        return type_;
    }
    const std::vector<std::shared_ptr<Expression>> &args() const
    {
        return args_;
    }
    explicit Expression(Location location, std::shared_ptr<const Type> type)
        : Statement(location), type_(type)
    {
    }
};
}

#endif // EXPRESSION_H_INCLUDED
