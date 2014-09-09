#ifndef TYPE_INTEGER_H_INCLUDED
#define TYPE_INTEGER_H_INCLUDED

#include "ast/type.h"
#include <memory>
#include <iostream>

namespace AST
{
class TypeBoolean final : public Type
{
private:
    TypeBoolean()
        : Type(Location())
    {
        //std::cout << "made type Boolean" << std::endl;
    }
    TypeBoolean(const TypeBoolean &) = delete;
    void operator =(const TypeBoolean &) = delete;
public:
    static std::shared_ptr<TypeBoolean> getInstance()
    {
        static std::shared_ptr<TypeBoolean> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeBoolean>(new TypeBoolean);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"Boolean";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeDouble final : public Type
{
private:
    TypeDouble()
        : Type(Location())
    {
        //std::cout << "made type Double" << std::endl;
    }
    TypeDouble(const TypeDouble &) = delete;
    void operator =(const TypeDouble &) = delete;
public:
    static std::shared_ptr<TypeDouble> getInstance()
    {
        static std::shared_ptr<TypeDouble> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeDouble>(new TypeDouble);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"Double";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

/** 32 or 64 bits depending on target architecture
 */
class TypeInteger final : public Type
{
private:
    TypeInteger()
        : Type(Location())
    {
        //std::cout << "made type Integer" << std::endl;
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
    virtual std::wstring toString() const override
    {
        return L"Integer";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeInt8 final : public Type
{
private:
    TypeInt8()
        : Type(Location())
    {
        //std::cout << "made type Int8" << std::endl;
    }

    TypeInt8(const TypeInt8 &) = delete;
    void operator =(const TypeInt8 &) = delete;
public:
    static std::shared_ptr<TypeInt8> getInstance()
    {
        static std::shared_ptr<TypeInt8> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeInt8>(new TypeInt8);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"Int8";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeInt16 final : public Type
{
private:
    TypeInt16()
        : Type(Location())
    {
        //std::cout << "made type Int16" << std::endl;
    }

    TypeInt16(const TypeInt16 &) = delete;
    void operator =(const TypeInt16 &) = delete;
public:
    static std::shared_ptr<TypeInt16> getInstance()
    {
        static std::shared_ptr<TypeInt16> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeInt16>(new TypeInt16);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"Int16";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeInt32 final : public Type
{
private:
    TypeInt32()
        : Type(Location())
    {
        //std::cout << "made type Int32" << std::endl;
    }

    TypeInt32(const TypeInt32 &) = delete;
    void operator =(const TypeInt32 &) = delete;
public:
    static std::shared_ptr<TypeInt32> getInstance()
    {
        static std::shared_ptr<TypeInt32> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeInt32>(new TypeInt32);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"Int32";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeInt64 final : public Type
{
private:
    TypeInt64()
        : Type(Location())
    {
        //std::cout << "made type Int64" << std::endl;
    }

    TypeInt64(const TypeInt64 &) = delete;
    void operator =(const TypeInt64 &) = delete;
public:
    static std::shared_ptr<TypeInt64> getInstance()
    {
        static std::shared_ptr<TypeInt64> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeInt64>(new TypeInt64);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"Int64";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeSingle final : public Type
{
private:
    TypeSingle()
        : Type(Location())
    {
        //std::cout << "made type Single" << std::endl;
    }

    TypeSingle(const TypeSingle &) = delete;
    void operator =(const TypeSingle &) = delete;
public:
    static std::shared_ptr<TypeSingle> getInstance()
    {
        static std::shared_ptr<TypeSingle> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeSingle>(new TypeSingle);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"Single";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeString final : public Type
{
private:
    TypeString()
        : Type(Location())
    {
        //std::cout << "made type String" << std::endl;
    }

    TypeString(const TypeString &) = delete;
    void operator =(const TypeString &) = delete;
public:
    static std::shared_ptr<TypeString> getInstance()
    {
        static std::shared_ptr<TypeString> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeString>(new TypeString);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"String";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeUInt8 final : public Type
{
private:
    TypeUInt8()
        : Type(Location())
    {
        //std::cout << "made type UInt8" << std::endl;
    }

    TypeUInt8(const TypeUInt8 &) = delete;
    void operator =(const TypeUInt8 &) = delete;
public:
    static std::shared_ptr<TypeUInt8> getInstance()
    {
        static std::shared_ptr<TypeUInt8> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeUInt8>(new TypeUInt8);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"UInt8";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeUInt16 final : public Type
{
private:
    TypeUInt16()
        : Type(Location())
    {
        //std::cout << "made type UInt16" << std::endl;
    }

    TypeUInt16(const TypeUInt16 &) = delete;
    void operator =(const TypeUInt16 &) = delete;
public:
    static std::shared_ptr<TypeUInt16> getInstance()
    {
        static std::shared_ptr<TypeUInt16> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeUInt16>(new TypeUInt16);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"UInt16";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeUInt32 final : public Type
{
private:
    TypeUInt32()
        : Type(Location())
    {
        //std::cout << "made type UInt32" << std::endl;
    }

    TypeUInt32(const TypeUInt32 &) = delete;
    void operator =(const TypeUInt32 &) = delete;
public:
    static std::shared_ptr<TypeUInt32> getInstance()
    {
        static std::shared_ptr<TypeUInt32> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeUInt32>(new TypeUInt32);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"UInt32";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

class TypeUInt64 final : public Type
{
private:
    TypeUInt64()
        : Type(Location())
    {
        //std::cout << "made type UInt64" << std::endl;
    }

    TypeUInt64(const TypeUInt64 &) = delete;
    void operator =(const TypeUInt64 &) = delete;
public:
    static std::shared_ptr<TypeUInt64> getInstance()
    {
        static std::shared_ptr<TypeUInt64> retval = nullptr;
        if(retval == nullptr)
            retval = std::shared_ptr<TypeUInt64>(new TypeUInt64);
        return retval;
    }
    virtual std::wstring toString() const override
    {
        return L"UInt64";
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override;
    virtual bool isIntegralType() const override
    {
        return true;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override;
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
};

}

#endif // TYPE_INTEGER_H_INCLUDED
