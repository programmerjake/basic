#ifndef IF_STATEMENT_H_INCLUDED
#define IF_STATEMENT_H_INCLUDED

#include "ast/statement.h"
#include "ast/expression.h"
#include "ast/code_block.h"
#include <tuple>
#include <vector>
#include <memory>

namespace AST
{
class IfStatement final : public Statement
{
public:
    std::vector<std::pair<std::shared_ptr<Expression>, std::shared_ptr<CodeBlock>>> ifSections;
    std::shared_ptr<CodeBlock> elseSection;
private:
    IfStatement(Location location, std::vector<std::pair<std::shared_ptr<Expression>, std::shared_ptr<CodeBlock>>> ifSections, std::shared_ptr<CodeBlock> elseSection)
        : Statement(location), ifSections(ifSections), elseSection(elseSection)
    {
    }
public:
    static std::shared_ptr<IfStatement> make(Location location, std::vector<std::pair<std::shared_ptr<Expression>, std::shared_ptr<CodeBlock>>> ifSections, std::shared_ptr<CodeBlock> elseSection = nullptr)
    {
        return std::shared_ptr<IfStatement>(new IfStatement(location, ifSections, elseSection));
    }
    void checkTypes();
};
}

#endif // IF_STATEMENT_H_INCLUDED
