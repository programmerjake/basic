#ifndef LITERAL_EXPRESSION_H_INCLUDED
#define LITERAL_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "ast/type_builtin.h"
#include <cwchar>
#include <string>
#include <sstream>
#include <cstdint>
#include <functional>

namespace AST
{
class LiteralExpression : public Expression
{
public:
    LiteralExpression(std::shared_ptr<const Type> type)
        : Expression(type)
    {
    }
    virtual std::wstring toString(bool useHighPrecision = false) const = 0;
    virtual std::wstring toSourceString() const
    {
        return toString(true);
    }
};

class BooleanLiteralExpression : public LiteralExpression
{
    BooleanLiteralExpression(bool value)
        : LiteralExpression(TypeBoolean::getInstance()), value(value)
    {
    }
public:
    bool value;
    static std::shared_ptr<BooleanLiteralExpression> make(bool value)
    {
        return std::shared_ptr<BooleanLiteralExpression>(new BooleanLiteralExpression(value));
    }
    virtual std::shared_ptr<Base> dup() const override
    {
        return make(value);
    }
    virtual std::wstring toString(bool /*useHighPrecision*/ = false) const override
    {
        if(value)
            return L"True";
        return L"False";
    }
};

class DoubleLiteralExpression : public LiteralExpression
{
    DoubleLiteralExpression(double value)
        : LiteralExpression(TypeDouble::getInstance()), value(value)
    {
    }
public:
    double value;
    static std::shared_ptr<DoubleLiteralExpression> make(double value)
    {
        return std::shared_ptr<DoubleLiteralExpression>(new DoubleLiteralExpression(value));
    }
    virtual std::shared_ptr<Base> dup() const override
    {
        return make(value);
    }
    virtual std::wstring toString(bool useHighPrecision = false) const override
    {
        std::wostringstream ss;
        ss.precision(useHighPrecision ? 17 : 15);
        ss << value;
        return ss.str();
    }
};

class SingleLiteralExpression : public LiteralExpression
{
    SingleLiteralExpression(float value)
        : LiteralExpression(TypeSingle::getInstance()), value(value)
    {
    }
public:
    float value;
    static std::shared_ptr<SingleLiteralExpression> make(float value)
    {
        return std::shared_ptr<SingleLiteralExpression>(new SingleLiteralExpression(value));
    }
    virtual std::shared_ptr<Base> dup() const override
    {
        return make(value);
    }
    virtual std::wstring toString(bool useHighPrecision = false) const override
    {
        std::wostringstream ss;
        ss.precision(useHighPrecision ? 9 : 6);
        ss << value << L"!";
        return ss.str();
    }
};

class StringLiteralExpression : public LiteralExpression
{
    StringLiteralExpression(const std::wstring &value)
        : LiteralExpression(TypeString::getInstance()), value(value)
    {
    }
public:
    std::wstring value;
    static std::shared_ptr<StringLiteralExpression> make(const std::wstring &value)
    {
        return std::shared_ptr<StringLiteralExpression>(new StringLiteralExpression(value));
    }
    virtual std::shared_ptr<Base> dup() const override
    {
        return make(value);
    }
    virtual std::wstring toString(bool /*useHighPrecision*/ = false) const override
    {
        return value;
    }
    virtual std::wstring toSourceString() const override
    {
        std::wstring retval = L"\"";
        retval.reserve(value.size());
        for(wchar_t ch : value)
        {
            if(ch == '\"')
                retval += L"\"\"";
            else
                retval += ch;
        }
        retval += L"\"";
        return retval;
    }
};

class IntegerLiteralExpression : public LiteralExpression
{
    IntegerLiteralExpression(std::int64_t value, bool isSigned, std::shared_ptr<const Type> type)
        : LiteralExpression(type), value(value)
    {
    }
    static std::shared_ptr<const Type> calcType(std::int64_t value, bool isSigned)
    {
        std::int32_t v = value;
        if(v == value)
        {
            if(!isSigned)
                return TypeUInt32::getInstance();
            return TypeInteger::getInstance();
        }
        if(!isSigned)
            return TypeUInt64::getInstance();
        return TypeInt64::getInstance();
    }
    int getMessiness(uint64_t value, int base) const
    {
        int retval = 0;
        int lastDigit = -100;
        do
        {
            int digit = value % base;
            value /= base;
            int digitDistance = digit - lastDigit;
            lastDigit = digit;
            if(digitDistance < 0)
                digitDistance = -digitDistance;
            int additionalMessiness = 1;
            if(digitDistance <= 2)
                additionalMessiness -= digitDistance;
            retval += additionalMessiness;
        }
        while(value > 0);
        return value;
    }
    bool shouldDisplayAsHex() const
    {
        uint64_t value = this->value;
        if(isSigned)
        {
            value = -value;
        }
        if(value < 0x18)
            return false;
        if(getMessiness(value, 10) <= getMessiness(value, 0x10) * 11 / 10)
            return false;
        return true;
    }
public:
    std::int64_t value;
    bool isSigned;
    static std::shared_ptr<IntegerLiteralExpression> make(std::wstring value, std::function<void(const std::wstring &)> errorHandler = nullptr); // in expressions.cpp
    static std::shared_ptr<IntegerLiteralExpression> make(std::int64_t value, bool isSigned = true)
    {
        return std::shared_ptr<IntegerLiteralExpression>(new IntegerLiteralExpression(value, isSigned, calcType(value, isSigned)));
    }
    static std::shared_ptr<IntegerLiteralExpression> make(std::int64_t value, bool isSigned, std::shared_ptr<const Type> type)
    {
        return std::shared_ptr<IntegerLiteralExpression>(new IntegerLiteralExpression(value, isSigned, type));
    }
    virtual std::shared_ptr<Base> dup() const override
    {
        return make(value, isSigned, type());
    }
    virtual std::wstring toString(bool /*useHighPrecision*/ = false) const override
    {
        std::wostringstream ss;
        uint64_t value = this->value;
        if(isSigned && this->value < 0)
        {
            ss << L"-";
            value = -value;
        }
        if(shouldDisplayAsHex())
        {
            ss << std::hex << L"&H";
        }
        ss << value;
        if(type() != calcType(value, isSigned))
        {
            ss << L"_";
            if(type() == TypeInt8::getInstance())
                ss << L"8";
            else if(type() == TypeInt16::getInstance())
                ss << L"16";
            else if(type() == TypeInt32::getInstance())
                ss << L"32";
            else if(type() == TypeInt64::getInstance())
                ss << L"64";
            else if(type() == TypeUInt8::getInstance())
                ss << L"U8";
            else if(type() == TypeUInt16::getInstance())
                ss << L"U16";
            else if(type() == TypeUInt32::getInstance())
                ss << L"U32";
            else if(type() == TypeUInt64::getInstance())
                ss << L"U64";
            else
                ss << L"unknown";
        }
        return ss.str();
    }
};
}

#endif // LITERAL_EXPRESSION_H_INCLUDED
