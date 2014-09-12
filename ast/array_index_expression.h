#ifndef ARRAY_INDEX_EXPRESSION_H_INCLUDED
#define ARRAY_INDEX_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "location.h"
#include <memory>
#include <initializer_list>

namespace AST
{
class ArrayIndexExpression final : public Expression
{
    void calcType(); // in expressions.cpp
    ArrayIndexExpression(Location location, std::shared_ptr<Expression> array, const std::vector<std::shared_ptr<Expression>> &indexes)
        : Expression(location, nullptr)
    {
        argsRef().reserve(indexes.size() + 1);
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{array};
        argsRef().insert(argsRef().end(), indexes.begin(), indexes.end());
        calcType();
    }
public:
    static std::shared_ptr<ArrayIndexExpression> make(Location location, std::shared_ptr<Expression> array, const std::vector<std::shared_ptr<Expression>> &indexes)
    {
        return std::shared_ptr<ArrayIndexExpression>(new ArrayIndexExpression(location, array, indexes));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // ARRAY_INDEX_EXPRESSION_H_INCLUDED
