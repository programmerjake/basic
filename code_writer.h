#ifndef CODE_WRITER_H_INCLUDED
#define CODE_WRITER_H_INCLUDED

#include "ast/all.h"
#include <memory>

class CodeWriter
{
    CodeWriter(const CodeWriter &) = delete;
    void operator =(const CodeWriter &) = delete;
public:
    CodeWriter()
    {
    }
    virtual ~CodeWriter()
    {
    }
    virtual void visitAddExpression(std::shared_ptr<const AST::AddExpression> node) = 0;
    virtual void visitAndExpression(std::shared_ptr<const AST::AndExpression> node) = 0;
    virtual void visitAssignStatement(std::shared_ptr<const AST::AssignStatement> node) = 0;
    virtual void visitAutoVariable(std::shared_ptr<const AST::AutoVariable> node) = 0;
    virtual void visitBooleanLiteralExpression(std::shared_ptr<const AST::BooleanLiteralExpression> node) = 0;
    virtual void visitBuiltInFunctionExpression(std::shared_ptr<const AST::BuiltInFunctionExpression> node) = 0;
    virtual void visitCastExpression(std::shared_ptr<const AST::CastExpression> node) = 0;
    virtual void visitCodeBlock(std::shared_ptr<const AST::CodeBlock> node) = 0;
    virtual void visitCompareExpression(std::shared_ptr<const AST::CompareExpression> node) = 0;
    virtual void visitDoStatement(std::shared_ptr<const AST::DoStatement> node) = 0;
    virtual void visitDoubleLiteralExpression(std::shared_ptr<const AST::DoubleLiteralExpression> node) = 0;
    virtual void visitFDivExpression(std::shared_ptr<const AST::FDivExpression> node) = 0;
    virtual void visitForStatement(std::shared_ptr<const AST::ForStatement> node) = 0;
    virtual void visitIDivExpression(std::shared_ptr<const AST::IDivExpression> node) = 0;
    virtual void visitIfStatement(std::shared_ptr<const AST::IfStatement> node) = 0;
    virtual void visitInitializeStatement(std::shared_ptr<const AST::InitializeStatement> node) = 0;
    virtual void visitIntegerLiteralExpression(std::shared_ptr<const AST::IntegerLiteralExpression> node) = 0;
    virtual void visitModExpression(std::shared_ptr<const AST::ModExpression> node) = 0;
    virtual void visitMulExpression(std::shared_ptr<const AST::MulExpression> node) = 0;
    virtual void visitNegExpression(std::shared_ptr<const AST::NegExpression> node) = 0;
    virtual void visitNotExpression(std::shared_ptr<const AST::NotExpression> node) = 0;
    virtual void visitOrExpression(std::shared_ptr<const AST::OrExpression> node) = 0;
    virtual void visitReferenceVariable(std::shared_ptr<const AST::ReferenceVariable> node) = 0;
    virtual void visitSingleLiteralExpression(std::shared_ptr<const AST::SingleLiteralExpression> node) = 0;
    virtual void visitStaticVariable(std::shared_ptr<const AST::StaticVariable> node) = 0;
    virtual void visitStringLiteralExpression(std::shared_ptr<const AST::StringLiteralExpression> node) = 0;
    virtual void visitSubExpression(std::shared_ptr<const AST::SubExpression> node) = 0;
    virtual void visitTypeBoolean(std::shared_ptr<const AST::TypeBoolean> node) = 0;
    virtual void visitTypeDouble(std::shared_ptr<const AST::TypeDouble> node) = 0;
    virtual void visitTypeInt16(std::shared_ptr<const AST::TypeInt16> node) = 0;
    virtual void visitTypeInt32(std::shared_ptr<const AST::TypeInt32> node) = 0;
    virtual void visitTypeInt64(std::shared_ptr<const AST::TypeInt64> node) = 0;
    virtual void visitTypeInt8(std::shared_ptr<const AST::TypeInt8> node) = 0;
    virtual void visitTypeInteger(std::shared_ptr<const AST::TypeInteger> node) = 0;
    virtual void visitTypeReference(std::shared_ptr<const AST::TypeReference> node) = 0;
    virtual void visitTypeSingle(std::shared_ptr<const AST::TypeSingle> node) = 0;
    virtual void visitTypeString(std::shared_ptr<const AST::TypeString> node) = 0;
    virtual void visitTypeUInt16(std::shared_ptr<const AST::TypeUInt16> node) = 0;
    virtual void visitTypeUInt32(std::shared_ptr<const AST::TypeUInt32> node) = 0;
    virtual void visitTypeUInt64(std::shared_ptr<const AST::TypeUInt64> node) = 0;
    virtual void visitTypeUInt8(std::shared_ptr<const AST::TypeUInt8> node) = 0;
    virtual void visitUnaryPlusExpression(std::shared_ptr<const AST::UnaryPlusExpression> node) = 0;
    virtual void visitWhileStatement(std::shared_ptr<const AST::WhileStatement> node) = 0;
    virtual void visitXorExpression(std::shared_ptr<const AST::XorExpression> node) = 0;
    virtual void finish() = 0;
};

#endif // CODE_WRITER_H_INCLUDED
