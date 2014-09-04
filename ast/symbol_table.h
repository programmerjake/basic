#ifndef SYMBOL_TABLE_H_INCLUDED
#define SYMBOL_TABLE_H_INCLUDED

#include <unordered_map>
#include <cwchar>
#include <string>
#include <memory>
#include <tuple>
#include <cassert>
#include "ast/base.h"
#include "ast/type.h"

namespace AST
{
class Symbol final
{
public:
    enum class Type
    {
        None,
        ReferenceVariable,
        AutoVariable,
        StaticVariable,
        Type,
        Procedure,
    };
private:
    std::wstring name_;
    Type stype_;
public:
    const std::wstring &name() const
    {
        return name_;
    }
    Type stype() const
    {
        return stype_;
    }
    std::shared_ptr<AST::Type> type;
    std::shared_ptr<Base> value;
    explicit Symbol(const std::wstring &name, Type stype, std::shared_ptr<AST::Type> type = nullptr, std::shared_ptr<Base> value = nullptr)
        : name_(name), stype_(stype), type(type), value(value)
    {
    }
    Symbol()
        : Symbol(L"", Type::None)
    {
    }
    bool empty() const
    {
        return stype() == Type::None;
    }
};

class SymbolTable final
{
    std::unordered_map<std::wstring, Symbol> symbols;
public:
    bool exists(const std::wstring &name) const
    {
        return symbols.count(name) > 0;
    }
    const Symbol &get(const std::wstring &name) const
    {
        return symbols.at(name);
    }
    bool make(Symbol s)
    {
        assert(!s.empty());
        return std::get<1>(symbols.insert(std::pair<std::wstring, Symbol>(s.name(), s)));
    }
};
}

#endif // SYMBOL_TABLE_H_INCLUDED