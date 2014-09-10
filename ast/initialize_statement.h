#ifndef INITIALIZE_STATEMENT_H_INCLUDED
#define INITIALIZE_STATEMENT_H_INCLUDED

#include "ast/variable.h"
#include "ast/type.h"
#include "ast/statement.h"
#include <memory>
#include <cwchar>
#include <string>

namespace AST
{
class InitializeStatement final : public Statement
{
public:
    std::shared_ptr<Variable> variable;
private:
    InitializeStatement(std::shared_ptr<Variable> variable)
        : Statement(variable->location()), variable(variable)
    {
    }
public:
    static std::shared_ptr<InitializeStatement> make(std::shared_ptr<Variable> variable)
    {
        return std::shared_ptr<InitializeStatement>(new InitializeStatement(variable));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};
}

#endif // INITIALIZE_STATEMENT_H_INCLUDED
