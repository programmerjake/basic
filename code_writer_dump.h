#ifndef CODE_WRITER_DUMP_H_INCLUDED
#define CODE_WRITER_DUMP_H_INCLUDED

#include "code_writer.h"
#include <ostream>
#include <memory>

class CodeWriterDump final : public CodeWriter
{
private:
    std::shared_ptr<std::ostream> dumpStream;
    std::ostream &os()
    {
        return *dumpStream;
    }
    struct Indenter
    {
        size_t depth = 0;
        static constexpr size_t indentMultiplier = 4;
        friend std::ostream &operator <<(std::ostream &os, const Indenter &v)
        {
            size_t spaceCount = v.depth * indentMultiplier;
            for(size_t i = 0; i < spaceCount; i++)
                os << ' ';
            return os;
        }
    };
    Indenter indent;
    void dumpExpression(std::shared_ptr<const AST::Expression> node, std::string nodeName);
public:
    CodeWriterDump(std::shared_ptr<std::ostream> dumpStream)
        : dumpStream(dumpStream)
    {
    }
    virtual void visitAddExpression(std::shared_ptr<const AST::AddExpression> node) override;
    virtual void visitAndExpression(std::shared_ptr<const AST::AndExpression> node) override;
    virtual void visitAssignStatement(std::shared_ptr<const AST::AssignStatement> node) override;
    virtual void visitAutoVariable(std::shared_ptr<const AST::AutoVariable> node) override;
    virtual void visitBooleanLiteralExpression(std::shared_ptr<const AST::BooleanLiteralExpression> node) override;
    virtual void visitCastExpression(std::shared_ptr<const AST::CastExpression> node) override;
    virtual void visitCodeBlock(std::shared_ptr<const AST::CodeBlock> node) override;
    virtual void visitCompareExpression(std::shared_ptr<const AST::CompareExpression> node) override;
    virtual void visitDoStatement(std::shared_ptr<const AST::DoStatement> node) override;
    virtual void visitDoubleLiteralExpression(std::shared_ptr<const AST::DoubleLiteralExpression> node) override;
    virtual void visitFDivExpression(std::shared_ptr<const AST::FDivExpression> node) override;
    virtual void visitIfStatement(std::shared_ptr<const AST::IfStatement> node) override;
    virtual void visitIntegerLiteralExpression(std::shared_ptr<const AST::IntegerLiteralExpression> node) override;
    virtual void visitNotExpression(std::shared_ptr<const AST::NotExpression> node) override;
    virtual void visitOrExpression(std::shared_ptr<const AST::OrExpression> node) override;
    virtual void visitReferenceVariable(std::shared_ptr<const AST::ReferenceVariable> node) override;
    virtual void visitSingleLiteralExpression(std::shared_ptr<const AST::SingleLiteralExpression> node) override;
    virtual void visitStaticVariable(std::shared_ptr<const AST::StaticVariable> node) override;
    virtual void visitStringLiteralExpression(std::shared_ptr<const AST::StringLiteralExpression> node) override;
    virtual void visitTypeBoolean(std::shared_ptr<const AST::TypeBoolean> node) override;
    virtual void visitTypeDouble(std::shared_ptr<const AST::TypeDouble> node) override;
    virtual void visitTypeInt16(std::shared_ptr<const AST::TypeInt16> node) override;
    virtual void visitTypeInt32(std::shared_ptr<const AST::TypeInt32> node) override;
    virtual void visitTypeInt64(std::shared_ptr<const AST::TypeInt64> node) override;
    virtual void visitTypeInt8(std::shared_ptr<const AST::TypeInt8> node) override;
    virtual void visitTypeInteger(std::shared_ptr<const AST::TypeInteger> node) override;
    virtual void visitTypeReference(std::shared_ptr<const AST::TypeReference> node) override;
    virtual void visitTypeSingle(std::shared_ptr<const AST::TypeSingle> node) override;
    virtual void visitTypeString(std::shared_ptr<const AST::TypeString> node) override;
    virtual void visitTypeUInt16(std::shared_ptr<const AST::TypeUInt16> node) override;
    virtual void visitTypeUInt32(std::shared_ptr<const AST::TypeUInt32> node) override;
    virtual void visitTypeUInt64(std::shared_ptr<const AST::TypeUInt64> node) override;
    virtual void visitTypeUInt8(std::shared_ptr<const AST::TypeUInt8> node) override;
    virtual void visitWhileStatement(std::shared_ptr<const AST::WhileStatement> node) override;
    virtual void visitXorExpression(std::shared_ptr<const AST::XorExpression> node) override;
    virtual void finish() override;
};

#endif // CODE_WRITER_DUMP_H_INCLUDED
