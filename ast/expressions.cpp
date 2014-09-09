#include "ast/type.h"
#include "ast/type_builtin.h"
#include "ast/or_expression.h"
#include "ast/and_expression.h"
#include "ast/xor_expression.h"
#include "ast/cast_expression.h"
#include "ast/literal_expression.h"
#include "ast/not_expression.h"
#include "ast/add_expression.h"
#include "ast/fdiv_expression.h"
#include "ast/idiv_expression.h"
#include "ast/mod_expression.h"
#include "ast/mul_expression.h"
#include "ast/neg_expression.h"
#include "ast/sub_expression.h"
#include "ast/builtin_function.h"
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

void CompareExpression::checkTypes()
{
    auto t = calcTypeH(argsRef(), getOperatorString(ctype()));
    if(t->getAbsoluteBaseType() == TypeBoolean::getInstance() && !isOrdered(ctype()))
        return;
    if(t->isNumericType())
        return;
    if(t->getAbsoluteBaseType() == TypeString::getInstance())
        return;
    throw ParserError(location(), L"invalid argument types for " + getOperatorString(ctype()));
}

void AddExpression::calcType()
{
    type(calcTypeH(argsRef(), L"binary +"));
    if(type()->isNumericType())
        return;
    if(type()->getAbsoluteBaseType() == TypeString::getInstance())
        return;
    throw ParserError(location(), L"invalid argument types for binary +");
}

void FDivExpression::calcType()
{
    shared_ptr<Expression> &l = argsRef().at(0);
    shared_ptr<Expression> &r = argsRef().at(1);
    shared_ptr<const Type> lBaseType = l->type()->getAbsoluteBaseType();
    shared_ptr<const Type> rBaseType = r->type()->getAbsoluteBaseType();
    if(lBaseType == TypeDouble::getInstance()
       || lBaseType == TypeInt64::getInstance()
       || lBaseType == TypeUInt64::getInstance()
       || rBaseType == TypeDouble::getInstance()
       || rBaseType == TypeInt64::getInstance()
       || rBaseType == TypeUInt64::getInstance())
        type(TypeDouble::getInstance());
    else if(lBaseType == TypeSingle::getInstance() || rBaseType == TypeSingle::getInstance())
        type(TypeSingle::getInstance());
    else
        type(TypeDouble::getInstance());
    l = CastExpression::castImplicit(l, type());
    r = CastExpression::castImplicit(r, type());
}

void IDivExpression::calcType()
{
    type(calcTypeH(argsRef(), L"\\"));
    if(type()->isIntegralType())
        return;
    throw ParserError(location(), L"invalid argument types for \\");
}

void ModExpression::calcType()
{
    type(calcTypeH(argsRef(), L"Mod"));
    if(type()->isIntegralType())
        return;
    throw ParserError(location(), L"invalid argument types for Mod");
}

void MulExpression::calcType()
{
    type(calcTypeH(argsRef(), L"*"));
    if(type()->isNumericType())
        return;
    throw ParserError(location(), L"invalid argument types for *");
}

void NegExpression::calcType()
{
    type(calcTypeH(argsRef(), L"unary -"));
    if(type()->isNumericType())
        return;
    throw ParserError(location(), L"invalid argument type for unary -");
}

void UnaryPlusExpression::calcType()
{
    type(calcTypeH(argsRef(), L"unary +"));
    if(type()->isNumericType())
        return;
    throw ParserError(location(), L"invalid argument type for unary +");
}

void SubExpression::calcType()
{
    type(calcTypeH(argsRef(), L"binary -"));
    if(type()->isNumericType())
        return;
    throw ParserError(location(), L"invalid argument types for binary -");
}

void BuiltInFunctionExpression::calcType()
{
    switch(fnType())
    {
    case BuiltInFunctionExpression::FnType::Abs:
    {
        shared_ptr<const Type> baseType = args().front()->type()->toRValue()->getAbsoluteBaseType();
        if(baseType != TypeDouble::getInstance()
           && baseType != TypeSingle::getInstance()
           && baseType != TypeInteger::getInstance()
           && baseType != TypeInt8::getInstance()
           && baseType != TypeInt16::getInstance()
           && baseType != TypeInt32::getInstance()
           && baseType != TypeInt64::getInstance())
            throw ParserError(location(), L"invalid argument type for Abs");
        type(baseType);
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::Asc:
    {
        shared_ptr<const Type> baseType = args().front()->type()->toRValue()->getAbsoluteBaseType();
        if(baseType != TypeString::getInstance())
            throw ParserError(location(), L"invalid argument type for Asc");
        type(TypeInteger::getInstance());
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::ATn1:
    {
        shared_ptr<const Type> baseType = Type::getCommonType(args().front()->type()->toRValue()->getAbsoluteBaseType(), TypeSingle::getInstance());
        if(baseType == nullptr)
            throw ParserError(location(), L"invalid argument type for ATn");
        type(baseType);
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::ATn2:
    {
        shared_ptr<const Type> baseType = Type::getCommonType(args()[0]->type()->toRValue()->getAbsoluteBaseType(), TypeSingle::getInstance());
        baseType = Type::getCommonType(args()[1]->type()->toRValue()->getAbsoluteBaseType(), baseType);
        if(baseType == nullptr)
            throw ParserError(location(), L"invalid argument types for ATn");
        type(baseType);
        argsRef()[0] = CastExpression::castImplicit(args()[0], baseType);
        argsRef()[1] = CastExpression::castImplicit(args()[1], baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::Chr:
    {
        type(TypeString::getInstance());
        argsRef().front() = CastExpression::castImplicit(args().front(), TypeInteger::getInstance());
        return;
    }
    case BuiltInFunctionExpression::FnType::Cos:
    {
        shared_ptr<const Type> baseType = Type::getCommonType(args().front()->type()->toRValue()->getAbsoluteBaseType(), TypeSingle::getInstance());
        if(baseType == nullptr)
            throw ParserError(location(), L"invalid argument type for Cos");
        type(baseType);
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::Exp:
    {
        shared_ptr<const Type> baseType = Type::getCommonType(args().front()->type()->toRValue()->getAbsoluteBaseType(), TypeSingle::getInstance());
        if(baseType == nullptr)
            throw ParserError(location(), L"invalid argument type for Exp");
        type(baseType);
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::Hex:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::InStr2:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::InStr3:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::LBound1:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::LBound2:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::LCase:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Left:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Mid2:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Mid3:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Oct:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Right:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::RTrim:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Sgn:
    {
        shared_ptr<const Type> baseType = args().front()->type()->toRValue()->getAbsoluteBaseType();
        if(baseType != TypeDouble::getInstance()
           && baseType != TypeSingle::getInstance()
           && baseType != TypeInteger::getInstance()
           && baseType != TypeInt8::getInstance()
           && baseType != TypeInt16::getInstance()
           && baseType != TypeInt32::getInstance()
           && baseType != TypeInt64::getInstance())
            throw ParserError(location(), L"invalid argument type for Abs");
        type(baseType);
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::Sin:
    {
        shared_ptr<const Type> baseType = Type::getCommonType(args().front()->type()->toRValue()->getAbsoluteBaseType(), TypeSingle::getInstance());
        if(baseType == nullptr)
            throw ParserError(location(), L"invalid argument type for Sin");
        type(baseType);
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::Space:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Sqr:
    {
        shared_ptr<const Type> baseType = Type::getCommonType(args().front()->type()->toRValue()->getAbsoluteBaseType(), TypeSingle::getInstance());
        if(baseType == nullptr)
            throw ParserError(location(), L"invalid argument type for Sqr");
        type(baseType);
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::String:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Tan:
    {
        shared_ptr<const Type> baseType = Type::getCommonType(args().front()->type()->toRValue()->getAbsoluteBaseType(), TypeSingle::getInstance());
        if(baseType == nullptr)
            throw ParserError(location(), L"invalid argument type for Tan");
        type(baseType);
        argsRef().front() = CastExpression::castImplicit(args().front(), baseType);
        return;
    }
    case BuiltInFunctionExpression::FnType::UBound1:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::UBound2:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::UCase:
        assert(false);
        return;
    case BuiltInFunctionExpression::FnType::Val:
        assert(false);
        return;
    }
    assert(false);
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
            if(retval >= -(uint64_t)digit && digit > 0)
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

void CompareExpression::writeCode(CodeWriter &cw) const
{
    cw.visitCompareExpression(static_pointer_cast<const CompareExpression>(shared_from_this()));
}

void AddExpression::writeCode(CodeWriter &cw) const
{
    cw.visitAddExpression(static_pointer_cast<const AddExpression>(shared_from_this()));
}

void FDivExpression::writeCode(CodeWriter &cw) const
{
    cw.visitFDivExpression(static_pointer_cast<const FDivExpression>(shared_from_this()));
}

void IDivExpression::writeCode(CodeWriter &cw) const
{
    cw.visitIDivExpression(static_pointer_cast<const IDivExpression>(shared_from_this()));
}

void ModExpression::writeCode(CodeWriter &cw) const
{
    cw.visitModExpression(static_pointer_cast<const ModExpression>(shared_from_this()));
}

void MulExpression::writeCode(CodeWriter &cw) const
{
    cw.visitMulExpression(static_pointer_cast<const MulExpression>(shared_from_this()));
}

void NegExpression::writeCode(CodeWriter &cw) const
{
    cw.visitNegExpression(static_pointer_cast<const NegExpression>(shared_from_this()));
}

void SubExpression::writeCode(CodeWriter &cw) const
{
    cw.visitSubExpression(static_pointer_cast<const SubExpression>(shared_from_this()));
}

void UnaryPlusExpression::writeCode(CodeWriter &cw) const
{
    cw.visitUnaryPlusExpression(static_pointer_cast<const UnaryPlusExpression>(shared_from_this()));
}

void BuiltInFunctionExpression::writeCode(CodeWriter &cw) const
{
    cw.visitBuiltInFunctionExpression(static_pointer_cast<const BuiltInFunctionExpression>(shared_from_this()));
}
}