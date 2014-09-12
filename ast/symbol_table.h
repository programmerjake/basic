#ifndef SYMBOL_TABLE_H_INCLUDED
#define SYMBOL_TABLE_H_INCLUDED

#include <unordered_map>
#include <cwchar>
#include <cwctype>
#include <string>
#include <memory>
#include <tuple>
#include <cassert>
#include <vector>
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
    std::shared_ptr<const AST::Type> type;
    std::shared_ptr<Base> value;
    explicit Symbol(const std::wstring &name, Type stype, std::shared_ptr<const AST::Type> type = nullptr, std::shared_ptr<Base> value = nullptr)
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
    std::unordered_map<std::wstring, size_t> symbolMap;
    std::vector<Symbol> symbols;
    SymbolTable()
    {
    }
    static std::wstring toLower(std::wstring str)
    {
        for(wchar_t &ch : str)
        {
            ch = towlower(ch);
        }
        return str;
    }
public:
    static std::shared_ptr<SymbolTable> make()
    {
        return std::shared_ptr<SymbolTable>(new SymbolTable);
    }
    bool exists(const std::wstring &name) const
    {
        return symbolMap.count(toLower(name)) > 0;
    }
    const Symbol &get(const std::wstring &name) const
    {
        return symbols[symbolMap.at(toLower(name))];
    }
    bool make(Symbol s)
    {
        assert(!s.empty());
        auto v = symbolMap.insert(std::pair<std::wstring, size_t>(toLower(s.name()), 0));
        if(std::get<1>(v))
        {
            std::get<1>(*std::get<0>(v)) = symbols.size();
            symbols.push_back(std::move(s));
        }
        else
        {
            symbols[std::get<1>(*std::get<0>(v))] = std::move(s);
        }
        return std::get<1>(v);
    }
    typedef std::vector<Symbol>::const_iterator iterator;
    iterator begin() const
    {
        return symbols.begin();
    }
    iterator end() const
    {
        return symbols.end();
    }
    iterator cbegin() const
    {
        return symbols.begin();
    }
    iterator cend() const
    {
        return symbols.end();
    }
    bool empty() const
    {
        return symbols.empty();
    }
    size_t size() const
    {
        return symbols.size();
    }
};
}

#endif // SYMBOL_TABLE_H_INCLUDED