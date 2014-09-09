#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

#include "ast/type.h"
#include "ast/expression.h"
#include "ast/cast_expression.h"
#include <memory>
#include <cwchar>
#include <string>

namespace AST
{
class Variable : public Expression
{
    std::wstring name_;
    std::shared_ptr<Expression> initializer_;
protected:
    void name(const std::wstring &v)
    {
        name_ = v;
    }
    void initializer(std::shared_ptr<Expression> v)
    {
        initializer_ = v;
    }
    Variable(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer)
        : Expression(location, type), name_(name), initializer_(initializer)
    {
    }
public:
    const std::wstring &name() const
    {
        return name_;
    }
    std::shared_ptr<Expression> initializer() const
    {
        return initializer_;
    }
};

class ReferenceVariable final : public Variable
{
private:
    ReferenceVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer)
        : Variable(location, type, name, initializer != nullptr ? CastExpression::castImplicit(initializer, type) : nullptr)
    {
    }
public:
    static std::shared_ptr<ReferenceVariable> make(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer = nullptr)
    {
        return std::shared_ptr<ReferenceVariable>(new ReferenceVariable(location, type, name, initializer));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class AutoVariable final : public Variable
{
private:
    AutoVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer)
        : Variable(location, type, name, initializer != nullptr ? CastExpression::castImplicit(initializer, type->toRValue()) : nullptr)
    {
    }
public:
    static std::shared_ptr<AutoVariable> make(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer)
    {
        return std::shared_ptr<AutoVariable>(new AutoVariable(location, type, name, initializer));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class StaticVariable final : public Variable
{
private:
    StaticVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer)
        : Variable(location, type, name, initializer != nullptr ? CastExpression::castImplicit(initializer, type->toRValue()) : nullptr)
    {
    }
public:
    static std::shared_ptr<StaticVariable> make(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer)
    {
        return std::shared_ptr<StaticVariable>(new StaticVariable(location, type, name, initializer));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // VARIABLE_H_INCLUDED
