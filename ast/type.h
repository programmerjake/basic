#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#include "ast/base.h"
#include <cwchar>
#include <string>
#include <memory>
#include <initializer_list>
#include <vector>

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
    virtual std::shared_ptr<const Type> toRValue() const
    {
        return std::static_pointer_cast<const Type>(shared_from_this());
    }
    virtual bool isTypeAlias() const
    {
        return false;
    }
    virtual bool isIntegralType() const
    {
        return false;
    }
    virtual std::shared_ptr<const Type> getBaseType() const
    {
        return std::static_pointer_cast<const Type>(shared_from_this());
    }
    std::shared_ptr<const Type> getAbsoluteBaseType() const
    {
        std::shared_ptr<const Type> retval = getBaseType();
        while(retval->isTypeAlias())
            retval = retval->getBaseType();
        return retval;
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const = 0;
    virtual bool operator ==(const Type &rt) const = 0;
    bool operator !=(const Type &rt) const
    {
        return !operator==(rt);
    }
protected:
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const = 0;
public:
    static std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> a, std::shared_ptr<const Type> b)
    {
        if(a == nullptr || b == nullptr)
            return nullptr;
        return a->getCommonType(b);
    }
    static std::shared_ptr<const Type> getCommonType(std::initializer_list<std::shared_ptr<const Type>> types)
    {
        std::shared_ptr<const Type> retval = nullptr;
        for(std::shared_ptr<const Type> t : types)
        {
            if(t == nullptr)
                return nullptr;
            if(retval == nullptr)
                retval = t;
            else
            {
                retval = getCommonType(retval, t);
                if(retval == nullptr)
                    return nullptr;
            }
        }
        return retval;
    }
    static std::shared_ptr<const Type> getCommonType(const std::vector<std::shared_ptr<const Type>> &types)
    {
        std::shared_ptr<const Type> retval = nullptr;
        for(std::shared_ptr<const Type> t : types)
        {
            if(t == nullptr)
                return nullptr;
            if(retval == nullptr)
                retval = t;
            else
            {
                retval = getCommonType(retval, t);
                if(retval == nullptr)
                    return nullptr;
            }
        }
        return retval;
    }
};
}

#endif // TYPE_H_INCLUDED
