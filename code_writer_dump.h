#ifndef CODE_WRITER_DUMP_H_INCLUDED
#define CODE_WRITER_DUMP_H_INCLUDED

#include "code_writer.h"
#include <ostream>
#include <memory>
#include <unordered_set>

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
    std::unordered_set<std::shared_ptr<const AST::Procedure>> procedures;
    std::unordered_set<std::shared_ptr<const AST::TypeType>> structures;
public:
    CodeWriterDump(std::shared_ptr<std::ostream> dumpStream)
        : dumpStream(dumpStream)
    {
    }
    virtual void visitAddExpression(std::shared_ptr<const AST::AddExpression> node) override;
    virtual void visitAndExpression(std::shared_ptr<const AST::AndExpression> node) override;
    virtual void visitArrayIndexExpression(std::shared_ptr<const AST::ArrayIndexExpression> node) override;
    virtual void visitAssignStatement(std::shared_ptr<const AST::AssignStatement> node) override;
    virtual void visitAutoVariable(std::shared_ptr<const AST::AutoVariable> node) override;
    virtual void visitBooleanLiteralExpression(std::shared_ptr<const AST::BooleanLiteralExpression> node) override;
    virtual void visitBuiltInFunctionExpression(std::shared_ptr<const AST::BuiltInFunctionExpression> node) override;
    virtual void visitCallExpression(std::shared_ptr<const AST::CallExpression> node) override;
    virtual void visitCastExpression(std::shared_ptr<const AST::CastExpression> node) override;
    virtual void visitCodeBlock(std::shared_ptr<const AST::CodeBlock> node) override;
    virtual void visitCompareExpression(std::shared_ptr<const AST::CompareExpression> node) override;
    virtual void visitDoStatement(std::shared_ptr<const AST::DoStatement> node) override;
    virtual void visitDoubleLiteralExpression(std::shared_ptr<const AST::DoubleLiteralExpression> node) override;
    virtual void visitExitDoStatement(std::shared_ptr<const AST::ExitDoStatement> node) override;
    virtual void visitExitForStatement(std::shared_ptr<const AST::ExitForStatement> node) override;
    virtual void visitExitWhileStatement(std::shared_ptr<const AST::ExitWhileStatement> node) override;
    virtual void visitExitProcedureStatement(std::shared_ptr<const AST::ExitProcedureStatement> node) override;
    virtual void visitFDivExpression(std::shared_ptr<const AST::FDivExpression> node) override;
    virtual void visitForStatement(std::shared_ptr<const AST::ForStatement> node) override;
    virtual void visitIDivExpression(std::shared_ptr<const AST::IDivExpression> node) override;
    virtual void visitIfStatement(std::shared_ptr<const AST::IfStatement> node) override;
    virtual void visitInitializeStatement(std::shared_ptr<const AST::InitializeStatement> node) override;
    virtual void visitIntegerLiteralExpression(std::shared_ptr<const AST::IntegerLiteralExpression> node) override;
    virtual void visitMemberAccessExpression(std::shared_ptr<const AST::MemberAccessExpression> node) override;
    virtual void visitModExpression(std::shared_ptr<const AST::ModExpression> node) override;
    virtual void visitMulExpression(std::shared_ptr<const AST::MulExpression> node) override;
    virtual void visitNegExpression(std::shared_ptr<const AST::NegExpression> node) override;
    virtual void visitNotExpression(std::shared_ptr<const AST::NotExpression> node) override;
    virtual void visitOrExpression(std::shared_ptr<const AST::OrExpression> node) override;
    virtual void visitProcedure(std::shared_ptr<const AST::Procedure> node) override;
    virtual void visitReferenceVariable(std::shared_ptr<const AST::ReferenceVariable> node) override;
    virtual void visitSingleLiteralExpression(std::shared_ptr<const AST::SingleLiteralExpression> node) override;
    virtual void visitStaticVariable(std::shared_ptr<const AST::StaticVariable> node) override;
    virtual void visitStringLiteralExpression(std::shared_ptr<const AST::StringLiteralExpression> node) override;
    virtual void visitSubExpression(std::shared_ptr<const AST::SubExpression> node) override;
    virtual void visitTypeArray(std::shared_ptr<const AST::TypeArray> node) override;
    virtual void visitTypeBoolean(std::shared_ptr<const AST::TypeBoolean> node) override;
    virtual void visitTypeDouble(std::shared_ptr<const AST::TypeDouble> node) override;
    virtual void visitTypeEmpty(std::shared_ptr<const AST::TypeEmpty> node) override;
    virtual void visitTypeInt16(std::shared_ptr<const AST::TypeInt16> node) override;
    virtual void visitTypeInt32(std::shared_ptr<const AST::TypeInt32> node) override;
    virtual void visitTypeInt64(std::shared_ptr<const AST::TypeInt64> node) override;
    virtual void visitTypeInt8(std::shared_ptr<const AST::TypeInt8> node) override;
    virtual void visitTypeInteger(std::shared_ptr<const AST::TypeInteger> node) override;
    virtual void visitTypeProcedure(std::shared_ptr<const AST::TypeProcedure> node) override;
    virtual void visitTypeReference(std::shared_ptr<const AST::TypeReference> node) override;
    virtual void visitTypeSingle(std::shared_ptr<const AST::TypeSingle> node) override;
    virtual void visitTypeString(std::shared_ptr<const AST::TypeString> node) override;
    virtual void visitTypeType(std::shared_ptr<const AST::TypeType> node) override;
    virtual void visitTypeUInt16(std::shared_ptr<const AST::TypeUInt16> node) override;
    virtual void visitTypeUInt32(std::shared_ptr<const AST::TypeUInt32> node) override;
    virtual void visitTypeUInt64(std::shared_ptr<const AST::TypeUInt64> node) override;
    virtual void visitTypeUInt8(std::shared_ptr<const AST::TypeUInt8> node) override;
    virtual void visitUnaryPlusExpression(std::shared_ptr<const AST::UnaryPlusExpression> node) override;
    virtual void visitWhileStatement(std::shared_ptr<const AST::WhileStatement> node) override;
    virtual void visitXorExpression(std::shared_ptr<const AST::XorExpression> node) override;
    virtual void finish() override;
};

#endif // CODE_WRITER_DUMP_H_INCLUDED
