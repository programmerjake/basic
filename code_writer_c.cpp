#include "code_writer_c.h"
#include "string_cast.h"

using namespace std;

void CodeWriterC::visitAndExpression(shared_ptr<const AST::AndExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " & ";
        e->writeCode(*this);
        os() << ")";
    }
}

void CodeWriterC::visitAssignStatement(shared_ptr<const AST::AssignStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "*(";
    node->lhs->writeCode(*this);
    os() << ") = (";
    node->rhs->writeCode(*this);
    os() << ")";
}

void CodeWriterC::visitAutoVariable(shared_ptr<const AST::AutoVariable> node)
{
    if(isDeclaration)
    {
        declarationTypeAfterVariable = "";
        if(!didIndent)
            os() << indent;
        didIndent = true;
        node->type()->toRValue()->writeCode(*this);
        os() << " " << string_cast<string>(createVariableName(node->name())) << declarationTypeAfterVariable << ";\n";
        didIndent = false;
    }
    else
    {
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "&" << string_cast<string>(createVariableName(node->name()));
    }
}

void CodeWriterC::visitBooleanLiteralExpression(shared_ptr<const AST::BooleanLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(node->value)
        os() << "true";
    else
        os() << "false";
}

void CodeWriterC::visitCastExpression(shared_ptr<const AST::CastExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    shared_ptr<AST::Expression> e = node->args().front();
    os() << "(";
    node->type()->toRValue()->writeCode(*this);
    os() << ")";
    if(e->type()->isLValue())
        os() << "*";
    os() << "(";
    e->writeCode(*this);
    os() << ")";
}

void CodeWriterC::visitCodeBlock(shared_ptr<const AST::CodeBlock> node)
{
    bool isOutermostCodeBlock = this->isOutermostCodeBlock;
    this->isOutermostCodeBlock = false;
    if(!isOutermostCodeBlock)
    {
        if(!didIndent)
            os() << indent;
        os() << "{\n";
        didIndent = false;
        indent.depth++;
    }
    for(const AST::Symbol &symbol : *node->symbols)
    {
        switch(symbol.stype())
        {
        case AST::Symbol::Type::AutoVariable:
        case AST::Symbol::Type::ReferenceVariable:
        case AST::Symbol::Type::StaticVariable:
            isDeclaration = true;
            symbol.value->writeCode(*this);
            isDeclaration = false;
            break;
        default:
            throw runtime_error("can't declare symbol ");
        }
    }
    if(isOutermostCodeBlock)
    {
        currentOutputStream = &mainCode;
        indent.depth = 1;
        didIndent = false;
    }
    for(shared_ptr<AST::Statement> statement : node->statements)
    {
        canSkipSemicolon = false;
        statement->writeCode(*this);
        if(!canSkipSemicolon)
        {
            os() << ";\n";
            didIndent = false;
        }
    }
    if(isOutermostCodeBlock)
    {
        currentOutputStream = sourceStream.get();
        indent.depth = 0;
        os() << indent << "int main()\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << mainCode.str();
        os() << indent << "return 0;\n";
        indent.depth--;
        os() << indent << "}\n";
    }
    else
    {
        indent.depth--;
        os() << indent << "}\n";
        didIndent = false;
    }
    canSkipSemicolon = true;
}

void CodeWriterC::visitDoStatement(shared_ptr<const AST::DoStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    switch(node->conditionType)
    {
    case AST::DoStatement::ConditionType::None:
        os() << "for(;;)\n";
        didIndent = false;
        node->body->writeCode(*this);
        break;
    case AST::DoStatement::ConditionType::DoWhile:
        os() << "while(";
        node->condition->writeCode(*this);
        os() << ")\n";
        didIndent = false;
        node->body->writeCode(*this);
        break;
    case AST::DoStatement::ConditionType::DoUntil:
        os() << "while(!(";
        node->condition->writeCode(*this);
        os() << "))\n";
        didIndent = false;
        node->body->writeCode(*this);
        break;
    case AST::DoStatement::ConditionType::LoopWhile:
        os() << "do\n";
        didIndent = false;
        node->body->writeCode(*this);
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "while(";
        node->condition->writeCode(*this);
        os() << ");\n";
        didIndent = false;
        break;
    case AST::DoStatement::ConditionType::LoopUntil:
        os() << "do\n";
        didIndent = false;
        node->body->writeCode(*this);
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "while(!(";
        node->condition->writeCode(*this);
        os() << "));\n";
        didIndent = false;
        break;
    }
}

void CodeWriterC::visitDoubleLiteralExpression(shared_ptr<const AST::DoubleLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    ostringstream ss;
    ss.precision(17);
    ss << node->value;
    os() << ss.str();
}

void CodeWriterC::visitIfStatement(shared_ptr<const AST::IfStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    bool first = true;
    for(pair<shared_ptr<AST::Expression>, shared_ptr<AST::CodeBlock>> ifSection : node->ifSections)
    {
        if(first)
            first = false;
        else
            os() << "\n" << indent << "else ";
        didIndent = true;
        os() << "if(";
        std::get<0>(ifSection)->writeCode(*this);
        os() << ")\n";
        didIndent = false;
        std::get<1>(ifSection)->writeCode(*this);
    }
    if(node->elseSection != nullptr)
    {
        os() << "\n" << indent << "else\n";
        didIndent = false;
        node->elseSection->writeCode(*this);
    }
}

void CodeWriterC::visitIntegerLiteralExpression(shared_ptr<const AST::IntegerLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << string_cast<string>(node->toCString());
}

void CodeWriterC::visitNotExpression(shared_ptr<const AST::NotExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    shared_ptr<AST::Expression> e = node->args().front();
    if(e->type() == AST::TypeBoolean::getInstance())
        os() << "!";
    else
        os() << "~";
    os() << "(";
    e->writeCode(*this);
    os() << ")";
}

void CodeWriterC::visitOrExpression(shared_ptr<const AST::OrExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " | ";
        e->writeCode(*this);
        os() << ")";
    }
}

void CodeWriterC::visitReferenceVariable(shared_ptr<const AST::ReferenceVariable> node)
{
}

void CodeWriterC::visitSingleLiteralExpression(shared_ptr<const AST::SingleLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    ostringstream ss;
    ss.precision(9);
    ss << node->value << "f";
    os() << ss.str();
}

void CodeWriterC::visitStaticVariable(shared_ptr<const AST::StaticVariable> node)
{
}

void CodeWriterC::visitStringLiteralExpression(shared_ptr<const AST::StringLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "wstring(L\"";
    for(size_t i = 0; i < node->value.size(); i++)
    {
        unsigned value = node->value[i];
        if(value >= 0xD800U && value <= 0xDBFFU && i + 1 < node->value.size())
        {
            unsigned nextValue = node->value[i + 1];
            if(nextValue >= 0xDC00U && nextValue <= 0xDFFFU)
            {
                i++;
                value -= 0xD800U;
                nextValue -= 0xDC00U;
                value <<= 10;
                value += nextValue;
            }
        }
        switch(value)
        {
        case '\a':
            os() << "\\a";
            break;
        case '\b':
            os() << "\\b";
            break;
        case '\f':
            os() << "\\f";
            break;
        case '\n':
            os() << "\\n";
            break;
        case '\r':
            os() << "\\r";
            break;
        case '\t':
            os() << "\\t";
            break;
        case '\v':
            os() << "\\v";
            break;
        case '\'':
        case '\"':
        case '\\':
            os() << "\\" << (char)value;
            break;
        default:
            if(value > 0xFFFF)
            {
                ostringstream ss;
                ss.fill('0');
                ss.width(8);
                ss << hex << value;
                os() << "\\U" << ss.str();
            }
            if(value > 0xFF)
            {
                ostringstream ss;
                ss.fill('0');
                ss.width(4);
                ss << hex << value;
                os() << "\\u" << ss.str();
            }
            else if(value < 0x20 || value > 0x7E)
            {
                ostringstream ss;
                ss.fill('0');
                ss.width(2);
                ss << hex << value;
                os() << "\\x" << ss.str();
            }
            else
            {
                os() << (char)value;
            }
        }
    }
    os() << "\")";
}

void CodeWriterC::visitTypeBoolean(shared_ptr<const AST::TypeBoolean> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "bool";
}

void CodeWriterC::visitTypeDouble(shared_ptr<const AST::TypeDouble> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "double";
}

void CodeWriterC::visitTypeInt16(shared_ptr<const AST::TypeInt16> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "int16_t";
}

void CodeWriterC::visitTypeInt32(shared_ptr<const AST::TypeInt32> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "int32_t";
}

void CodeWriterC::visitTypeInt64(shared_ptr<const AST::TypeInt64> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "int64_t";
}

void CodeWriterC::visitTypeInt8(shared_ptr<const AST::TypeInt8> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "int8_t";
}

void CodeWriterC::visitTypeInteger(shared_ptr<const AST::TypeInteger> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "long";
}

void CodeWriterC::visitTypeReference(shared_ptr<const AST::TypeReference> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    node->toRValue()->writeCode(*this);
    os() << " *";
}

void CodeWriterC::visitTypeSingle(shared_ptr<const AST::TypeSingle> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "float";
}

void CodeWriterC::visitTypeString(shared_ptr<const AST::TypeString> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "wstring";
}

void CodeWriterC::visitTypeUInt16(shared_ptr<const AST::TypeUInt16> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "uint16_t";
}

void CodeWriterC::visitTypeUInt32(shared_ptr<const AST::TypeUInt32> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "uint32_t";
}

void CodeWriterC::visitTypeUInt64(shared_ptr<const AST::TypeUInt64> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "uint64_t";
}

void CodeWriterC::visitTypeUInt8(shared_ptr<const AST::TypeUInt8> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "uint8_t";
}

void CodeWriterC::visitWhileStatement(shared_ptr<const AST::WhileStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "while(";
    node->condition->writeCode(*this);
    os() << ")\n";
    didIndent = false;
    node->body->writeCode(*this);
}

void CodeWriterC::visitXorExpression(shared_ptr<const AST::XorExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " ^ ";
        e->writeCode(*this);
        os() << ")";
    }
}

void CodeWriterC::finish()
{
    sourceStream = nullptr;
}
