#ifndef TYPE_INTEGER_H_INCLUDED
#define TYPE_INTEGER_H_INCLUDED

#include "ast/type.h"
#include <memory>

namespace AST
{
#define TYPE_BUILTIN_DEFINITION(t) \
class Type##t final : public Type \
{ \
private: \
    Type##t() \
    { \
    } \
    Type##t(const Type##t &) = delete; \
    void operator =(const Type##t &) = delete; \
public: \
    static std::shared_ptr<Type##t> getInstance() \
    { \
        static std::shared_ptr<Type##t> retval = nullptr; \
        if(retval == nullptr) \
            retval = std::shared_ptr<Type##t>(new Type##t); \
        return retval; \
    } \
    virtual std::shared_ptr<Base> dup() const override \
    { \
        return getInstance(); \
    } \
    virtual std::wstring toString() const override \
    { \
        return L ## #t; \
    } \
}

TYPE_BUILTIN_DEFINITION(Boolean);
TYPE_BUILTIN_DEFINITION(Integer);
TYPE_BUILTIN_DEFINITION(Int8);
TYPE_BUILTIN_DEFINITION(Int16);
TYPE_BUILTIN_DEFINITION(Int32);
TYPE_BUILTIN_DEFINITION(Int64);
TYPE_BUILTIN_DEFINITION(Single);
TYPE_BUILTIN_DEFINITION(String);
TYPE_BUILTIN_DEFINITION(UInt8);
TYPE_BUILTIN_DEFINITION(UInt16);
TYPE_BUILTIN_DEFINITION(UInt32);
TYPE_BUILTIN_DEFINITION(UInt64);

#undef TYPE_BUILTIN_DEFINITION
}

#endif // TYPE_INTEGER_H_INCLUDED
