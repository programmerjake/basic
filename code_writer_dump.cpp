#include "code_writer_dump.h"
#include "string_cast.h"

using namespace std;

void CodeWriterDump::dumpExpression(shared_ptr<const AST::Expression> node, string nodeName)
{
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".Type:\n";
    indent.depth++;
    node->type()->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".Args:\n";
    indent.depth++;
    for(shared_ptr<const AST::Expression> e : node->args())
    {
        e->writeCode(*this);
    }
    indent.depth--;
}

void CodeWriterDump::visitAddExpression(shared_ptr<const AST::AddExpression> node)
{
    dumpExpression(node, "Add");
}

void CodeWriterDump::visitAndExpression(shared_ptr<const AST::AndExpression> node)
{
    dumpExpression(node, "And");
}

void CodeWriterDump::visitArrayIndexExpression(shared_ptr<const AST::ArrayIndexExpression> node)
{
    string nodeName = "ArrayIndex";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".Type:\n";
    indent.depth++;
    node->type()->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".Array:\n";
    indent.depth++;
    node->args()[0]->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".Indexes:\n";
    indent.depth++;
    bool first = true;
    for(shared_ptr<const AST::Expression> e : node->args())
    {
        if(first)
            first = false;
        else
            e->writeCode(*this);
    }
    indent.depth--;
}

void CodeWriterDump::visitAssignStatement(shared_ptr<const AST::AssignStatement> node)
{
    string nodeName = "Assign";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".LHS:\n";
    indent.depth++;
    node->lhs->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".RHS:\n";
    indent.depth++;
    node->rhs->writeCode(*this);
    indent.depth--;
}

void CodeWriterDump::visitAutoVariable(shared_ptr<const AST::AutoVariable> node)
{
    string nodeName = "AutoVariable";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".Name: " << string_cast<string>(node->name()) << "\n";
    os() << indent << nodeName << ".Type:\n";
    indent.depth++;
    node->type()->writeCode(*this);
    indent.depth--;
    if(node->initializer() != nullptr)
    {
        os() << indent << nodeName << ".Initializer:\n";
        indent.depth++;
        node->initializer()->writeCode(*this);
        indent.depth--;
    }
}

void CodeWriterDump::visitBooleanLiteralExpression(shared_ptr<const AST::BooleanLiteralExpression> node)
{
    os() << indent << "BooleanLiteral: " << string_cast<string>(node->toSourceString()) << "\n";
}

void CodeWriterDump::visitBuiltInFunctionExpression(shared_ptr<const AST::BuiltInFunctionExpression> node)
{
    dumpExpression(node, string_cast<string>(L"BuiltInFunction(" + AST::BuiltInFunctionExpression::fnName(node->fnType()) + L")"));
}

void CodeWriterDump::visitCallExpression(shared_ptr<const AST::CallExpression> node)
{
    string nodeName = "Call";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".Type:\n";
    indent.depth++;
    node->type()->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".Procedure:\n";
    indent.depth++;
    node->args()[0]->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".Arguments:\n";
    indent.depth++;
    bool first = true;
    for(shared_ptr<const AST::Expression> e : node->args())
    {
        if(first)
            first = false;
        else
            e->writeCode(*this);
    }
    indent.depth--;
}

void CodeWriterDump::visitCastExpression(shared_ptr<const AST::CastExpression> node)
{
    dumpExpression(node, "Cast");
}

void CodeWriterDump::visitCodeBlock(shared_ptr<const AST::CodeBlock> node)
{
    string nodeName = "CodeBlock";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".Symbols:\n";
    indent.depth++;
    for(const AST::Symbol &symbol : *node->symbols)
    {
        os() << indent << "Symbol:\n";
        os() << indent << "Symbol.Name: " << string_cast<string>(symbol.name()) << "\n";
        indent.depth++;
        symbol.value->writeCode(*this);
        indent.depth--;
    }
    indent.depth--;
    os() << indent << nodeName << ".Statements:\n";
    indent.depth++;
    for(shared_ptr<const AST::Statement> s : node->statements)
    {
        s->writeCode(*this);
    }
    indent.depth--;
}

void CodeWriterDump::visitCompareExpression(shared_ptr<const AST::CompareExpression> node)
{
    dumpExpression(node, string_cast<string>(L"Compare(" + AST::CompareExpression::getOperatorString(node->ctype()) + L")"));
}

void CodeWriterDump::visitDoStatement(shared_ptr<const AST::DoStatement> node)
{
    string nodeName = "Do";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".conditionType: ";
    switch(node->conditionType)
    {
    case AST::DoStatement::ConditionType::None:
        os() << "None\n";
        break;
    case AST::DoStatement::ConditionType::DoWhile:
        os() << "DoWhile\n";
        break;
    case AST::DoStatement::ConditionType::DoUntil:
        os() << "DoUntil\n";
        break;
    case AST::DoStatement::ConditionType::LoopWhile:
        os() << "LoopWhile\n";
        break;
    case AST::DoStatement::ConditionType::LoopUntil:
        os() << "LoopUntil\n";
        break;
    }
    os() << indent << nodeName << ".condition:\n";
    indent.depth++;
    node->condition->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".body:\n";
    indent.depth++;
    node->body->writeCode(*this);
    indent.depth--;
}

void CodeWriterDump::visitDoubleLiteralExpression(shared_ptr<const AST::DoubleLiteralExpression> node)
{
    os() << indent << "DoubleLiteral: " << string_cast<string>(node->toSourceString()) << "\n";
}

void CodeWriterDump::visitFDivExpression(shared_ptr<const AST::FDivExpression> node)
{
    dumpExpression(node, "FDiv");
}

void CodeWriterDump::visitForStatement(shared_ptr<const AST::ForStatement> node)
{
    string nodeName = "For";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".variable:\n";
    indent.depth++;
    node->variable->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".start:\n";
    indent.depth++;
    node->start->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".end:\n";
    indent.depth++;
    node->end->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".step:\n";
    indent.depth++;
    node->step->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".body:\n";
    indent.depth++;
    node->body->writeCode(*this);
    indent.depth--;
}

void CodeWriterDump::visitIDivExpression(shared_ptr<const AST::IDivExpression> node)
{
    dumpExpression(node, "IDiv");
}

void CodeWriterDump::visitIfStatement(shared_ptr<const AST::IfStatement> node)
{
    string nodeName = "If";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    for(pair<shared_ptr<AST::Expression>, shared_ptr<AST::CodeBlock>> ifSection : node->ifSections)
    {
        os() << indent << nodeName << ".condition:\n";
        indent.depth++;
        std::get<0>(ifSection)->writeCode(*this);
        indent.depth--;
        os() << indent << nodeName << ".code:\n";
        indent.depth++;
        std::get<1>(ifSection)->writeCode(*this);
        indent.depth--;
    }
    if(node->elseSection != nullptr)
    {
        os() << indent << nodeName << ".else:\n";
        indent.depth++;
        node->elseSection->writeCode(*this);
        indent.depth--;
    }
}

void CodeWriterDump::visitInitializeStatement(shared_ptr<const AST::InitializeStatement> node)
{
    os() << indent << "InitializeStatement:\n";
    indent.depth++;
    node->variable->writeCode(*this);
    indent.depth--;
}

void CodeWriterDump::visitIntegerLiteralExpression(shared_ptr<const AST::IntegerLiteralExpression> node)
{
    os() << indent << "IntegerLiteral: " << string_cast<string>(node->toSourceString()) << "\n";
}

void CodeWriterDump::visitModExpression(shared_ptr<const AST::ModExpression> node)
{
    dumpExpression(node, "Mod");
}

void CodeWriterDump::visitMulExpression(shared_ptr<const AST::MulExpression> node)
{
    dumpExpression(node, "Mul");
}

void CodeWriterDump::visitNegExpression(shared_ptr<const AST::NegExpression> node)
{
    dumpExpression(node, "Neg");
}

void CodeWriterDump::visitNotExpression(shared_ptr<const AST::NotExpression> node)
{
    dumpExpression(node, "Not");
}

void CodeWriterDump::visitOrExpression(shared_ptr<const AST::OrExpression> node)
{
    dumpExpression(node, "Or");
}

void CodeWriterDump::visitProcedure(shared_ptr<const AST::Procedure> node)
{
    os() << indent << "Procedure:\n";
    os() << indent << "Procedure.Type:\n";
    indent.depth++;
    node->type()->writeCode(*this);
    indent.depth--;
    if(!std::get<1>(procedures.insert(node)))
        return;
    size_t argNumber = 0;
    for(shared_ptr<const AST::Variable> arg : node->args())
    {
        if(arg != nullptr)
        os() << indent << "Procedure.Argument" << ++argNumber << ":\n";
        indent.depth++;
        arg->writeCode(*this);
        indent.depth--;
    }
    if(node->code)
    {
        os() << indent << "Procedure.Code:\n";
        indent.depth++;
        node->code->writeCode(*this);
        indent.depth--;
    }
}

void CodeWriterDump::visitReferenceVariable(shared_ptr<const AST::ReferenceVariable> node)
{
    string nodeName = "ReferenceVariable";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".Name: " << string_cast<string>(node->name()) << "\n";
    os() << indent << nodeName << ".Type:\n";
    indent.depth++;
    node->type()->writeCode(*this);
    indent.depth--;
    if(node->initializer() != nullptr)
    {
        os() << indent << nodeName << ".Initializer:\n";
        indent.depth++;
        node->initializer()->writeCode(*this);
        indent.depth--;
    }
}

void CodeWriterDump::visitSingleLiteralExpression(shared_ptr<const AST::SingleLiteralExpression> node)
{
    os() << indent << "SingleLiteral: " << string_cast<string>(node->toSourceString()) << "\n";
}

void CodeWriterDump::visitStaticVariable(shared_ptr<const AST::StaticVariable> node)
{
    string nodeName = "StaticVariable";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".Name: " << string_cast<string>(node->name()) << "\n";
    os() << indent << nodeName << ".Type:\n";
    indent.depth++;
    node->type()->writeCode(*this);
    indent.depth--;
    if(node->initializer() != nullptr)
    {
        os() << indent << nodeName << ".Initializer:\n";
        indent.depth++;
        node->initializer()->writeCode(*this);
        indent.depth--;
    }
}

void CodeWriterDump::visitStringLiteralExpression(shared_ptr<const AST::StringLiteralExpression> node)
{
    os() << indent << "StringLiteral: " << string_cast<string>(node->toSourceString()) << "\n";
}

void CodeWriterDump::visitSubExpression(shared_ptr<const AST::SubExpression> node)
{
    dumpExpression(node, "Subtract");
}

void CodeWriterDump::visitTypeArray(shared_ptr<const AST::TypeArray> node)
{
    os() << indent << "TypeArray:\n";
    indent.depth++;
    size_t indexNumber = 0;
    for(AST::TypeArray::IndexRange ir : node->indexRanges())
    {
        os() << indent << "TypeArray." << ++indexNumber << ": ";
        if(AST::TypeArray::isEmptyRange(ir))
            os() << "Any\n";
        else
            os() << std::get<0>(ir) << " To " << std::get<1>(ir) << "\n";
    }
    indent.depth--;
    os() << indent << "TypeArray.elementType:\n";
    indent.depth++;
    node->elementType()->writeCode(*this);
    indent.depth--;
}

void CodeWriterDump::visitTypeBoolean(shared_ptr<const AST::TypeBoolean> node)
{
    os() << indent << "TypeBoolean\n";
}

void CodeWriterDump::visitTypeDouble(shared_ptr<const AST::TypeDouble> node)
{
    os() << indent << "TypeDouble\n";
}

void CodeWriterDump::visitTypeEmpty(shared_ptr<const AST::TypeEmpty> node)
{
    os() << indent << "TypeEmpty\n";
}

void CodeWriterDump::visitTypeInt16(shared_ptr<const AST::TypeInt16> node)
{
    os() << indent << "TypeInt16\n";
}

void CodeWriterDump::visitTypeInt32(shared_ptr<const AST::TypeInt32> node)
{
    os() << indent << "TypeInt32\n";
}

void CodeWriterDump::visitTypeInt64(shared_ptr<const AST::TypeInt64> node)
{
    os() << indent << "TypeInt64\n";
}

void CodeWriterDump::visitTypeInt8(shared_ptr<const AST::TypeInt8> node)
{
    os() << indent << "TypeInt8\n";
}

void CodeWriterDump::visitTypeInteger(shared_ptr<const AST::TypeInteger> node)
{
    os() << indent << "TypeInteger\n";
}

void CodeWriterDump::visitTypeProcedure(shared_ptr<const AST::TypeProcedure> node)
{
    os() << indent << "TypeProcedure: " << string_cast<string>(AST::TypeProcedure::getProcedureTypeString(node->procedureType)) << "\n";
    if(AST::TypeProcedure::getProcedureHasReturnValue(node->procedureType))
    {
        os() << indent << "TypeProcedure.ReturnType:\n";
        indent.depth++;
        node->returnType->writeCode(*this);
        indent.depth--;
    }
    size_t argNumber = 0;
    for(shared_ptr<const AST::Type> arg : node->args)
    {
        os() << indent << "TypeProcedure.Argument" << ++argNumber << "\n";
        indent.depth++;
        arg->writeCode(*this);
        indent.depth--;
    }
}

void CodeWriterDump::visitTypeReference(shared_ptr<const AST::TypeReference> node)
{
    os() << indent << "TypeReference:\n";
    indent.depth++;
    node->toRValue()->writeCode(*this);
    indent.depth--;
}

void CodeWriterDump::visitTypeSingle(shared_ptr<const AST::TypeSingle> node)
{
    os() << indent << "TypeSingle\n";
}

void CodeWriterDump::visitTypeString(shared_ptr<const AST::TypeString> node)
{
    os() << indent << "TypeString\n";
}

void CodeWriterDump::visitTypeUInt16(shared_ptr<const AST::TypeUInt16> node)
{
    os() << indent << "TypeUInt16\n";
}

void CodeWriterDump::visitTypeUInt32(shared_ptr<const AST::TypeUInt32> node)
{
    os() << indent << "TypeUInt32\n";
}

void CodeWriterDump::visitTypeUInt64(shared_ptr<const AST::TypeUInt64> node)
{
    os() << indent << "TypeUInt64\n";
}

void CodeWriterDump::visitTypeUInt8(shared_ptr<const AST::TypeUInt8> node)
{
    os() << indent << "TypeUInt8\n";
}

void CodeWriterDump::visitUnaryPlusExpression(shared_ptr<const AST::UnaryPlusExpression> node)
{
    dumpExpression(node, "UnaryPlus");
}

void CodeWriterDump::visitWhileStatement(shared_ptr<const AST::WhileStatement> node)
{
    string nodeName = "While";
    os() << indent << nodeName << ": " << string_cast<string>(static_cast<wstring>(node->location())) << "\n";
    os() << indent << nodeName << ".condition:\n";
    indent.depth++;
    node->condition->writeCode(*this);
    indent.depth--;
    os() << indent << nodeName << ".body:\n";
    indent.depth++;
    node->body->writeCode(*this);
    indent.depth--;
}

void CodeWriterDump::visitXorExpression(shared_ptr<const AST::XorExpression> node)
{
    dumpExpression(node, "Xor");
}

void CodeWriterDump::finish()
{
    dumpStream = nullptr;
}
