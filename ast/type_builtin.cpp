#include "ast/type_builtin.h"

namespace AST
{
bool TypeBoolean::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
    if(destType->isIntegeralType() && !isImplicitCast)
        return true;
    return false;
}

bool TypeDouble::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeInteger::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeInt8::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeInt16::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeInt32::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeInt64::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeSingle::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeString::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
    return false;
}

bool TypeUInt8::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeUInt16::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeUInt32::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

bool TypeUInt64::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
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

}