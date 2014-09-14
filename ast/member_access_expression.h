#ifndef MEMBER_ACCESS_EXPRESSION_H_INCLUDED
#define MEMBER_ACCESS_EXPRESSION_H_INCLUDED

#include "ast/expression.h"
#include "ast/type.h"
#include "ast/type_type.h"
#include "ast/type_reference.h"
#include "ast/cast_expression.h"
#include <cwchar>
#include <string>
#include "error.h"

namespace AST
{
class MemberAccessExpression final : public Expression
{
public:
    std::wstring memberName;
    static Symbol getMember(Location location, std::shared_ptr<const Type> base, std::wstring memberName)
    {
        std::shared_ptr<const TypeType> t = std::dynamic_pointer_cast<const TypeType>(base->toRValue()->getAbsoluteBaseType());
        if(t == nullptr)
            throw ParserError(location, L"can't use . operator here");
        if(!t->symbols->exists(memberName))
            throw ParserError(location, L"no member " + memberName + L" of " + t->toString());
        return t->symbols->get(memberName);
    }
    static std::shared_ptr<const Type> getMemberType(Location location, std::shared_ptr<const Type> base, std::wstring memberName)
    {
        Symbol s = getMember(location, base, memberName);
        switch(s.stype())
        {
        case Symbol::Type::AutoVariable:
        case Symbol::Type::StaticVariable:
        case Symbol::Type::Procedure:
        {
            std::shared_ptr<Expression> e = std::dynamic_pointer_cast<Expression>(s.value);
            if(!base->isLValue())
                return e->type()->toRValue();
            return e->type();
        }
        case Symbol::Type::ReferenceVariable:
        {
            std::shared_ptr<Expression> e = std::dynamic_pointer_cast<Expression>(s.value);
            return e->type();
        }
        case Symbol::Type::None:
        case Symbol::Type::Type:
            break;
        }
        throw ParserError(location, L"can't use member " + memberName + L" of " + base->toRValue()->getAbsoluteBaseType()->toString() + L" here");
    }
private:
    MemberAccessExpression(Location location, std::shared_ptr<Expression> expression, std::wstring memberName)
        : Expression(location, getMemberType(location, expression->type(), memberName)), memberName(memberName)
    {
        std::shared_ptr<const Type> castToType = expression->type()->toRValue()->getAbsoluteBaseType();
        if(expression->type()->isLValue())
            castToType = AST::TypeReference::toLValue(castToType);
        argsRef().assign(1, CastExpression::castImplicit(expression, castToType));
    }
public:
    static std::shared_ptr<MemberAccessExpression> make(Location location, std::shared_ptr<Expression> expression, std::wstring memberName)
    {
        return std::shared_ptr<MemberAccessExpression>(new MemberAccessExpression(location, expression, memberName));
    }
    virtual void writeCode(CodeWriter &cw) const override; // in expressions.cpp
};
}

#endif // MEMBER_ACCESS_EXPRESSION_H_INCLUDED
