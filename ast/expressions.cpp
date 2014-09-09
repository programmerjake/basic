#include "ast/type.h"
#include "ast/type_builtin.h"
#include "ast/or_expression.h"
#include "ast/and_expression.h"
#include "ast/xor_expression.h"
#include "ast/cast_expression.h"
#include "ast/literal_expression.h"
#include "ast/not_expression.h"
#include "error.h"
#include "code_writer.h"
#include <algorithm>
#include <vector>
#include <memory>
#include <cwchar>
#include <string>
#include <cstdint>
#include <sstream>
#include <cwctype>

using namespace std;

namespace AST
{
namespace
{
shared_ptr<const Type> calcTypeH(vector<shared_ptr<Expression>> &args, const wstring &opName, bool plural = true)
{
    vector<shared_ptr<const Type>> types;
    types.reserve(args.size());

    for(const shared_ptr<Expression> &e : args)
    {
        types.push_back(e->type()->toRValue());
    }

    shared_ptr<const Type> type = Type::getCommonType(types);

    if(type == nullptr)
    {
        throw ParserError(args[0]->location(), L"invalid argument type" + static_cast<wstring>(plural ? L"s" : L"") + L" for " + opName);
    }

    for(shared_ptr<Expression> &e : args)
    {
        e = CastExpression::castImplicitNotChecked(e, type);
    }

    return type;
}

shared_ptr<const Type> calcTypeHLogicOps(vector<shared_ptr<Expression>> &args, const wstring &opName, bool plural = true)
{
    shared_ptr<const Type> type = calcTypeH(args, opName, plural);

    if(type->getAbsoluteBaseType() != TypeBoolean::getInstance() && !type->isIntegralType())
    {
        throw ParserError(args[0]->location(), L"invalid argument type" + static_cast<wstring>(plural ? L"s" : L"") + L" for " + opName);
    }

    return type;
}
}

void OrExpression::calcType()
{
    type(calcTypeHLogicOps(argsRef(), L"Or"));
}

void AndExpression::calcType()
{
    type(calcTypeHLogicOps(argsRef(), L"And"));
}

void XorExpression::calcType()
{
    type(calcTypeHLogicOps(argsRef(), L"Xor"));
}

void NotExpression::calcType()
{
    type(calcTypeHLogicOps(argsRef(), L"Not", false));
}

namespace
{
int IntegerLiteralMakeOnErrorHelper(function<void(Location location, const std::wstring &)> errorHandler, Location location, const std::wstring &msg)
{
    if(errorHandler)
        errorHandler(location, msg);
    return 0;
}

bool iswodigit(wint_t ch)
{
    if(ch >= '0' && ch <= '7')
        return true;
    return false;
}
}

shared_ptr<IntegerLiteralExpression> IntegerLiteralExpression::make(Location location, wstring value, function<void(Location, const std::wstring &)> errorHandler)
{
    if(value.empty())
        return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"empty string"));
    wstring typeString = L"";
    size_t underlinePos = value.find_last_of(L'_');
    if(underlinePos != wstring::npos)
    {
        typeString = value.substr(underlinePos + 1);
        value.erase(underlinePos);
        if(value.empty())
            return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"missing digits"));
        if(typeString.empty())
            return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"missing literal type after _"));
    }
    bool isNegative = (value[0] == '-');
    if(value[0] == '-' || value[0] == '+')
    {
        value.erase(0, 1);
    }
    if(value.empty())
        return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"missing digits"));
    uint64_t retval = 0;
    constexpr uint64_t top3Bits = ~(~(uint64_t)0 >> 3);
    constexpr uint64_t top4Bits = ~(~(uint64_t)0 >> 4);
    if(value[0] == '&')
    {
        if(value.size() < 2)
            return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"missing digits"));
        if(iswodigit(value[1]) || value[1] == 'o' || value[1] == 'O')
        {
            size_t start = 1;
            if(value[1] == 'o' || value[1] == 'O')
            {
                start = 2;
                if(value.size() < 3)
                    return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"missing digits"));
            }
            for(size_t i = start; i < value.size(); i++)
            {
                if(!iswodigit(value[i]))
                    return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"invalid octal digit"));
                int digit = value[i] - '0';
                if(retval & top3Bits)
                    return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"number too big"));
                retval <<= 3;
                retval += digit;
            }
        }
        else
        {
            if(value.size() < 3)
                return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"missing digits"));
            if(value[1] == 'h' || value[1] == 'H') // Hex
            {
                for(size_t i = 2; i < value.size(); i++)
                {
                    if(!iswxdigit(value[i]))
                        return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"invalid hexadecimal digit"));
                    int digit;
                    if(iswdigit(value[i]))
                        digit = value[i] - '0';
                    else if(iswlower(value[i]))
                        digit = value[i] - 'a' + 0xA;
                    else // upper case
                        digit = value[i] - 'A' + 0xA;
                    if(retval & top4Bits)
                        return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"number too big"));
                    retval <<= 4;
                    retval += digit;
                }
            }
            else
                return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"invalid base indicator"));
        }
    }
    else
    {
        static constexpr uint64_t maxBeforeMultiplying = ~(uint64_t)0 / 10;
        for(size_t i = 0; i < value.size(); i++)
        {
            if(!iswdigit(value[i]))
                return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"invalid decimal digit"));
            int digit = value[i] - '0';
            if(retval > maxBeforeMultiplying)
                return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"number too big"));
            retval *= 10;
            if(retval >= -(uint64_t)digit)
                return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"number too big"));
            retval += digit;
        }
    }
    if(typeString != L"")
    {
        if(typeString == L"b" || typeString == L"B")
            typeString = L"U8";
        if(typeString[0] == 'u')
            typeString[0] = 'U';
        uint64_t mask;
        shared_ptr<const Type> type;
        bool isSigned;
        if(typeString == L"U8")
        {
            mask = 0xFF;
            type = TypeUInt8::getInstance();
            isSigned = false;
        }
        else if(typeString == L"8")
        {
            mask = 0xFF;
            type = TypeInt8::getInstance();
            isSigned = true;
        }
        else if(typeString == L"U16")
        {
            mask = 0xFFFF;
            type = TypeUInt16::getInstance();
            isSigned = false;
        }
        else if(typeString == L"16")
        {
            mask = 0xFFFF;
            type = TypeInt16::getInstance();
            isSigned = true;
        }
        else if(typeString == L"U32")
        {
            mask = 0xFFFFFFFFUL;
            type = TypeUInt32::getInstance();
            isSigned = false;
        }
        else if(typeString == L"32")
        {
            mask = 0xFFFFFFFFUL;
            type = TypeInt32::getInstance();
            isSigned = true;
        }
        else if(typeString == L"U64")
        {
            mask = ~(uint64_t)0;
            type = TypeUInt64::getInstance();
            isSigned = false;
        }
        else if(typeString == L"64")
        {
            mask = ~(uint64_t)0;
            type = TypeInt64::getInstance();
            isSigned = true;
        }
        else
        {
            return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"invalid literal type"));
        }
        if(retval & ~mask)
            return make(location, IntegerLiteralMakeOnErrorHelper(errorHandler, location, L"number too big"));
        if(isNegative)
            retval = mask + 1 - retval;
        return make(location, retval, isSigned, type);
    }
    if(isNegative)
        retval = -retval;
    return make(location, retval);
}

void AndExpression::writeCode(CodeWriter &cw) const
{
    cw.visitAndExpression(static_pointer_cast<const AndExpression>(shared_from_this()));
}

void CastExpression::writeCode(CodeWriter &cw) const
{
    cw.visitCastExpression(static_pointer_cast<const CastExpression>(shared_from_this()));
}

void BooleanLiteralExpression::writeCode(CodeWriter &cw) const
{
    cw.visitBooleanLiteralExpression(static_pointer_cast<const BooleanLiteralExpression>(shared_from_this()));
}

void DoubleLiteralExpression::writeCode(CodeWriter &cw) const
{
    cw.visitDoubleLiteralExpression(static_pointer_cast<const DoubleLiteralExpression>(shared_from_this()));
}

void SingleLiteralExpression::writeCode(CodeWriter &cw) const
{
    cw.visitSingleLiteralExpression(static_pointer_cast<const SingleLiteralExpression>(shared_from_this()));
}

void StringLiteralExpression::writeCode(CodeWriter &cw) const
{
    cw.visitStringLiteralExpression(static_pointer_cast<const StringLiteralExpression>(shared_from_this()));
}

void IntegerLiteralExpression::writeCode(CodeWriter &cw) const
{
    cw.visitIntegerLiteralExpression(static_pointer_cast<const IntegerLiteralExpression>(shared_from_this()));
}

void NotExpression::writeCode(CodeWriter &cw) const
{
    cw.visitNotExpression(static_pointer_cast<const NotExpression>(shared_from_this()));
}

void OrExpression::writeCode(CodeWriter &cw) const
{
    cw.visitOrExpression(static_pointer_cast<const OrExpression>(shared_from_this()));
}

void AutoVariable::writeCode(CodeWriter &cw) const
{
    cw.visitAutoVariable(static_pointer_cast<const AutoVariable>(shared_from_this()));
}

void ReferenceVariable::writeCode(CodeWriter &cw) const
{
    cw.visitReferenceVariable(static_pointer_cast<const ReferenceVariable>(shared_from_this()));
}

void StaticVariable::writeCode(CodeWriter &cw) const
{
    cw.visitStaticVariable(static_pointer_cast<const StaticVariable>(shared_from_this()));
}

void XorExpression::writeCode(CodeWriter &cw) const
{
    cw.visitXorExpression(static_pointer_cast<const XorExpression>(shared_from_this()));
}
}