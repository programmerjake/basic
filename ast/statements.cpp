#include "ast/if_statement.h"
#include "ast/type.h"
#include "ast/type_builtin.h"
#include "ast/code_block.h"
#include "ast/statement.h"
#include "ast/expression.h"
#include "ast/cast_expression.h"
#include "location.h"
#include "error.h"
#include "code_writer.h"
#include <stdexcept>

using namespace std;

namespace AST
{
void IfStatement::checkTypes()
{
    if(ifSections.empty())
        throw logic_error("no if sections in IfStatement::checkTypes()");
    for(pair<shared_ptr<Expression>, shared_ptr<CodeBlock>> &ifSection : ifSections)
    {
        get<0>(ifSection) = CastExpression::castImplicit(get<0>(ifSection), TypeBoolean::getInstance());
    }
}

void AssignStatement::writeCode(CodeWriter &cw) const
{
    cw.visitAssignStatement(static_pointer_cast<const AssignStatement>(shared_from_this()));
}

void CodeBlock::writeCode(CodeWriter &cw) const
{
    cw.visitCodeBlock(static_pointer_cast<const CodeBlock>(shared_from_this()));
}

void DoStatement::writeCode(CodeWriter &cw) const
{
    cw.visitDoStatement(static_pointer_cast<const DoStatement>(shared_from_this()));
}

void IfStatement::writeCode(CodeWriter &cw) const
{
    cw.visitIfStatement(static_pointer_cast<const IfStatement>(shared_from_this()));
}

void WhileStatement::writeCode(CodeWriter &cw) const
{
    cw.visitWhileStatement(static_pointer_cast<const WhileStatement>(shared_from_this()));
}
}