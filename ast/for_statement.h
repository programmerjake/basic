#ifndef FOR_STATEMENT_H_INCLUDED
#define FOR_STATEMENT_H_INCLUDED

#include "ast/expression.h"
#include "ast/statement.h"
#include "ast/code_block.h"
#include "ast/cast_expression.h"
#include "ast/type_builtin.h"
#include <memory>

namespace AST
{
class ForStatement final : public Statement
{
public:
    std::shared_ptr<Expression> variable;
    std::shared_ptr<Expression> start;
    std::shared_ptr<Expression> end;
    std::shared_ptr<Expression> step;
    std::shared_ptr<CodeBlock> body;
private:
    void calcTypes(); // in statements.cpp
    ForStatement(Location location, std::shared_ptr<Expression> variable, std::shared_ptr<Expression> start, std::shared_ptr<Expression> end, std::shared_ptr<Expression> step, std::shared_ptr<CodeBlock> body)
        : Statement(location), variable(variable), start(start), end(end), step(step), body(body)
    {
        calcTypes();
    }
public:
    static std::shared_ptr<ForStatement> make(Location location, std::shared_ptr<Expression> variable, std::shared_ptr<Expression> start, std::shared_ptr<Expression> end, std::shared_ptr<Expression> step, std::shared_ptr<CodeBlock> body)
    {
        return std::shared_ptr<ForStatement>(new ForStatement(location, variable, start, end, step, body));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};
}

#endif // FOR_STATEMENT_H_INCLUDED
