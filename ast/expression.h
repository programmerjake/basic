#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

#include "base.h"
#include "type.h"
#include <memory>
#include <vector>

namespace AST
{
class Expression : public Base
{
    std::shared_ptr<const Type> type_;
    std::vector<std::shared_ptr<Expression>> args_;
protected:
    Expression(const Expression &rt)
        : Base(rt), type_(rt.type_), args_(rt.args_)
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
    explicit Expression(std::shared_ptr<const Type> type)
        : type_(type)
    {
    }
};
}

#endif // EXPRESSION_H_INCLUDED
