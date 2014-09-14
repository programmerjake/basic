#include "code_writer_c.h"
#include "string_cast.h"
#include <unordered_set>

using namespace std;

void CodeWriterC::visitAddExpression(shared_ptr<const AST::AddExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " + ";
        e->writeCode(*this);
        os() << ")";
    }
}

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

void CodeWriterC::visitArrayIndexExpression(shared_ptr<const AST::ArrayIndexExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "(";
    assert(node->args().size() >= 2);
    string seperator = "", nextSeperator = ")->at(";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator;
        e->writeCode(*this);
        seperator = nextSeperator;
        nextSeperator = ", ";
    }
    os() << ")";
}

void CodeWriterC::visitAssignStatement(shared_ptr<const AST::AssignStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(AST::BuiltInFunctionExpression::isMidFunction(node->lhs))
    {
        shared_ptr<const AST::BuiltInFunctionExpression> fn = dynamic_pointer_cast<const AST::BuiltInFunctionExpression>(node->lhs);
        switch(fn->fnType())
        {
        case AST::BuiltInFunctionExpression::FnType::Mid2:
            os() << "(";
            fn->args()[0]->writeCode(*this);
            os() << ")->replace((size_t)(";
            fn->args()[1]->writeCode(*this);
            os() << ") - 1, wstring::npos, ";
            node->rhs->writeCode(*this);
            os() << ")";
            break;
        case AST::BuiltInFunctionExpression::FnType::Mid3:
            os() << "(";
            fn->args()[0]->writeCode(*this);
            os() << ")->replace((size_t)(";
            fn->args()[1]->writeCode(*this);
            os() << ") - 1, (size_t)(";
            fn->args()[2]->writeCode(*this);
            os() << "), ";
            node->rhs->writeCode(*this);
            os() << ")";
            break;
        default:
            assert(false);
        }
    }
    else
    {
        os() << "*(";
        node->lhs->writeCode(*this);
        os() << ") = (";
        node->rhs->writeCode(*this);
        os() << ")";
    }
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
        os() << " " << string_cast<string>(createVariableName(node)) << declarationTypeAfterVariable;
        didIndent = false;
    }
    else if(isInitialization)
    {
        declarationTypeAfterVariable = "";
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << string_cast<string>(createVariableName(node));
        os() << " = (";
        isInitialization = false;
        if(node->initializer() != nullptr)
            node->initializer()->writeCode(*this);
        else
            writeInitializer(node);
        isInitialization = true;
        os() << ")";
        didIndent = false;
    }
    else
    {
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "&" << string_cast<string>(createVariableName(node));
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

void CodeWriterC::visitBuiltInFunctionExpression(shared_ptr<const AST::BuiltInFunctionExpression> node)
{
    switch(node->fnType())
    {
    case AST::BuiltInFunctionExpression::FnType::Abs:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::abs(";
        node->args().front()->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Asc:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_Asc(";
        node->args().front()->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::ATn1:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::atan(";
        node->args().front()->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::ATn2:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::atan2(";
        node->args()[0]->writeCode(*this);
        os() << ", ";
        node->args()[1]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Chr:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "wstring(1, (wchar_t)(";
        node->args().front()->writeCode(*this);
        os() << "))";
        return;
    case AST::BuiltInFunctionExpression::FnType::Cos:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::cos(";
        node->args().front()->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Exp:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::exp(";
        node->args().front()->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Hex:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_Hex((uint64_t)(int64_t)(";
        node->args().front()->writeCode(*this);
        os() << "))";
        return;
    case AST::BuiltInFunctionExpression::FnType::InStr2:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_InStr(";
        node->args()[0]->writeCode(*this);
        os() << ", ";
        node->args()[1]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::InStr3:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_InStr(";
        node->args()[0]->writeCode(*this);
        os() << ", ";
        node->args()[1]->writeCode(*this);
        os() << ", ";
        node->args()[2]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::LBound1:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "(";
        node->args()[0]->writeCode(*this);
        os() << ").lbound(1L)";
        return;
    case AST::BuiltInFunctionExpression::FnType::LBound2:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "(";
        node->args()[0]->writeCode(*this);
        os() << ").lbound(";
        node->args()[1]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::LCase:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_LCase(";
        node->args().front()->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Left:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "(";
        node->args()[0]->writeCode(*this);
        os() << ").substr(0, (size_t)(";
        node->args()[1]->writeCode(*this);
        os() << "))";
        return;
    case AST::BuiltInFunctionExpression::FnType::Len:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "(";
        node->args()[0]->writeCode(*this);
        os() << ").size()";
        return;
    case AST::BuiltInFunctionExpression::FnType::Log:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::log(";
        node->args().front()->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::LTrim:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_LTrim(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Mid2:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "(";
        node->args()[0]->writeCode(*this);
        if(node->args()[0]->type()->isLValue())
            os() << ")->substr((size_t)(";
        else
            os() << ").substr((size_t)(";
        node->args()[1]->writeCode(*this);
        os() << ") - 1)";
        return;
    case AST::BuiltInFunctionExpression::FnType::Mid3:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "(";
        node->args()[0]->writeCode(*this);
        if(node->args()[0]->type()->isLValue())
            os() << ")->substr((size_t)(";
        else
            os() << ").substr((size_t)(";
        node->args()[1]->writeCode(*this);
        os() << ") - 1, (size_t)(";
        node->args()[2]->writeCode(*this);
        os() << "))";
        return;
    case AST::BuiltInFunctionExpression::FnType::Oct:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_Oct((uint64_t)(int64_t)(";
        node->args().front()->writeCode(*this);
        os() << "))";
        return;
    case AST::BuiltInFunctionExpression::FnType::Right:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_Right(";
        node->args()[0]->writeCode(*this);
        os() << ", ";
        node->args()[1]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::RTrim:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_RTrim(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Sgn:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_Sgn(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Sin:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::sin(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Space:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "wstring((size_t)(";
        node->args()[0]->writeCode(*this);
        os() << "), L' ')";
        return;
    case AST::BuiltInFunctionExpression::FnType::Sqr:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::sqrt(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Str:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_Str(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::String:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "wstring((size_t)(";
        node->args()[0]->writeCode(*this);
        os() << "), (wchar_t)(";
        node->args()[0]->writeCode(*this);
        os() << "))";
        return;
    case AST::BuiltInFunctionExpression::FnType::Tan:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "std::tan(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::UBound1:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "(";
        node->args()[0]->writeCode(*this);
        os() << ").ubound(1L)";
        return;
    case AST::BuiltInFunctionExpression::FnType::UBound2:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "(";
        node->args()[0]->writeCode(*this);
        os() << ").ubound(";
        node->args()[1]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::UCase:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_UCase(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Val:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_Val(";
        node->args()[0]->writeCode(*this);
        os() << ")";
        return;
    }
    assert(false);
}

void CodeWriterC::visitCallExpression(shared_ptr<const AST::CallExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "(*(";
    assert(node->args().size() >= 1);
    node->args()[0]->writeCode(*this);
    os() << "))(";
    for(size_t i = 1; i < node->args().size(); i++)
    {
        if(i > 1)
            os() << ", ";
        node->args()[i]->writeCode(*this);
    }
    os() << ")";
}

void CodeWriterC::visitCastExpression(shared_ptr<const AST::CastExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    shared_ptr<AST::Expression> e = node->args().front();
    if(*node->type() != *e->type() && *node->type() != *e->type()->toRValue())
    {
        os() << "(";
        node->type()->writeCode(*this);
        os() << ")";
    }
    if(e->type()->isLValue() && !node->type()->isLValue())
        os() << "*";
    os() << "(";
    e->writeCode(*this);
    os() << ")";
}

void CodeWriterC::visitCodeBlock(shared_ptr<const AST::CodeBlock> node)
{
    bool isOutermostCodeBlock = this->isOutermostCodeBlock;
    this->isOutermostCodeBlock = false;
    if(isOutermostCodeBlock)
    {
        indent.depth = 0;
        os() << indent << "#include <cwchar>\n";
        os() << indent << "#include <cwctype>\n";
        os() << indent << "#include <cmath>\n";
        os() << indent << "#include <string>\n";
        os() << indent << "#include <sstream>\n";
        os() << indent << "#include <cstdint>\n";
        os() << indent << "#include <cassert>\n";
        os() << indent << "#include <tuple>\n";
        os() << indent << "#include <memory>\n";
        os() << indent << "\n";
        os() << indent << "using namespace std;\n";
        os() << indent << "\n";
        os() << indent << "inline long fn_Asc(wstring str)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return (str.empty() ? -1 : (long)str[0]);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_Hex(uint64_t v)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "wstring retval = L\"\";\n";
        os() << indent << "do\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "long digit = (int)(v % 0x10);\n";
        os() << indent << "v /= 0x10;\n";
        os() << indent << "retval = (wchar_t)(digit <= 9 ? '0' + digit : 'A' + digit - 0xA) + retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "while(v > 0);\n";
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_Oct(uint64_t v)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "wstring retval = L\"\";\n";
        os() << indent << "do\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "long digit = (int)(v % 8);\n";
        os() << indent << "v /= 8;\n";
        os() << indent << "retval = (wchar_t)('0' + digit) + retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "while(v > 0);\n";
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_Str(uint64_t v)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "wstring retval = L\"\";\n";
        os() << indent << "do\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "long digit = (int)(v % 10);\n";
        os() << indent << "v /= 10;\n";
        os() << indent << "retval = (wchar_t)('0' + digit) + retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "while(v > 0);\n";
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_Str(int64_t v)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return v < 0 ? L\"-\" + fn_Str((uint64_t)-v) : fn_Str((uint64_t)v);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_Str(double v)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "wostringstream ss;\n";
        os() << indent << "ss << uppercase << v;\n";
        os() << indent << "return ss.str();\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_LTrim(wstring str)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "size_t pos = str.find_first_not_of(L\" \");\n";
        os() << indent << "if(pos == wstring::npos)\n";
        indent.depth++;
        os() << indent << "return L\"\";\n";
        indent.depth--;
        os() << indent << "return str.substr(pos);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_RTrim(wstring str)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "size_t pos = str.find_last_not_of(L\" \");\n";
        os() << indent << "if(pos == wstring::npos)\n";
        indent.depth++;
        os() << indent << "return L\"\";\n";
        indent.depth--;
        os() << indent << "return str.substr(0, pos + 1);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_Right(wstring str, long length)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "if(length <= 0)\n";
        indent.depth++;
        os() << indent << "return L\"\";\n";
        indent.depth--;
        os() << indent << "if((size_t)length >= str.size())\n";
        indent.depth++;
        os() << indent << "return str;\n";
        indent.depth--;
        os() << indent << "return str.substr(str.size() - length);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline long fn_Sgn(int64_t v)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "if(v < 0)\n";
        indent.depth++;
        os() << indent << "return -1;\n";
        indent.depth--;
        os() << indent << "if(v > 0)\n";
        indent.depth++;
        os() << indent << "return 1;\n";
        indent.depth--;
        os() << indent << "return 0;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline long fn_Sgn(uint64_t v)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "if(v > 0)\n";
        indent.depth++;
        os() << indent << "return 1;\n";
        indent.depth--;
        os() << indent << "return 0;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline long fn_Sgn(double v)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "if(v < 0)\n";
        indent.depth++;
        os() << indent << "return -1;\n";
        indent.depth--;
        os() << indent << "if(v > 0)\n";
        indent.depth++;
        os() << indent << "return 1;\n";
        indent.depth--;
        os() << indent << "return 0;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline long fn_InStr(wstring a, wstring b, long start = 1)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "size_t retval = a.find(b, (size_t)(start - 1));\n";
        os() << indent << "if(retval == wstring::npos)\n";
        indent.depth++;
        os() << indent << "return 0;\n";
        indent.depth--;
        os() << indent << "return retval + 1;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_UCase(wstring str)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "wstring retval;\n";
        os() << indent << "retval.reserve(str.size());\n";
        os() << indent << "for(size_t i = 0; i < str.size(); i++)\n";
        indent.depth++;
        os() << indent << "retval += (wchar_t)std::towupper(str[i]);\n";
        indent.depth--;
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline wstring fn_LCase(wstring str)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "wstring retval;\n";
        os() << indent << "retval.reserve(str.size());\n";
        os() << indent << "for(size_t i = 0; i < str.size(); i++)\n";
        indent.depth++;
        os() << indent << "retval += (wchar_t)std::towlower(str[i]);\n";
        indent.depth--;
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline bool iswodigit(wint_t ch)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return (ch >= '0' && ch <= '7');\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "inline double fn_Val(wstring svalue)\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "if(svalue.empty())\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "size_t stringIndex = 0;\n";
        os() << indent << "int peekChar = svalue[stringIndex++];\n";
        os() << indent << "auto getChar = [&]()->int\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "int retval = peekChar;\n";
        os() << indent << "if(stringIndex >= svalue.size())\n";
        indent.depth++;
        os() << indent << "peekChar = EOF;\n";
        indent.depth--;
        os() << indent << "else\n";
        indent.depth++;
        os() << indent << "peekChar = svalue[stringIndex++];\n";
        indent.depth--;
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "};\n";
        os() << indent << "double retval = 0;\n";
        os() << indent << "if(peekChar == '&')\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "getChar();\n";
        os() << indent << "if(peekChar == 'h' || peekChar == 'H')\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "getChar();\n";
        os() << indent << "if(!std::iswxdigit(peekChar))\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "while(std::iswxdigit(peekChar))\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "int digit;\n";
        os() << indent << "if(std::iswdigit(peekChar))\n";
        indent.depth++;
        os() << indent << "digit = peekChar - '0';\n";
        indent.depth--;
        os() << indent << "else if(std::iswlower(peekChar))\n";
        indent.depth++;
        os() << indent << "digit = peekChar - 'a' + 0xA;\n";
        indent.depth--;
        os() << indent << "else\n";
        indent.depth++;
        os() << indent << "digit = peekChar - 'A' + 0xA;\n";
        indent.depth--;
        os() << indent << "retval *= 0x10;\n";
        os() << indent << "retval += digit;\n";
        os() << indent << "getChar();\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "if(peekChar == '_')\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "if(peekChar != EOF)\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "else if(iswodigit(peekChar) || peekChar == 'o' || peekChar == 'O')\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "if(peekChar == 'o' || peekChar == 'O')\n";
        indent.depth++;
        os() << indent << "getChar();\n";
        indent.depth--;
        os() << indent << "if(!iswodigit(peekChar))\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "while(iswodigit(peekChar))\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "int digit = peekChar - '0';\n";
        os() << indent << "retval *= 8;\n";
        os() << indent << "retval += digit;\n";
        os() << indent << "getChar();\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "if(peekChar != EOF)\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "bool gotAnyIntegerDigits = false;\n";
        os() << indent << "while(std::iswdigit(peekChar))\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "int digit = getChar() - '0';\n";
        os() << indent << "retval *= 10;\n";
        os() << indent << "retval += digit;\n";
        os() << indent << "gotAnyIntegerDigits = true;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "if(peekChar == '.')\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "getChar();\n";
        os() << indent << "double factor = 1;\n";
        os() << indent << "if(!gotAnyIntegerDigits && !std::iswdigit(peekChar))\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "while(std::iswdigit(peekChar))\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "int digit = getChar() - '0';\n";
        os() << indent << "factor *= 0.1;\n";
        os() << indent << "retval += digit * factor;\n";
        indent.depth--;
        os() << indent << "}\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "if(peekChar == 'e' || peekChar == 'E' || peekChar == 'f' || peekChar == 'F')\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "getChar();\n";
        os() << indent << "bool isExponentNegative = false;\n";
        os() << indent << "if(peekChar == '+' || peekChar == '-')\n";
        indent.depth++;
        os() << indent << "isExponentNegative = (getChar() == '-');\n";
        indent.depth--;
        os() << indent << "if(!std::iswdigit(peekChar))\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "int exponent = 0;\n";
        os() << indent << "while(std::iswdigit(peekChar))\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "int digit = getChar() - '0';\n";
        os() << indent << "if(exponent >= 10000000)\n";
        indent.depth++;
        os() << indent << "continue;\n";
        indent.depth--;
        os() << indent << "exponent *= 10;\n";
        os() << indent << "exponent += digit;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "if(isExponentNegative)\n";
        indent.depth++;
        os() << indent << "exponent = -exponent;\n";
        indent.depth--;
        os() << indent << "retval *= std::pow(10.0L, exponent);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "if(peekChar != EOF)\n";
        indent.depth++;
        os() << indent << "return NAN;\n";
        indent.depth--;
        os() << indent << "return retval;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        os() << indent << "template <typename T, size_t N = 1>\n";
        os() << indent << "struct ArrayDescriptor final\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "T *elements;\n";
        os() << indent << "long base[N];\n";
        os() << indent << "size_t size[N];\n";
        os() << indent << "template <typename ...Args>\n";
        os() << indent << "ArrayDescriptor(T *elements, Args... args)\n";
        indent.depth++;
        os() << indent << ": ArrayDescriptor(args...)\n";
        indent.depth--;
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "static_assert(sizeof...(args) == 2 * N, \"array index count doesn't match\");\n";
        os() << indent << "this->elements = elements;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "~ArrayDescriptor()\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "delete []elements;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "ArrayDescriptor(const ArrayDescriptor &) = delete;\n";
        os() << indent << "void operator =(const ArrayDescriptor &) = delete;\n";
        indent.depth--;
        os() << indent << "private:\n";
        indent.depth++;
        os() << indent << "template <typename ...Args>\n";
        os() << indent << "ArrayDescriptor(long base, size_t size, Args... args)\n";
        indent.depth++;
        os() << indent << ": ArrayDescriptor(args...)\n";
        indent.depth--;
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "constexpr size_t indexNumber = N - (sizeof...(args) / 2 + 1);\n";
        os() << indent << "this->base[indexNumber] = base;\n";
        os() << indent << "this->size[indexNumber] = size;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "ArrayDescriptor()\n";
        os() << indent << "{\n";
        os() << indent << "}\n";
        os() << indent << "constexpr size_t arrayIndexSize(size_t indexNumber) const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return (indexNumber + 1 < N) ? arrayIndexSize(indexNumber + 1) * size[indexNumber] : (indexNumber < N ? size[indexNumber] : 1);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "template <typename ...Args>\n";
        os() << indent << "inline T *atHelper(long index, Args... args) const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "T *ptr = atHelper(args...);\n";
        os() << indent << "constexpr size_t indexNumber = N - (sizeof...(args) + 1);\n";
        os() << indent << "assert((size_t)(index - base[indexNumber]) < size[indexNumber]);\n";
        os() << indent << "return ptr + (arrayIndexSize(indexNumber) * (index - base[indexNumber]));\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "inline T * atHelper() const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return elements;\n";
        indent.depth--;
        os() << indent << "}\n";
        indent.depth--;
        os() << indent << "public:\n";
        indent.depth++;
        os() << indent << "template <typename ...Args>\n";
        os() << indent << "inline T *at(Args... args) const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "static_assert(sizeof...(args) == N, \"incorrect argument count for array\");\n";
        os() << indent << "return atHelper(args...);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "inline long lbound(long index) const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "assert(index >= 1 && index <= (long)N);\n";
        os() << indent << "return base[index - 1];\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "inline long ubound(long index) const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "assert(index >= 1 && index <= (long)N);\n";
        os() << indent << "return base[index - 1] + size[index - 1] - 1;\n";
        indent.depth--;
        os() << indent << "}\n";
        indent.depth--;
        os() << indent << "};\n";
        os() << indent << "\n";
        os() << indent << "\n";
    }
    else
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
        case AST::Symbol::Type::Procedure:
            break;
        case AST::Symbol::Type::Type:
        {
            shared_ptr<const AST::TypeType> type = dynamic_pointer_cast<const AST::TypeType>(symbol.value);
            assert(type != nullptr);
            if(!didIndent)
                os() << indent;
            didIndent = true;
            os() << "struct " << string_cast<string>(createStructureName(type)) << ";\n";
            didIndent = false;
            isDeclaration = false;
            break;
        }
        default:
            throw runtime_error("can't declare symbol " + string_cast<string>(symbol.name()));
        }
    }
    for(const AST::Symbol &symbol : *node->symbols)
    {
        switch(symbol.stype())
        {
        case AST::Symbol::Type::AutoVariable:
        case AST::Symbol::Type::ReferenceVariable:
        case AST::Symbol::Type::StaticVariable:
        case AST::Symbol::Type::Procedure:
            break;
        case AST::Symbol::Type::Type:
            isImplementation = true;
            symbol.value->writeCode(*this);
            didIndent = false;
            isImplementation = false;
            break;
        default:
            throw runtime_error("can't declare symbol " + string_cast<string>(symbol.name()));
        }
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
            os() << ";\n";
            didIndent = false;
            isDeclaration = false;
            break;
        case AST::Symbol::Type::Procedure:
            if(std::get<1>(procedureSet.insert(symbol.value)))
            {
                procedureList.push_back(symbol.value);
                newProcedures.push_back(symbol.value);
            }
            break;
        case AST::Symbol::Type::Type:
            break;
        default:
            throw runtime_error("can't declare symbol " + string_cast<string>(symbol.name()));
        }
    }
    if(isOutermostCodeBlock)
    {
        while(!newProcedures.empty())
        {
            isDeclaration = true;
            newProcedures.front()->writeCode(*this);
            newProcedures.pop_front();
            os() << ";\n";
            didIndent = false;
            isDeclaration = false;
        }
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
        while(!newProcedures.empty())
        {
            isDeclaration = true;
            newProcedures.front()->writeCode(*this);
            newProcedures.pop_front();
            os() << ";\n";
            didIndent = false;
            isDeclaration = false;
        }
        currentOutputStream = sourceStream.get();
        indent.depth = 0;
        os() << indent << "int main()\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << mainCode.str();
        os() << indent << "return 0;\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "\n";
        for(size_t i = 0; i < procedureList.size(); i++)
        {
            while(!newProcedures.empty())
            {
                isDeclaration = true;
                newProcedures.front()->writeCode(*this);
                newProcedures.pop_front();
                os() << ";\n";
                didIndent = false;
                isDeclaration = false;
            }
            isImplementation = true;
            canSkipSemicolon = false;
            procedureList[i]->writeCode(*this);
            if(!canSkipSemicolon)
            {
                os() << ";\n";
            }
            os() << indent << "\n";
            didIndent = false;
            isImplementation = false;
        }
    }
    else
    {
        indent.depth--;
        os() << indent << "}\n";
        didIndent = false;
    }
    canSkipSemicolon = true;
    this->isOutermostCodeBlock = isOutermostCodeBlock;
}

void CodeWriterC::visitCompareExpression(shared_ptr<const AST::CompareExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "", op;
    switch(node->ctype())
    {
    case AST::CompareExpression::CType::Eq:
        op = "==";
        break;
    case AST::CompareExpression::CType::NE:
        op = "!=";
        break;
    case AST::CompareExpression::CType::GT:
        op = ">";
        break;
    case AST::CompareExpression::CType::LT:
        op = "<";
        break;
    case AST::CompareExpression::CType::GE:
        op = ">=";
        break;
    case AST::CompareExpression::CType::LE:
        op = "<=";
        break;
    }
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " " + op + " ";
        e->writeCode(*this);
        os() << ")";
    }
}

void CodeWriterC::visitDoStatement(shared_ptr<const AST::DoStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string exitLabel = makeLabel();
    doStatementExitLabelMap[node] = exitLabel;
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
    os() << indent << exitLabel << ": ;\n";
}

void CodeWriterC::visitDoubleLiteralExpression(shared_ptr<const AST::DoubleLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    ostringstream ss;
    ss.precision(17);
    ss << showpoint << node->value;
    string str = ss.str();
    size_t exponentLocation = str.find_last_of("eE");
    size_t periodLocation = str.find('.');
    if(exponentLocation != 0 && periodLocation != string::npos)
    {
        size_t lastNonzeroLocation = str.find_last_not_of('0', exponentLocation == string::npos ? string::npos : exponentLocation - 1);
        if(lastNonzeroLocation != string::npos && lastNonzeroLocation >= periodLocation)
        {
            if(exponentLocation == string::npos)
                str.erase(lastNonzeroLocation + 1);
            else
                str.erase(lastNonzeroLocation + 1, exponentLocation - lastNonzeroLocation - 1);
        }
    }
    os() << str;
}

void CodeWriterC::visitExitDoStatement(shared_ptr<const AST::ExitDoStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "goto " << doStatementExitLabelMap[node->statement];
}

void CodeWriterC::visitExitForStatement(shared_ptr<const AST::ExitForStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "goto " << forStatementExitLabelMap[node->statement];
}

void CodeWriterC::visitExitWhileStatement(shared_ptr<const AST::ExitWhileStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "goto " << whileStatementExitLabelMap[node->statement];
}

void CodeWriterC::visitExitProcedureStatement(shared_ptr<const AST::ExitProcedureStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(AST::TypeProcedure::getProcedureHasReturnValue(node->procedure->type()->procedureType))
        os() << "return " << string_cast<string>(createVariableName(node->procedure->returnValue));
    else
        os() << "return";
}

void CodeWriterC::visitFDivExpression(shared_ptr<const AST::FDivExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " / ";
        e->writeCode(*this);
        os() << ")";
    }
}

void CodeWriterC::visitForStatement(shared_ptr<const AST::ForStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string exitLabel = makeLabel();
    forStatementExitLabelMap[node] = exitLabel;
    string variableRefTemp = makeTempVariable();
    string endTemp = makeTempVariable();
    string stepTemp = makeTempVariable();
    string isFirstTemp = makeTempVariable();
    declarationTypeAfterVariable = "";
    node->variable->type()->writeCode(*this);
    os() << " " << variableRefTemp << declarationTypeAfterVariable << ";\n" << indent;
    didIndent = true;
    declarationTypeAfterVariable = "";
    node->end->type()->writeCode(*this);
    os() << " " << endTemp << declarationTypeAfterVariable << ";\n" << indent;
    didIndent = true;
    declarationTypeAfterVariable = "";
    node->step->type()->writeCode(*this);
    os() << " " << stepTemp << declarationTypeAfterVariable << ";\n";
    didIndent = true;
    os() << indent << "bool " << isFirstTemp << " = true;\n";
    os() << indent << variableRefTemp << " = (";
    node->variable->writeCode(*this);
    os() << ");\n";
    didIndent = true;
    os() << indent << "*" << variableRefTemp << " = (";
    node->start->writeCode(*this);
    os() << ");\n";
    os() << indent << "for(;;)\n";
    os() << indent << "{\n";
    indent.depth++;
    didIndent = true;
    os() << indent << endTemp << " = (";
    node->end->writeCode(*this);
    os() << ");\n";
    didIndent = true;
    os() << indent << stepTemp << " = (";
    node->step->writeCode(*this);
    os() << ");\n";
    os() << indent << "if(" << isFirstTemp << ")\n";
    indent.depth++;
    os() << indent << isFirstTemp << " = false;\n";
    indent.depth--;
    os() << indent << "else\n";
    indent.depth++;
    os() << indent << "*" << variableRefTemp << " += " << stepTemp << ";\n";
    indent.depth--;
    os() << indent << "if(" << stepTemp << " < 0 && *" << variableRefTemp << " < " << endTemp << ")\n";
    indent.depth++;
    os() << indent << "break;\n";
    indent.depth--;
    os() << indent << "if(" << stepTemp << " > 0 && *" << variableRefTemp << " > " << endTemp << ")\n";
    indent.depth++;
    os() << indent << "break;\n";
    indent.depth--;
    didIndent = false;
    node->body->writeCode(*this);
    indent.depth--;
    os() << indent << "}\n";
    os() << indent << exitLabel << ": ;\n";
    didIndent = false;
    canSkipSemicolon = true;
}

void CodeWriterC::visitIDivExpression(shared_ptr<const AST::IDivExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " / ";
        e->writeCode(*this);
        os() << ")";
    }
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

void CodeWriterC::visitInitializeStatement(shared_ptr<const AST::InitializeStatement> node)
{
    isInitialization = true;
    node->variable->writeCode(*this);
    isInitialization = false;
}

void CodeWriterC::visitIntegerLiteralExpression(shared_ptr<const AST::IntegerLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << string_cast<string>(node->toCString());
}

void CodeWriterC::visitMemberAccessExpression(shared_ptr<const AST::MemberAccessExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    AST::Symbol symbol = AST::MemberAccessExpression::getMember(node->location(), node->args().front()->type(), node->memberName);
    switch(symbol.stype())
    {
    case AST::Symbol::Type::AutoVariable:
    case AST::Symbol::Type::StaticVariable:
    {
        shared_ptr<AST::Variable> variable = dynamic_pointer_cast<AST::Variable>(symbol.value);
        os() << "&(";
        node->args().front()->writeCode(*this);
        os() << ")";
        if(node->args().front()->type()->isLValue())
            os() << "->";
        else
            os() << ".";
        os() << string_cast<string>(createVariableName(variable));
        break;
    }
    case AST::Symbol::Type::ReferenceVariable:
    {
        shared_ptr<AST::Variable> variable = dynamic_pointer_cast<AST::Variable>(symbol.value);
        os() << "(";
        node->args().front()->writeCode(*this);
        os() << ")";
        if(node->args().front()->type()->isLValue())
            os() << "->";
        else
            os() << ".";
        os() << string_cast<string>(createVariableName(variable));
        break;
    }
    case AST::Symbol::Type::Procedure:
    {
        throw logic_error("implement variable.procedure");
    }
    default:
        assert(false);
    }
}

void CodeWriterC::visitModExpression(shared_ptr<const AST::ModExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " % ";
        e->writeCode(*this);
        os() << ")";
    }
}

void CodeWriterC::visitMulExpression(shared_ptr<const AST::MulExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " * ";
        e->writeCode(*this);
        os() << ")";
    }
}

void CodeWriterC::visitNegExpression(shared_ptr<const AST::NegExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    shared_ptr<AST::Expression> e = node->args().front();
    os() << "-(";
    e->writeCode(*this);
    os() << ")";
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

void CodeWriterC::visitProcedure(shared_ptr<const AST::Procedure> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isImplementation)
    {
        isDeclaration = true;
        declarationTypeAfterVariable = "";
        functionArguments = &node->args();
        node->type()->writeCode(*this);
        functionArguments = nullptr;
        isImplementation = false;
        os() << string_cast<string>(createFunctionName(node)) << declarationTypeAfterVariable;
        isDeclaration = false;
        if(node->code != nullptr)
        {
            os() << "\n";
            os() << indent << "{\n";
            didIndent = false;
            indent.depth++;
            if(AST::TypeProcedure::getProcedureHasReturnValue(node->type()->procedureType))
            {
                isDeclaration = true;
                canSkipSemicolon = false;
                node->returnValue->writeCode(*this);
                if(!canSkipSemicolon)
                {
                    os() << ";\n";
                    didIndent = false;
                }
                isDeclaration = false;
                isInitialization = true;
                canSkipSemicolon = false;
                node->returnValue->writeCode(*this);
                if(!canSkipSemicolon)
                {
                    os() << ";\n";
                    didIndent = false;
                }
                isInitialization = false;
            }
            node->code->writeCode(*this);
            if(AST::TypeProcedure::getProcedureHasReturnValue(node->type()->procedureType))
            {
                os() << indent << "return " << string_cast<string>(createVariableName(node->returnValue)) << ";\n";
            }
            indent.depth--;
            os() << indent << "}\n";
            canSkipSemicolon = true;
        }
        else
        {
            os() << ";\n";
            canSkipSemicolon = true;
            didIndent = false;
        }
        isImplementation = true;
    }
    else if(isDeclaration)
    {
        isImplementation = true;
        declarationTypeAfterVariable = "";
        functionArguments = &node->args();
        node->type()->writeCode(*this);
        functionArguments = nullptr;
        os() << string_cast<string>(createFunctionName(node)) << declarationTypeAfterVariable;
        isImplementation = false;
    }
    else
    {
        os() << "(&" << string_cast<string>(createFunctionName(node)) << ")";
    }
}

void CodeWriterC::visitReferenceVariable(shared_ptr<const AST::ReferenceVariable> node)
{
    if(isDeclaration)
    {
        declarationTypeAfterVariable = "";
        if(!didIndent)
            os() << indent;
        didIndent = true;
        node->type()->writeCode(*this);
        os() << " " << string_cast<string>(createVariableName(node)) << declarationTypeAfterVariable;
        didIndent = false;
    }
    else if(isInitialization)
    {
        if(!didIndent)
            os() << indent;
        didIndent = true;
        if(node->initializer() != nullptr)
        {
            os() << string_cast<string>(createVariableName(node));
            os() << " = (";
            isInitialization = false;
            node->initializer()->writeCode(*this);
            isInitialization = true;
            os() << ")";
        }
        didIndent = false;
    }
    else
    {
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << string_cast<string>(createVariableName(node));
    }
}

void CodeWriterC::visitSingleLiteralExpression(shared_ptr<const AST::SingleLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    ostringstream ss;
    ss.precision(9);
    ss << showpoint << node->value;
    string str = ss.str();
    size_t exponentLocation = str.find_last_of("eE");
    size_t periodLocation = str.find('.');
    if(exponentLocation != 0 && periodLocation != string::npos)
    {
        size_t lastNonzeroLocation = str.find_last_not_of('0', exponentLocation == string::npos ? string::npos : exponentLocation - 1);
        if(lastNonzeroLocation != string::npos && lastNonzeroLocation >= periodLocation)
        {
            if(exponentLocation == string::npos)
                str.erase(lastNonzeroLocation + 1);
            else
                str.erase(lastNonzeroLocation + 1, exponentLocation - lastNonzeroLocation - 1);
        }
    }
    os() << str << "f";
}

void CodeWriterC::visitStaticVariable(shared_ptr<const AST::StaticVariable> node)
{
    if(isDeclaration)
    {
        declarationTypeAfterVariable = "";
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "static ";
        node->type()->toRValue()->writeCode(*this);
        os() << " " << string_cast<string>(createVariableName(node)) << declarationTypeAfterVariable;
        os() << " = (";
        isDeclaration = false;
        if(node->initializer() != nullptr)
            node->initializer()->writeCode(*this);
        else
            writeInitializer(node);
        isDeclaration = true;
        os() << ")";
        didIndent = false;
    }
    else
    {
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "&" << string_cast<string>(createVariableName(node));
    }
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
                ss << hex << uppercase << value;
                os() << "\\U" << ss.str();
            }
            if(value > 0xFF)
            {
                ostringstream ss;
                ss.fill('0');
                ss.width(4);
                ss << hex << uppercase << value;
                os() << "\\u" << ss.str();
            }
            else if(value < 0x20 || value > 0x7E)
            {
                ostringstream ss;
                ss.fill('0');
                ss.width(2);
                ss << hex << uppercase << value;
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

void CodeWriterC::visitSubExpression(shared_ptr<const AST::SubExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string seperator = "";
    for(shared_ptr<AST::Expression> e : node->args())
    {
        os() << seperator << "(";
        seperator = " - ";
        e->writeCode(*this);
        os() << ")";
    }
}

void CodeWriterC::visitTypeArray(shared_ptr<const AST::TypeArray> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
    {
        isInitializer = false;
        os() << "shared_ptr<const ArrayDescriptor<";
        node->elementType()->writeCode(*this);
        os() << ", " << node->indexRanges().size() << "> >(";
        if(node->isTotallySpecified())
        {
            os() << "new ArrayDescriptor<";
            node->elementType()->writeCode(*this);
            os() << ", " << node->indexRanges().size() << ">(";
            int64_t size = node->elementCount();
            if(size <= 0)
            {
                os() << "nullptr";
            }
            else
            {
                declarationTypeAfterVariable = "";
                isDeclaration = true;
                os() << "new (";
                node->elementType()->writeCode(*this);
                isDeclaration = false;
                os() << "[" << size << "]" << declarationTypeAfterVariable << ")";
            }
            for(AST::TypeArray::IndexRange ir : node->indexRanges())
            {
                os() << ", " << std::get<0>(ir) << ", " << AST::TypeArray::getRangeSize(ir);
            }
            os() << "))";
        }
        else
        {
            os() << "nullptr)";
        }
        isInitializer = true;
        return;
    }
    os() << "shared_ptr<const ArrayDescriptor<";
    node->elementType()->writeCode(*this);
    os() << ", " << node->indexRanges().size() << "> >";
}

void CodeWriterC::visitTypeBoolean(shared_ptr<const AST::TypeBoolean> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "false";
    else
        os() << "bool";
}

void CodeWriterC::visitTypeDouble(shared_ptr<const AST::TypeDouble> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "0.0";
    else
        os() << "double";
}

void CodeWriterC::visitTypeEmpty(shared_ptr<const AST::TypeEmpty> node)
{
    assert(false);
}

void CodeWriterC::visitTypeInt16(shared_ptr<const AST::TypeInt16> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "(int16_t)0";
    else
        os() << "int16_t";
}

void CodeWriterC::visitTypeInt32(shared_ptr<const AST::TypeInt32> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "(int32_t)0";
    else
        os() << "int32_t";
}

void CodeWriterC::visitTypeInt64(shared_ptr<const AST::TypeInt64> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "(int64_t)0";
    else
        os() << "int64_t";
}

void CodeWriterC::visitTypeInt8(shared_ptr<const AST::TypeInt8> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "(int8_t)0";
    else
        os() << "int8_t";
}

void CodeWriterC::visitTypeProcedure(shared_ptr<const AST::TypeProcedure> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
    {
        isInitializer = false;
        os() << "(";
        visitTypeProcedure(node);
        isInitializer = true;
        os() << ")nullptr";
        return;
    }
    if(!isDeclaration)
    {
        declarationTypeAfterVariable = "";
        isDeclaration = true;
        visitTypeProcedure(node);
        os() << declarationTypeAfterVariable;
        isDeclaration = false;
        return;
    }
    string outsideDeclarationTypeAfterVariable = declarationTypeAfterVariable;
    declarationTypeAfterVariable = "";
    switch(node->procedureType)
    {
    case AST::TypeProcedure::ProcedureType::Sub:
        os() << "void ";
        break;
    case AST::TypeProcedure::ProcedureType::Function:
    {
        bool temp = isImplementation;
        isImplementation = false;
        node->returnType->writeCode(*this);
        isImplementation = temp;
        os() << " ";
        break;
    }
    }
    if(!isImplementation)
        os() << "(*";
    ostream *outsideStream = currentOutputStream;
    ostringstream argumentStream;
    currentOutputStream = &argumentStream;
    os() << outsideDeclarationTypeAfterVariable;
    if(!isImplementation)
        os() << ")";
    os() << "(";
    string seperator = "";
    string temp = declarationTypeAfterVariable;
    size_t index = 0;
    for(shared_ptr<const AST::Type> arg : node->args)
    {
        bool temp = isImplementation;
        isImplementation = false;
        os() << seperator;
        seperator = ", ";
        declarationTypeAfterVariable = "";
        arg->writeCode(*this);
        isImplementation = temp;
        isDeclaration = true;
        if(isImplementation && (index < functionArguments->size() && functionArguments->at(index) != nullptr))
        {
            os() << " " << string_cast<string>(createVariableName(functionArguments->at(index)));
        }
        os() << declarationTypeAfterVariable;
        index++;

    }
    os() << ")" << temp;
    currentOutputStream = outsideStream;
    declarationTypeAfterVariable = argumentStream.str();
}

void CodeWriterC::visitTypeInteger(shared_ptr<const AST::TypeInteger> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "0L";
    else
        os() << "long";
}

void CodeWriterC::visitTypeReference(shared_ptr<const AST::TypeReference> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    node->toRValue()->writeCode(*this);
    assert(!isInitializer);
    os() << " *";
}

void CodeWriterC::visitTypeSingle(shared_ptr<const AST::TypeSingle> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "0.0f";
    else
        os() << "float";
}

void CodeWriterC::visitTypeString(shared_ptr<const AST::TypeString> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "wstring(L\"\")";
    else
        os() << "wstring";
}

void CodeWriterC::visitTypeType(shared_ptr<const AST::TypeType> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    os() << "struct " << string_cast<string>(createStructureName(node));
    if(isInitializer)
    {
        os() << "()";
        return;
    }
    if(!isImplementation)
        return;
    os() << "\n";
    os() << indent << "{\n";
    didIndent = false;
    indent.depth++;
    isImplementation = false;
    for(const AST::Symbol &symbol : *node->symbols)
    {
        switch(symbol.stype())
        {
        case AST::Symbol::Type::AutoVariable:
        case AST::Symbol::Type::ReferenceVariable:
        case AST::Symbol::Type::StaticVariable:
        case AST::Symbol::Type::Procedure:
            break;
        case AST::Symbol::Type::Type:
        {
            shared_ptr<const AST::TypeType> type = dynamic_pointer_cast<const AST::TypeType>(symbol.value);
            assert(type != nullptr);
            if(!didIndent)
                os() << indent;
            didIndent = true;
            os() << "struct " << string_cast<string>(createStructureName(type)) << ";\n";
            didIndent = false;
            isDeclaration = false;
            break;
        }
        default:
            throw runtime_error("can't declare symbol " + string_cast<string>(symbol.name()));
        }
    }
    for(const AST::Symbol &symbol : *node->symbols)
    {
        switch(symbol.stype())
        {
        case AST::Symbol::Type::AutoVariable:
        case AST::Symbol::Type::ReferenceVariable:
        case AST::Symbol::Type::StaticVariable:
        case AST::Symbol::Type::Procedure:
            break;
        case AST::Symbol::Type::Type:
            isImplementation = true;
            symbol.value->writeCode(*this);
            didIndent = false;
            isImplementation = false;
            break;
        default:
            throw runtime_error("can't declare symbol " + string_cast<string>(symbol.name()));
        }
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
            os() << ";\n";
            didIndent = false;
            isDeclaration = false;
            break;
        case AST::Symbol::Type::Procedure:
            if(std::get<1>(procedureSet.insert(symbol.value)))
            {
                procedureList.push_back(symbol.value);
                newProcedures.push_back(symbol.value);
            }
            break;
       case AST::Symbol::Type::Type:
            break;
        default:
            throw runtime_error("can't declare symbol " + string_cast<string>(symbol.name()));
        }
    }
    indent.depth--;
    os() << indent << "};\n";
    isImplementation = true;
}

void CodeWriterC::visitTypeUInt16(shared_ptr<const AST::TypeUInt16> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "(uint16_t)0";
    else
        os() << "uint16_t";
}

void CodeWriterC::visitTypeUInt32(shared_ptr<const AST::TypeUInt32> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "(uint32_t)0";
    else
        os() << "uint32_t";
}

void CodeWriterC::visitTypeUInt64(shared_ptr<const AST::TypeUInt64> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "(uint64_t)0";
    else
        os() << "uint64_t";
}

void CodeWriterC::visitTypeUInt8(shared_ptr<const AST::TypeUInt8> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    if(isInitializer)
        os() << "(uint8_t)0";
    else
        os() << "uint8_t";
}

void CodeWriterC::visitUnaryPlusExpression(shared_ptr<const AST::UnaryPlusExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    shared_ptr<AST::Expression> e = node->args().front();
    os() << "+(";
    e->writeCode(*this);
    os() << ")";
}

void CodeWriterC::visitWhileStatement(shared_ptr<const AST::WhileStatement> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    string exitLabel = makeLabel();
    whileStatementExitLabelMap[node] = exitLabel;
    os() << "while(";
    node->condition->writeCode(*this);
    os() << ")\n";
    didIndent = false;
    node->body->writeCode(*this);
    os() << indent << exitLabel << ": ;\n";
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

void CodeWriterC::writeInitializer(std::shared_ptr<const AST::Variable> node)
{
    assert(dynamic_cast<const AST::ReferenceVariable *>(node.get()) == nullptr);
    isInitializer = true;
    node->type()->toRValue()->writeCode(*this);
    isInitializer = false;
}