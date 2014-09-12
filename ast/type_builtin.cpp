#include "ast/all.h"
#include <cassert>
#include "code_writer.h"

using namespace std;

namespace AST
{
namespace
{
bool isUnsigned(const shared_ptr<const Type> &t)
{
    if(t == TypeUInt8::getInstance() || t == TypeUInt16::getInstance() || t == TypeUInt32::getInstance() || t == TypeUInt64::getInstance())
        return true;
    return false;
}

bool isFloatingPoint(const shared_ptr<const Type> &t)
{
    if(t == TypeSingle::getInstance() || t == TypeDouble::getInstance())
        return true;
    return false;
}

bool isAtMost8Bits(const shared_ptr<const Type> &t)
{
    if(t == TypeUInt8::getInstance() || t == TypeInt8::getInstance())
        return true;
    return false;
}

bool fitsInInt8(const shared_ptr<const Type> &t)
{
    if(t == TypeInt8::getInstance())
        return true;
    return false;
}

bool isAtMost16Bits(const shared_ptr<const Type> &t)
{
    if(isAtMost8Bits(t) || t == TypeUInt16::getInstance() || t == TypeInt16::getInstance())
        return true;
    return false;
}

bool fitsInInt16(const shared_ptr<const Type> &t)
{
    if(isAtMost8Bits(t) || t == TypeInt16::getInstance())
        return true;
    return false;
}

bool isAtMost32Bits(const shared_ptr<const Type> &t)
{
    if(isAtMost16Bits(t) || t == TypeUInt32::getInstance() || t == TypeInt32::getInstance())
        return true;
    return false;
}

bool fitsInInt32(const shared_ptr<const Type> &t)
{
    if(isAtMost16Bits(t) || t == TypeInt32::getInstance())
        return true;
    return false;
}

bool isAtMost64Bits(const shared_ptr<const Type> &t)
{
    if(isAtMost32Bits(t) || t == TypeUInt64::getInstance() || t == TypeInt64::getInstance() || t == TypeInteger::getInstance())
        return true;
    return false;
}

bool fitsInInt64(const shared_ptr<const Type> &t)
{
    if(isAtMost32Bits(t) || t == TypeInt64::getInstance() || t == TypeInteger::getInstance())
        return true;
    return false;
}

bool is64Bits(const shared_ptr<const Type> &t)
{
    if(t == TypeUInt64::getInstance() || t == TypeInt64::getInstance())
        return true;
    return false;
}

shared_ptr<const Type> getCommonTypeH(shared_ptr<const Type> a, shared_ptr<const Type> b)
{
    a = a->toRValue();
    b = b->toRValue();
    if(*a == *b)
        return a;
    shared_ptr<const Type> aBaseType = a->getAbsoluteBaseType(), bBaseType = b->getAbsoluteBaseType();
    if(*aBaseType == *bBaseType)
        return aBaseType;
    if(isFloatingPoint(aBaseType) || isFloatingPoint(bBaseType))
    {
        if(!isFloatingPoint(aBaseType) && !isAtMost64Bits(aBaseType))
            return nullptr;
        if(!isFloatingPoint(bBaseType) && !isAtMost64Bits(bBaseType))
            return nullptr;
        shared_ptr<const Type> retval = TypeSingle::getInstance();
        if(aBaseType == TypeDouble::getInstance() || bBaseType == TypeDouble::getInstance() || is64Bits(aBaseType) || is64Bits(bBaseType))
        {
            retval = TypeDouble::getInstance();
        }
        return retval;
    }
    if(isUnsigned(a) && isUnsigned(b))
    {
        if(isAtMost8Bits(a) && isAtMost8Bits(b))
            return TypeUInt8::getInstance();
        if(isAtMost16Bits(a) && isAtMost16Bits(b))
            return TypeUInt16::getInstance();
        if(isAtMost32Bits(a) && isAtMost32Bits(b))
            return TypeUInt32::getInstance();
        if(isAtMost64Bits(a) && isAtMost64Bits(b))
            return TypeUInt64::getInstance();
    }
    if(a == TypeInteger::getInstance() || b == TypeInteger::getInstance())
    {
        if(a != TypeInteger::getInstance())
        {
            swap(a, b);
        }
        assert(a == TypeInteger::getInstance());
        if(!is64Bits(b))
        {
            return TypeInteger::getInstance();
        }
    }
    if(fitsInInt8(a) && fitsInInt8(b))
        return TypeInt8::getInstance();
    if(fitsInInt16(a) && fitsInInt16(b))
        return TypeInt16::getInstance();
    if(fitsInInt32(a) && fitsInInt32(b))
        return TypeInt32::getInstance();
    if(fitsInInt64(a) && fitsInInt64(b))
        return TypeInt64::getInstance();
    return nullptr;
}
}

bool TypeBoolean::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType->isIntegralType() && !isImplicitCast)
        return true;
    return false;
}

void TypeBoolean::writeCode(CodeWriter &cw) const
{
    cw.visitTypeBoolean(static_pointer_cast<const TypeBoolean>(shared_from_this()));
}

shared_ptr<const Type> TypeBoolean::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeDouble::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt64::getInstance())
        return !isImplicitCast;
    if(destType == TypeInteger::getInstance())
        return !isImplicitCast;
    if(destType == TypeSingle::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt64::getInstance())
        return !isImplicitCast;
    return false;
}

void TypeDouble::writeCode(CodeWriter &cw) const
{
    cw.visitTypeDouble(static_pointer_cast<const TypeDouble>(shared_from_this()));
}

shared_ptr<const Type> TypeDouble::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeInteger::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt64::getInstance())
        return true;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt64::getInstance())
        return !isImplicitCast;
    return false;
}

void TypeInteger::writeCode(CodeWriter &cw) const
{
    cw.visitTypeInteger(static_pointer_cast<const TypeInteger>(shared_from_this()));
}

shared_ptr<const Type> TypeInteger::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeInt8::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt16::getInstance())
        return true;
    if(destType == TypeInt32::getInstance())
        return true;
    if(destType == TypeInt64::getInstance())
        return true;
    if(destType == TypeInteger::getInstance())
        return true;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt64::getInstance())
        return !isImplicitCast;
    return false;
}

void TypeInt8::writeCode(CodeWriter &cw) const
{
    cw.visitTypeInt8(static_pointer_cast<const TypeInt8>(shared_from_this()));
}

shared_ptr<const Type> TypeInt8::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeInt16::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt32::getInstance())
        return true;
    if(destType == TypeInt64::getInstance())
        return true;
    if(destType == TypeInteger::getInstance())
        return true;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt64::getInstance())
        return !isImplicitCast;
    return false;
}

void TypeInt16::writeCode(CodeWriter &cw) const
{
    cw.visitTypeInt16(static_pointer_cast<const TypeInt16>(shared_from_this()));
}

shared_ptr<const Type> TypeInt16::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeInt32::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt64::getInstance())
        return true;
    if(destType == TypeInteger::getInstance())
        return true;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt64::getInstance())
        return !isImplicitCast;
    return false;
}

void TypeInt32::writeCode(CodeWriter &cw) const
{
    cw.visitTypeInt32(static_pointer_cast<const TypeInt32>(shared_from_this()));
}

shared_ptr<const Type> TypeInt32::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeInt64::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeInteger::getInstance())
        return !isImplicitCast;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt64::getInstance())
        return !isImplicitCast;
    return false;
}

void TypeInt64::writeCode(CodeWriter &cw) const
{
    cw.visitTypeInt64(static_pointer_cast<const TypeInt64>(shared_from_this()));
}

shared_ptr<const Type> TypeInt64::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeSingle::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt64::getInstance())
        return !isImplicitCast;
    if(destType == TypeInteger::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt64::getInstance())
        return !isImplicitCast;
    return false;
}

void TypeSingle::writeCode(CodeWriter &cw) const
{
    cw.visitTypeSingle(static_pointer_cast<const TypeSingle>(shared_from_this()));
}

shared_ptr<const Type> TypeSingle::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeString::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    return false;
}

void TypeString::writeCode(CodeWriter &cw) const
{
    cw.visitTypeString(static_pointer_cast<const TypeString>(shared_from_this()));
}

shared_ptr<const Type> TypeString::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeUInt8::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return true;
    if(destType == TypeInt32::getInstance())
        return true;
    if(destType == TypeInt64::getInstance())
        return true;
    if(destType == TypeInteger::getInstance())
        return true;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt16::getInstance())
        return true;
    if(destType == TypeUInt32::getInstance())
        return true;
    if(destType == TypeUInt64::getInstance())
        return true;
    return false;
}

void TypeUInt8::writeCode(CodeWriter &cw) const
{
    cw.visitTypeUInt8(static_pointer_cast<const TypeUInt8>(shared_from_this()));
}

shared_ptr<const Type> TypeUInt8::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeUInt16::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt32::getInstance())
        return true;
    if(destType == TypeInt64::getInstance())
        return true;
    if(destType == TypeInteger::getInstance())
        return true;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return true;
    if(destType == TypeUInt64::getInstance())
        return true;
    return false;
}

void TypeUInt16::writeCode(CodeWriter &cw) const
{
    cw.visitTypeUInt16(static_pointer_cast<const TypeUInt16>(shared_from_this()));
}

shared_ptr<const Type> TypeUInt16::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeUInt32::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt64::getInstance())
        return true;
    if(destType == TypeInteger::getInstance())
        return true;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt64::getInstance())
        return true;
    return false;
}

void TypeUInt32::writeCode(CodeWriter &cw) const
{
    cw.visitTypeUInt32(static_pointer_cast<const TypeUInt32>(shared_from_this()));
}

shared_ptr<const Type> TypeUInt32::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

bool TypeUInt64::canCastTo(shared_ptr<const Type> destType, bool isImplicitCast) const
{
    if(destType->isLValue())
        return false;
    if(destType->isTypeAlias() && !isImplicitCast)
        destType = destType->getAbsoluteBaseType();
    if(destType.get() == this)
        return true;
    if(destType == TypeDouble::getInstance())
        return true;
    if(destType == TypeInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt32::getInstance())
        return !isImplicitCast;
    if(destType == TypeInt64::getInstance())
        return !isImplicitCast;
    if(destType == TypeInteger::getInstance())
        return !isImplicitCast;
    if(destType == TypeSingle::getInstance())
        return true;
    if(destType == TypeUInt8::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt16::getInstance())
        return !isImplicitCast;
    if(destType == TypeUInt32::getInstance())
        return !isImplicitCast;
    return false;
}

void TypeUInt64::writeCode(CodeWriter &cw) const
{
    cw.visitTypeUInt64(static_pointer_cast<const TypeUInt64>(shared_from_this()));
}

shared_ptr<const Type> TypeUInt64::getCommonType(shared_ptr<const Type> rt) const
{
    return getCommonTypeH(static_pointer_cast<const Type>(shared_from_this()), rt);
}

void TypeReference::writeCode(CodeWriter &cw) const
{
    cw.visitTypeReference(static_pointer_cast<const TypeReference>(shared_from_this()));
}

void TypeArray::writeCode(CodeWriter &cw) const
{
    cw.visitTypeArray(static_pointer_cast<const TypeArray>(shared_from_this()));
}

constexpr TypeArray::IndexRange TypeArray::defaultEmptyRange;
constexpr int64_t TypeArray::defaultRangeStart;

void TypeProcedure::writeCode(CodeWriter &cw) const
{
    cw.visitTypeProcedure(static_pointer_cast<const TypeProcedure>(shared_from_this()));
}
}