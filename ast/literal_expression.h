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
    LiteralExpression(Location location, std::shared_ptr<const Type> type)
        : Expression(location, type)
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
    BooleanLiteralExpression(Location location, bool value)
        : LiteralExpression(location, TypeBoolean::getInstance()), value(value)
    {
    }
public:
    bool value;
    static std::shared_ptr<BooleanLiteralExpression> make(Location location, bool value)
    {
        return std::shared_ptr<BooleanLiteralExpression>(new BooleanLiteralExpression(location, value));
    }
    virtual std::wstring toString(bool /*useHighPrecision*/ = false) const override
    {
        if(value)
            return L"True";
        return L"False";
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class DoubleLiteralExpression : public LiteralExpression
{
    DoubleLiteralExpression(Location location, double value)
        : LiteralExpression(location, TypeDouble::getInstance()), value(value)
    {
    }
public:
    double value;
    static std::shared_ptr<DoubleLiteralExpression> make(Location location, double value)
    {
        return std::shared_ptr<DoubleLiteralExpression>(new DoubleLiteralExpression(location, value));
    }
    virtual std::wstring toString(bool useHighPrecision = false) const override
    {
        std::wostringstream ss;
        ss.precision(useHighPrecision ? 17 : 15);
        ss << std::showpoint << value;
        return ss.str();
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class SingleLiteralExpression : public LiteralExpression
{
    SingleLiteralExpression(Location location, float value)
        : LiteralExpression(location, TypeSingle::getInstance()), value(value)
    {
    }
public:
    float value;
    static std::shared_ptr<SingleLiteralExpression> make(Location location, float value)
    {
        return std::shared_ptr<SingleLiteralExpression>(new SingleLiteralExpression(location, value));
    }
    virtual std::wstring toString(bool useHighPrecision = false) const override
    {
        std::wostringstream ss;
        ss.precision(useHighPrecision ? 9 : 6);
        ss << value << L"!";
        return ss.str();
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class StringLiteralExpression : public LiteralExpression
{
    StringLiteralExpression(Location location, const std::wstring &value)
        : LiteralExpression(location, TypeString::getInstance()), value(value)
    {
    }
public:
    std::wstring value;
    static std::shared_ptr<StringLiteralExpression> make(Location location, const std::wstring &value)
    {
        return std::shared_ptr<StringLiteralExpression>(new StringLiteralExpression(location, value));
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
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class IntegerLiteralExpression : public LiteralExpression
{
    IntegerLiteralExpression(Location location, std::int64_t value, bool isSigned, std::shared_ptr<const Type> type)
        : LiteralExpression(location, type), value(value)
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
        int lastDigit = 0;
        do
        {
            int digit = value % base;
            value /= base;
            int digitDistance = digit - lastDigit;
            if(digitDistance > base / 2)
                digitDistance -= base;
            else if(digitDistance < -base / 2)
                digitDistance += base;
            lastDigit = digit;
            int additionalMessiness = 1;
            digitDistance *= digitDistance;
            if(digitDistance > 4)
                digitDistance = 4;
            additionalMessiness += digitDistance;
            retval += additionalMessiness;
        }
        while(value > 0);
        return retval;
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
        if(getMessiness(value, 10) <= getMessiness(value, 0x10) * 25 / 10)
            return false;
        return true;
    }
public:
    std::int64_t value;
    bool isSigned;
    static std::shared_ptr<IntegerLiteralExpression> make(Location location, std::wstring value, std::function<void(Location, const std::wstring &)> errorHandler = nullptr); // in expressions.cpp
    static std::shared_ptr<IntegerLiteralExpression> make(Location location, std::int64_t value, bool isSigned = true)
    {
        return std::shared_ptr<IntegerLiteralExpression>(new IntegerLiteralExpression(location, value, isSigned, calcType(value, isSigned)));
    }
    static std::shared_ptr<IntegerLiteralExpression> make(Location location, std::int64_t value, bool isSigned, std::shared_ptr<const Type> type)
    {
        return std::shared_ptr<IntegerLiteralExpression>(new IntegerLiteralExpression(location, value, isSigned, type));
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
            ss << std::hex << std::uppercase << L"&H";
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
    std::wstring toCString() const
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
            ss << std::hex << std::uppercase << L"0x";
        }
        ss << value;
        if(type() == TypeInteger::getInstance())
            return ss.str() + L"L";
        else if(type() == TypeInt8::getInstance())
            return L"(int8_t)" + ss.str();
        else if(type() == TypeInt16::getInstance())
            return L"(int16_t)" + ss.str();
        else if(type() == TypeInt32::getInstance())
            return L"(int32_t)" + ss.str();
        else if(type() == TypeInt64::getInstance())
            return L"(int64_t)" + ss.str() + L"LL";
        else if(type() == TypeUInt8::getInstance())
            return L"(uint8_t)" + ss.str() + L"U";
        else if(type() == TypeUInt16::getInstance())
            return L"(uint16_t)" + ss.str() + L"U";
        else if(type() == TypeUInt32::getInstance())
            return L"(uint32_t)" + ss.str() + L"U";
        else if(type() == TypeUInt64::getInstance())
            return L"(uint64_t)" + ss.str() + L"ULL";
        return ss.str();
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // LITERAL_EXPRESSION_H_INCLUDED
