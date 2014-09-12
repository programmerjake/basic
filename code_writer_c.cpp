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
        os() << " " << string_cast<string>(createVariableName(node->name())) << declarationTypeAfterVariable;
        didIndent = false;
    }
    else if(isInitialization)
    {
        declarationTypeAfterVariable = "";
        if(!didIndent)
            os() << indent;
        didIndent = true;
        if(node->initializer() != nullptr)
        {
            os() << string_cast<string>(createVariableName(node->name()));
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
        throw logic_error("LBound not implemented");
    case AST::BuiltInFunctionExpression::FnType::LBound2:
        throw logic_error("LBound not implemented");
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
        os() << "fn_Mid(";
        node->args()[0]->writeCode(*this);
        os() << ", ";
        node->args()[1]->writeCode(*this);
        os() << ")";
        return;
    case AST::BuiltInFunctionExpression::FnType::Mid3:
        if(!didIndent)
            os() << indent;
        didIndent = true;
        os() << "fn_Mid(";
        node->args()[0]->writeCode(*this);
        os() << ", ";
        node->args()[1]->writeCode(*this);
        os() << ", ";
        node->args()[2]->writeCode(*this);
        os() << ")";
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
        throw logic_error("UBound not implemented");
    case AST::BuiltInFunctionExpression::FnType::UBound2:
        throw logic_error("UBound not implemented");
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
    if(isOutermostCodeBlock)
    {
        indent.depth = 0;
        os() << indent << "#include <cwchar>\n";
        os() << indent << "#include <cwctype>\n";
        os() << indent << "#include <cmath>\n";
        os() << indent << "#include <string>\n";
        os() << indent << "#include <sstream>\n";
        os() << indent << "#include <cstdint>\n";
        os() << indent << "#include <tuple>\n";
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
        os() << indent << "retval += (wchar_t)(digit <= 9 ? '0' + digit : 'A' + digit - 0xA);\n";
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
        os() << indent << "retval += (wchar_t)('0' + digit);\n";
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
        os() << indent << "retval += (wchar_t)('0' + digit);\n";
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
        os() << indent << "struct fn_Mid final\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "wstring str;\n";
        os() << indent << "wstring *pstr;\n";
        os() << indent << "size_t start;\n";
        os() << indent << "size_t length;\n";
        os() << indent << "fn_Mid(wstring str, long start)\n";
        indent.depth++;
        os() << indent << ": str(str), pstr(&this->str), start(start - 1), length(wstring::npos)\n";
        indent.depth--;
        os() << indent << "{\n";
        os() << indent << "}\n";
        os() << indent << "fn_Mid(wstring str, long start, long count)\n";
        indent.depth++;
        os() << indent << ": str(str), pstr(&this->str), start(start - 1), length(count)\n";
        indent.depth--;
        os() << indent << "{\n";
        os() << indent << "}\n";
        os() << indent << "fn_Mid(wstring *pstr, long start)\n";
        indent.depth++;
        os() << indent << ": str(), pstr(pstr), start(start - 1), length(wstring::npos)\n";
        indent.depth--;
        os() << indent << "{\n";
        os() << indent << "}\n";
        os() << indent << "fn_Mid(wstring *pstr, long start, long count)\n";
        indent.depth++;
        os() << indent << ": str(), pstr(pstr), start(start - 1), length(count)\n";
        indent.depth--;
        os() << indent << "{\n";
        os() << indent << "}\n";
        os() << indent << "struct ref final\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "wstring str;\n";
        os() << indent << "wstring *pstr;\n";
        os() << indent << "size_t start;\n";
        os() << indent << "size_t length;\n";
        os() << indent << "ref(const wstring &str, wstring *pstr, size_t start, size_t length)\n";
        indent.depth++;
        os() << indent << ": str(str), pstr(pstr == &str ? &this->str : pstr), start(start), length(length)\n";
        indent.depth--;
        os() << indent << "{\n";
        os() << indent << "}\n";
        os() << indent << "void operator =(wstring s) const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "pstr->replace(start, length, s);\n";
        indent.depth--;
        os() << indent << "}\n";
        os() << indent << "operator wstring() const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return pstr->substr(start, length);\n";
        indent.depth--;
        os() << indent << "}\n";
        indent.depth--;
        os() << indent << "};\n";
        os() << indent << "ref operator *() const\n";
        os() << indent << "{\n";
        indent.depth++;
        os() << indent << "return ref(str, pstr, start, length);\n";
        indent.depth--;
        os() << indent << "}\n";
        indent.depth--;
        os() << indent << "};\n";
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
        indent.depth--;
        os() << indent << "private:\n";
        indent.depth++;
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
            isDeclaration = true;
            symbol.value->writeCode(*this);
            os() << ";\n";
            didIndent = false;
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
    ss << showpoint << node->value;
    os() << ss.str();
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

void CodeWriterC::visitReferenceVariable(shared_ptr<const AST::ReferenceVariable> node)
{
    if(isDeclaration)
    {
        declarationTypeAfterVariable = "";
        if(!didIndent)
            os() << indent;
        didIndent = true;
        node->type()->writeCode(*this);
        os() << " " << string_cast<string>(createVariableName(node->name())) << declarationTypeAfterVariable;
        didIndent = false;
    }
    else if(isInitialization)
    {
        if(!didIndent)
            os() << indent;
        didIndent = true;
        if(node->initializer() != nullptr)
        {
            os() << string_cast<string>(createVariableName(node->name()));
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
        os() << string_cast<string>(createVariableName(node->name()));
    }
}

void CodeWriterC::visitSingleLiteralExpression(shared_ptr<const AST::SingleLiteralExpression> node)
{
    if(!didIndent)
        os() << indent;
    didIndent = true;
    ostringstream ss;
    ss.precision(9);
    ss << showpoint << node->value << "f";
    os() << ss.str();
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
        os() << " " << string_cast<string>(createVariableName(node->name())) << declarationTypeAfterVariable;
        if(node->initializer() != nullptr)
        {
            os() << " = (";
            isDeclaration = false;
            node->initializer()->writeCode(*this);
            isDeclaration = true;
            os() << ")";
        }
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
    os() << "const ArrayDescriptor<";
    node->elementType()->writeCode(*this);
    os() << ", " << node->indexRanges().size() << "> *";
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
