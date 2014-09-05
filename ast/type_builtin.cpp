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
#warning finish
    return false;
}

bool TypeDouble::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeInteger::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeInt8::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeInt16::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeInt32::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeInt64::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeSingle::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeString::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeUInt8::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeUInt16::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeUInt32::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

bool TypeUInt64::canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast) const
{
    destType = destType->toRValue();
    if(destType->isTypeAlias())
        destType = destType->getBaseType();
    if(destType.get() == this)
        return true;
#warning finish
    return false;
}

}