#ifndef BUILTIN_FUNCTION_H_INCLUDED
#define BUILTIN_FUNCTION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "location.h"
#include <memory>
#include <functional>
#include <cwchar>
#include <string>
#include <initializer_list>
#include <cassert>

namespace AST
{
class BuiltInFunctionExpression final : public Expression
{
public:
    enum class FnType
    {
        Abs,
        Asc,
        ATn1,
        ATn2,
        Chr,
        Cos,
        Exp,
        Hex,
        InStr2,
        InStr3,
        LBound1,
        LBound2,
        LCase,
        Left,
        Mid2,
        Mid3,
        Oct,
        Right,
        RTrim,
        Sgn,
        Sin,
        Space,
        Sqr,
        String,
        Tan,
        UBound1,
        UBound2,
        UCase,
        Val,
    };
    static size_t fnArgCount(FnType fnType)
    {
        switch(fnType)
        {
        case FnType::Abs:
            return 1;
        case FnType::Asc:
            return 1;
        case FnType::ATn1:
            return 1;
        case FnType::ATn2:
            return 2;
        case FnType::Chr:
            return 1;
        case FnType::Cos:
            return 1;
        case FnType::Exp:
            return 1;
        case FnType::Hex:
            return 1;
        case FnType::InStr2:
            return 2;
        case FnType::InStr3:
            return 3;
        case FnType::LBound1:
            return 1;
        case FnType::LBound2:
            return 2;
        case FnType::LCase:
            return 1;
        case FnType::Left:
            return 2;
        case FnType::Mid2:
            return 2;
        case FnType::Mid3:
            return 3;
        case FnType::Oct:
            return 1;
        case FnType::Right:
            return 2;
        case FnType::RTrim:
            return 1;
        case FnType::Sgn:
            return 1;
        case FnType::Sin:
            return 1;
        case FnType::Space:
            return 1;
        case FnType::Sqr:
            return 1;
        case FnType::String:
            return 2;
        case FnType::Tan:
            return 1;
        case FnType::UBound1:
            return 1;
        case FnType::UBound2:
            return 2;
        case FnType::UCase:
            return 1;
        case FnType::Val:
            return 1;
        }
        assert(false);
        return 0;
    }
    static std::wstring fnName(FnType fnType)
    {
        switch(fnType)
        {
        case FnType::Abs:
            return L"Abs";
        case FnType::Asc:
            return L"Asc";
        case FnType::ATn1:
            return L"ATn1";
        case FnType::ATn2:
            return L"ATn2";
        case FnType::Chr:
            return L"Chr";
        case FnType::Cos:
            return L"Cos";
        case FnType::Exp:
            return L"Exp";
        case FnType::Hex:
            return L"Hex";
        case FnType::InStr2:
            return L"InStr2";
        case FnType::InStr3:
            return L"InStr3";
        case FnType::LBound1:
            return L"LBound1";
        case FnType::LBound2:
            return L"LBound2";
        case FnType::LCase:
            return L"LCase";
        case FnType::Left:
            return L"Left";
        case FnType::Mid2:
            return L"Mid2";
        case FnType::Mid3:
            return L"Mid3";
        case FnType::Oct:
            return L"Oct";
        case FnType::Right:
            return L"Right";
        case FnType::RTrim:
            return L"RTrim";
        case FnType::Sgn:
            return L"Sgn";
        case FnType::Sin:
            return L"Sin";
        case FnType::Space:
            return L"Space";
        case FnType::Sqr:
            return L"Sqr";
        case FnType::String:
            return L"String";
        case FnType::Tan:
            return L"Tan";
        case FnType::UBound1:
            return L"UBound1";
        case FnType::UBound2:
            return L"UBound2";
        case FnType::UCase:
            return L"UCase";
        case FnType::Val:
            return L"Val";
        }
        assert(false);
        return L"";
    }
    static std::wstring fnSourceName(FnType fnType)
    {
        switch(fnType)
        {
        case FnType::Abs:
            return L"Abs";
        case FnType::Asc:
            return L"Asc";
        case FnType::ATn1:
            return L"ATn";
        case FnType::ATn2:
            return L"ATn";
        case FnType::Chr:
            return L"Chr";
        case FnType::Cos:
            return L"Cos";
        case FnType::Exp:
            return L"Exp";
        case FnType::Hex:
            return L"Hex";
        case FnType::InStr2:
            return L"InStr";
        case FnType::InStr3:
            return L"InStr";
        case FnType::LBound1:
            return L"LBound";
        case FnType::LBound2:
            return L"LBound";
        case FnType::LCase:
            return L"LCase";
        case FnType::Left:
            return L"Left";
        case FnType::Mid2:
            return L"Mid";
        case FnType::Mid3:
            return L"Mid";
        case FnType::Oct:
            return L"Oct";
        case FnType::Right:
            return L"Right";
        case FnType::RTrim:
            return L"RTrim";
        case FnType::Sgn:
            return L"Sgn";
        case FnType::Sin:
            return L"Sin";
        case FnType::Space:
            return L"Space";
        case FnType::Sqr:
            return L"Sqr";
        case FnType::String:
            return L"String";
        case FnType::Tan:
            return L"Tan";
        case FnType::UBound1:
            return L"UBound";
        case FnType::UBound2:
            return L"UBound";
        case FnType::UCase:
            return L"UCase";
        case FnType::Val:
            return L"Val";
        }
        assert(false);
        return L"";
    }
private:
    FnType fnType_;
    void calcType(); // in expressions.cpp
    BuiltInFunctionExpression(Location location, std::shared_ptr<Expression> arg1)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1};
        calcType();
    }
    BuiltInFunctionExpression(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1, arg2};
        calcType();
    }
    BuiltInFunctionExpression(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2, std::shared_ptr<Expression> arg3)
        : Expression(location, nullptr)
    {
        argsRef() = std::initializer_list<std::shared_ptr<Expression>>{arg1, arg2, arg3};
        calcType();
    }
public:
    FnType fnType() const
    {
        return fnType_;
    }
    static std::shared_ptr<BuiltInFunctionExpression> make(Location location, std::shared_ptr<Expression> arg1)
    {
        return std::shared_ptr<BuiltInFunctionExpression>(new BuiltInFunctionExpression(location, arg1));
    }
    static std::shared_ptr<BuiltInFunctionExpression> make(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2)
    {
        return std::shared_ptr<BuiltInFunctionExpression>(new BuiltInFunctionExpression(location, arg1, arg2));
    }
    static std::shared_ptr<BuiltInFunctionExpression> make(Location location, std::shared_ptr<Expression> arg1, std::shared_ptr<Expression> arg2, std::shared_ptr<Expression> arg3)
    {
        return std::shared_ptr<BuiltInFunctionExpression>(new BuiltInFunctionExpression(location, arg1, arg2, arg3));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // BUILTIN_FUNCTION_H_INCLUDED
