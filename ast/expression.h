#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

#include "base.h"
#include "type.h"
#include <memory>

namespace AST
{
class Expression : public Base
{
    std::shared_ptr<Type> type;
protected:
    Expression(const Expression &rt)
        : Base(rt), type(rt.type)
    {
    }
public:
    explicit Expression(std::shared_ptr<Type> type)
        : type(type)
    {
    }
};
}

#endif // EXPRESSION_H_INCLUDED
