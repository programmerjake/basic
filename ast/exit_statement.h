#ifndef EXIT_STATEMENT_H_INCLUDED
#define EXIT_STATEMENT_H_INCLUDED

#include "ast/statement.h"
#include "ast/do_statement.h"
#include "ast/for_statement.h"
#include "ast/while_statement.h"
#include "ast/procedure.h"
#include <memory>

namespace AST
{
class ExitStatement : public Statement
{
public:
    explicit ExitStatement(Location location)
        : Statement(location)
    {
    }
};

class ExitDoStatement final : public ExitStatement
{
public:
    std::shared_ptr<DoStatement> statement;
private:
    ExitDoStatement(Location location, std::shared_ptr<DoStatement> statement)
        : ExitStatement(location), statement(statement)
    {
    }
public:
    static std::shared_ptr<ExitDoStatement> make(Location location, std::shared_ptr<DoStatement> statement)
    {
        return std::shared_ptr<ExitDoStatement>(new ExitDoStatement(location, statement));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};

class ExitForStatement final : public ExitStatement
{
public:
    std::shared_ptr<ForStatement> statement;
private:
    ExitForStatement(Location location, std::shared_ptr<ForStatement> statement)
        : ExitStatement(location), statement(statement)
    {
    }
public:
    static std::shared_ptr<ExitForStatement> make(Location location, std::shared_ptr<ForStatement> statement)
    {
        return std::shared_ptr<ExitForStatement>(new ExitForStatement(location, statement));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};

class ExitWhileStatement final : public ExitStatement
{
public:
    std::shared_ptr<WhileStatement> statement;
private:
    ExitWhileStatement(Location location, std::shared_ptr<WhileStatement> statement)
        : ExitStatement(location), statement(statement)
    {
    }
public:
    static std::shared_ptr<ExitWhileStatement> make(Location location, std::shared_ptr<WhileStatement> statement)
    {
        return std::shared_ptr<ExitWhileStatement>(new ExitWhileStatement(location, statement));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};

class ExitProcedureStatement final : public ExitStatement
{
public:
    std::shared_ptr<Procedure> procedure;
private:
    ExitProcedureStatement(Location location, std::shared_ptr<Procedure> procedure)
        : ExitStatement(location), procedure(procedure)
    {
    }
public:
    static std::shared_ptr<ExitProcedureStatement> make(Location location, std::shared_ptr<Procedure> procedure)
    {
        return std::shared_ptr<ExitProcedureStatement>(new ExitProcedureStatement(location, procedure));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};
}

#endif // EXIT_STATEMENT_H_INCLUDED
