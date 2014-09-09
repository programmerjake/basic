#ifndef COMPARE_EXPRESSION_H_INCLUDED
#define COMPARE_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "ast/type_builtin.h"
#include "location.h"
#include <memory>
#include <functional>
#include <cwchar>
#include <string>
#include <initializer_list>

namespace AST
{
class CompareExpression final : public Expression
{
public:
    enum class CType
    {
        GT,
        LT,
        GE,
        LE,
        NE,
        Eq
    };
    static std::wstring getOperatorString(CType ctype)
    {
        switch(ctype)
        {
        case CType::GT:
            return L">";
        case CType::LT:
            return L"<";
        case CType::GE:
            return L">=";
        case CType::LE:
            return L"<=";
        case CType::NE:
            return L"!=";
        case CType::Eq:
            return L"=";
        }
        return L"<unknown compare operator>";
    }
    static bool isOrdered(CType ctype)
    {
        switch(ctype)
        {
        case CType::GT:
        case CType::LT:
        case CType::GE:
        case CType::LE:
            return true;
        default:
            return false;
        }
    }
private:
    CType ctype_;
    void checkTypes(); // in expressions.cpp
    CompareExpression(Location location, std::shared_ptr<Expression> arg1, CType ctype, std::shared_ptr<Expression> arg2)
        : Expression(location, TypeBoolean::getInstance()), ctype_(ctype)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1, arg2};
        checkTypes();
    }
    void ctype(CType v)
    {
        ctype_ = v;
    }
public:
    CType ctype() const
    {
        return ctype_;
    }
    static std::shared_ptr<CompareExpression> make(Location location, std::shared_ptr<Expression> arg1, CType ctype, std::shared_ptr<Expression> arg2)
    {
        return std::shared_ptr<CompareExpression>(new CompareExpression(location, arg1, ctype, arg2));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // COMPARE_EXPRESSION_H_INCLUDED
