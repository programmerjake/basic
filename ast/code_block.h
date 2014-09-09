#ifndef CODE_BLOCK_H_INCLUDED
#define CODE_BLOCK_H_INCLUDED

#include "ast/statement.h"
#include "ast/symbol_table.h"
#include <vector>
#include <memory>

namespace AST
{
class CodeBlock final : public Statement
{
    CodeBlock(Location location, std::shared_ptr<SymbolTable> symbols, std::vector<std::shared_ptr<Statement>> statements)
        : Statement(location), symbols(symbols), statements(statements)
    {
    }
public:
    std::shared_ptr<SymbolTable> symbols;
    std::vector<std::shared_ptr<Statement>> statements;
    static std::shared_ptr<CodeBlock> make(Location location, std::shared_ptr<SymbolTable> symbols, std::vector<std::shared_ptr<Statement>> statements)
    {
        return std::shared_ptr<CodeBlock>(new CodeBlock(location, symbols, statements));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in statements.cpp
};
}

#endif // CODE_BLOCK_H_INCLUDED
