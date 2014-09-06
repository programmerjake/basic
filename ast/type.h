#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#include "ast/base.h"
#include <cwchar>
#include <string>
#include <memory>

namespace AST
{
class Type : public Base
{
public:
    virtual std::wstring toString() const = 0;
    virtual bool isLValue() const
    {
        return false;
    }
    virtual std::shared_ptr<Type> toRValue() const
    {
        return std::static_pointer_cast<Type>(shared_from_this());
    }
    virtual bool isTypeAlias() const
    {
        return false;
    }
    virtual bool isIntegralType() const
    {
        return false;
    }
    virtual std::shared_ptr<Type> getBaseType() const
    {
        return std::static_pointer_cast<Type>(shared_from_this());
    }
    virtual bool canCastTo(std::shared_ptr<Type> destType, bool isImplicitCast = true) const = 0;
};
}

#endif // TYPE_H_INCLUDED
