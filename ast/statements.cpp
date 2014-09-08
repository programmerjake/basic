#include "ast/if_statement.h"
#include "ast/type.h"
#include "ast/type_builtin.h"
#include "ast/code_block.h"
#include "ast/statement.h"
#include "ast/expression.h"
#include "ast/cast_expression.h"
#include "location.h"
#include "error.h"
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
}