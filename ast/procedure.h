#ifndef PROCEDURE_H_INCLUDED
#define PROCEDURE_H_INCLUDED

#include "ast/type.h"
#include "ast/type_procedure.h"
#include "ast/expression.h"
#include "ast/variable.h"
#include "ast/code_block.h"
#include <memory>
#include <cwchar>
#include <string>

class ParserBase;

namespace AST
{
class Procedure final : public Expression
{
    friend class ParserBase;
    std::vector<std::shared_ptr<Variable>> args_;
    std::wstring name_;
public:
    std::shared_ptr<CodeBlock> code;
    std::shared_ptr<Variable> returnValue;
    const std::vector<std::shared_ptr<Variable>> &args() const
    {
        return args_;
    }
    std::wstring name() const
    {
        return name_;
    }
    std::shared_ptr<const TypeProcedure> type() const
    {
        return std::dynamic_pointer_cast<const TypeProcedure>(Expression::type());
    }
    std::vector<std::shared_ptr<Variable>> &argsRef()
    {
        return args_;
    }
private:
    Procedure(Location location, const std::wstring &name, std::shared_ptr<const TypeProcedure> type, std::vector<std::shared_ptr<Variable>> args, std::shared_ptr<CodeBlock> code, std::shared_ptr<Variable> returnValue)
        : Expression(location, type), args_(args), name_(name), code(code), returnValue(returnValue)
    {
        if(code != nullptr)
        {
            assert(args.size() == type->args.size());
        }
        else
        {
            assert(args.empty());
        }
        for(std::shared_ptr<Variable> v : args)
        {
            assert(v->isFunctionArgument());
        }
    }
public:
    static std::shared_ptr<Procedure> make(Location location, const std::wstring &name, std::shared_ptr<const TypeProcedure> type, std::vector<std::shared_ptr<Variable>> args, std::shared_ptr<CodeBlock> code, std::shared_ptr<Variable> returnValue)
    {
        return std::shared_ptr<Procedure>(new Procedure(location, name, type, args, code, returnValue));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};
}

#endif // PROCEDURE_H_INCLUDED
