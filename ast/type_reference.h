#ifndef TYPE_REFERENCE_H_INCLUDED
#define TYPE_REFERENCE_H_INCLUDED

#include "type.h"
#include <memory>

namespace AST
{
class TypeReference final : public Type
{
private:
    std::shared_ptr<const Type> type;
    TypeReference(Location location, std::shared_ptr<const Type> type)
        : Type(location), type(type)
    {
    }
public:
    static std::shared_ptr<TypeReference> make(Location location, std::shared_ptr<const Type> type)
    {
        return std::shared_ptr<TypeReference>(new TypeReference(location, type->toRValue()));
    }
    static std::shared_ptr<const Type> toLValue(std::shared_ptr<const Type> type)
    {
        if(type->isLValue())
            return type;
        return make(type->location(), type);
    }
    virtual std::wstring toString() const override
    {
        return L"ByRef " + type->toString();
    }
    virtual bool isLValue() const override
    {
        return true;
    }
    virtual std::shared_ptr<const Type> toRValue() const override
    {
        return type;
    }
    virtual bool isTypeAlias() const override
    {
        return type->isTypeAlias();
    }
    virtual bool isIntegralType() const override
    {
        return type->isIntegralType();
    }
    virtual std::shared_ptr<const Type> getBaseType() const override
    {
        return type->getBaseType();
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override
    {
        return type->canCastTo(destType->toRValue(), isImplicitCast);
    }
    virtual bool operator ==(const Type &rt) const override
    {
        if(rt.isLValue())
            return *type == *rt.toRValue();
        return false;
    }
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
protected:
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override
    {
        if(*this == *rt)
            return rt;
        return Type::getCommonType(type, rt->toRValue());
    }
};
}

#endif // TYPE_REFERENCE_H_INCLUDED
