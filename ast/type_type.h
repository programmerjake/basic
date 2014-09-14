#ifndef TYPE_TYPE_H_INCLUDED
#define TYPE_TYPE_H_INCLUDED

#include "ast/type.h"
#include "ast/symbol_table.h"
#include <memory>
#include <cwchar>
#include <string>

namespace AST
{
class TypeType final : public Type
{
    std::wstring name_;
public:
    std::shared_ptr<SymbolTable> symbols;
    std::wstring name() const
    {
        return name_;
    }
    bool isComplete;
private:
    TypeType(Location location, std::wstring name, std::shared_ptr<SymbolTable> symbols, bool isComplete)
        : Type(location), name_(name), symbols(symbols), isComplete(isComplete)
    {
    }
public:
    static std::shared_ptr<TypeType> make(Location location, std::wstring name, std::shared_ptr<SymbolTable> symbols, bool isComplete = true)
    {
        return std::shared_ptr<TypeType>(new TypeType(location, name, symbols, isComplete));
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override
    {
        return *this == destType;
    }
    virtual bool operator ==(const Type &rt) const override
    {
        return this == &rt;
    }
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override
    {
        if(*this == *rt)
            return rt;
        return nullptr;
    }
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
    virtual std::wstring toString() const override
    {
        return name();
    }
    #warning finish
};
}

#endif // TYPE_TYPE_H_INCLUDED
