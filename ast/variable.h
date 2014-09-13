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
class Procedure;

class Variable : public Expression
{
    std::wstring name_;
    std::shared_ptr<Expression> initializer_;
    bool isFunctionArgument_;
    std::shared_ptr<Procedure> containingProcedure_;
protected:
    void name(const std::wstring &v)
    {
        name_ = v;
    }
    void initializer(std::shared_ptr<Expression> v)
    {
        initializer_ = v;
    }
    Variable(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer, bool isFunctionArgument, std::shared_ptr<Procedure> containingProcedure)
        : Expression(location, type), name_(name), initializer_(initializer), isFunctionArgument_(isFunctionArgument), containingProcedure_(containingProcedure)
    {
    }
    void isFunctionArgument(bool v)
    {
        isFunctionArgument_ = v;
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
    bool isFunctionArgument() const
    {
        return isFunctionArgument_;
    }
    static std::wstring getReturnValueName()
    {
        return L"$ReturnValue";
    }
    std::shared_ptr<Procedure> containingProcedure() const
    {
        return containingProcedure_;
    }
    void containingProcedure(std::shared_ptr<Procedure> v)
    {
        containingProcedure_ = v;
    }
};

class ReferenceVariable final : public Variable
{
private:
    ReferenceVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer, bool isFunctionArgument, std::shared_ptr<Procedure> containingProcedure)
        : Variable(location, type, name, initializer != nullptr ? CastExpression::castImplicit(initializer, type) : nullptr, isFunctionArgument, containingProcedure)
    {
    }
public:
    static std::shared_ptr<ReferenceVariable> make(Location location, std::shared_ptr<Procedure> containingProcedure, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer, bool isFunctionArgument = false)
    {
        return std::shared_ptr<ReferenceVariable>(new ReferenceVariable(location, type, name, initializer, isFunctionArgument, containingProcedure));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class AutoVariable final : public Variable
{
private:
    AutoVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer, bool isFunctionArgument, std::shared_ptr<Procedure> containingProcedure)
        : Variable(location, type, name, initializer != nullptr ? CastExpression::castImplicit(initializer, type->toRValue()) : nullptr, isFunctionArgument, containingProcedure)
    {
    }
public:
    static std::shared_ptr<AutoVariable> make(Location location, std::shared_ptr<Procedure> containingProcedure, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer, bool isFunctionArgument = false)
    {
        return std::shared_ptr<AutoVariable>(new AutoVariable(location, type, name, initializer, isFunctionArgument, containingProcedure));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};

class StaticVariable final : public Variable
{
private:
    StaticVariable(Location location, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer, std::shared_ptr<Procedure> containingProcedure)
        : Variable(location, type, name, initializer != nullptr ? CastExpression::castImplicit(initializer, type->toRValue()) : nullptr, false, containingProcedure)
    {
    }
public:
    static std::shared_ptr<StaticVariable> make(Location location, std::shared_ptr<Procedure> containingProcedure, std::shared_ptr<const Type> type, const std::wstring &name, std::shared_ptr<Expression> initializer)
    {
        return std::shared_ptr<StaticVariable>(new StaticVariable(location, type, name, initializer, containingProcedure));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // VARIABLE_H_INCLUDED
