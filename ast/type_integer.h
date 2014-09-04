#ifndef TYPE_INTEGER_H_INCLUDED
#define TYPE_INTEGER_H_INCLUDED

#include "ast/type.h"
#include <memory>

namespace AST
{
class TypeInteger final : public Type
{
private:
    TypeInteger()
    {
    }
    TypeInteger(const TypeInteger &) = delete;
    void operator =(const TypeInteger &) = delete;
public:
    static std::shared_ptr<TypeInteger> getInstance()
    {
        static std::shared_ptr<TypeInteger> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeInteger>(new TypeInteger);
        return retval;
    }
    virtual std::shared_ptr<Base> dup() const override
    {
        return getInstance();
    }
    virtual std::wstring toString() const override
    {
        return L"Integer";
    }
};
}

#endif // TYPE_INTEGER_H_INCLUDED
