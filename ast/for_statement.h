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
    void calcTypes(); // in statements.cpp
private:
    ForStatement(Location location, std::shared_ptr<Expression> variable, std::shared_ptr<Expression> start, std::shared_ptr<Expression> end, std::shared_ptr<Expression> step, std::shared_ptr<CodeBlock> body, bool skipCalcTypes)
        : Statement(location), variable(variable), start(start), end(end), step(step), body(body)
    {
        if(!skipCalcTypes)
            calcTypes();
    }
public:
    static std::shared_ptr<ForStatement> make(Location location, std::shared_ptr<Expression> variable, std::shared_ptr<Expression> start, std::shared_ptr<Expression> end, std::shared_ptr<Expression> step, std::shared_ptr<CodeBlock> body, bool skipCalcTypes = false)
    {
        return std::shared_ptr<ForStatement>(new ForStatement(location, variable, start, end, step, body, skipCalcTypes));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};
}

#endif // FOR_STATEMENT_H_INCLUDED
