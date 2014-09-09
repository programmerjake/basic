#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

#include "ast/type.h"
#include "ast/expression.h"
#include <memory>
#include <cwchar>
#include <string>

namespace AST
{
class Variable : public Expression
{
    std::wstring name_;
protected:
    void name(const std::wstring &v)
    {
        name_ = v;
    }
    Variable(Location location, std::shared_ptr<const Type> type, const std::wstring &name)
        : Expression(location, type), name_(name)
    {
    }
public:
    const std::wstring &name() const
    {
        return name_;
    }
};

class ReferenceVariable final : public Variable
{
private:
    ReferenceVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name)
        : Variable(location, type, name)
    {
    }
public:
    static std::shared_ptr<ReferenceVariable> make(Location location, std::shared_ptr<const Type> type, const std::wstring &name)
    {
        return std::shared_ptr<ReferenceVariable>(new ReferenceVariable(location, type, name));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class AutoVariable final : public Variable
{
private:
    AutoVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name)
        : Variable(location, type, name)
    {
    }
public:
    static std::shared_ptr<AutoVariable> make(Location location, std::shared_ptr<const Type> type, const std::wstring &name)
    {
        return std::shared_ptr<AutoVariable>(new AutoVariable(location, type, name));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class StaticVariable final : public Variable
{
private:
    StaticVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name)
        : Variable(location, type, name)
    {
    }
public:
    static std::shared_ptr<StaticVariable> make(Location location, std::shared_ptr<const Type> type, const std::wstring &name)
    {
        return std::shared_ptr<StaticVariable>(new StaticVariable(location, type, name));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // VARIABLE_H_INCLUDED
