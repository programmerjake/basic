#include "ast/if_statement.h"
#include "ast/for_statement.h"
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

void ForStatement::calcTypes()
{
    if(!variable->type()->isLValue())
        throw ParserError(location(), L"For variable is not a lvalue");
    if(!variable->type()->isNumericType())
        throw ParserError(location(), L"invalid type for For variable");
    shared_ptr<const Type> variableType = variable->type()->toRValue()->getAbsoluteBaseType();
    if(step == nullptr)
    {
        if(variableType == TypeSingle::getInstance())
            step = SingleLiteralExpression::make(location(), 1);
        else if(variableType == TypeDouble::getInstance())
            step = DoubleLiteralExpression::make(location(), 1);
        else if(variableType == TypeInt8::getInstance())
            step = IntegerLiteralExpression::make(location(), 1, true, TypeInt8::getInstance());
        else
            step = CastExpression::castImplicit(IntegerLiteralExpression::make(location(), 1, false, TypeUInt8::getInstance()), variableType);
    }
    else
    {
        step = CastExpression::castImplicit(step, variableType);
    }
    start = CastExpression::castImplicit(start, variableType);
    end = CastExpression::castImplicit(end, variableType);
}

void AssignStatement::writeCode(CodeWriter &cw) const
{
    cw.visitAssignStatement(static_pointer_cast<const AssignStatement>(shared_from_this()));
}

void InitializeStatement::writeCode(CodeWriter &cw) const
{
    cw.visitInitializeStatement(static_pointer_cast<const InitializeStatement>(shared_from_this()));
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

void ForStatement::writeCode(CodeWriter &cw) const
{
    cw.visitForStatement(static_pointer_cast<const ForStatement>(shared_from_this()));
}

void Procedure::writeCode(CodeWriter &cw) const
{
    cw.visitProcedure(static_pointer_cast<const Procedure>(shared_from_this()));
}
}