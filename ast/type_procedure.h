#ifndef TYPE_PROCEDURE_H_INCLUDED
#define TYPE_PROCEDURE_H_INCLUDED

#include "ast/type.h"
#include "ast/type_builtin.h"
#include <memory>
#include <vector>
#include <cassert>
#include <cwchar>
#include <string>
#include <sstream>

namespace AST
{
class TypeProcedure final : public Type
{
public:
    std::vector<std::shared_ptr<const Type>> args;
    std::shared_ptr<const Type> returnType;
    enum class ProcedureType
    {
        Sub,
        Function
    };
    ProcedureType procedureType;
    static std::wstring getProcedureTypeString(ProcedureType pt)
    {
        switch(pt)
        {
        case ProcedureType::Sub:
            return L"Sub";
        case ProcedureType::Function:
            return L"Function";
        }
        assert(false);
        return L"unknown";
    }
    static bool getProcedureHasReturnValue(ProcedureType pt)
    {
        switch(pt)
        {
        case ProcedureType::Sub:
            return false;
        case ProcedureType::Function:
            return true;
        }
        assert(false);
        return false;
    }
private:
    TypeProcedure(Location location, ProcedureType procedureType, std::vector<std::shared_ptr<const Type>> args, std::shared_ptr<const Type> returnType)
        : Type(location), args(args), returnType(getProcedureHasReturnValue(procedureType) ? returnType : TypeEmpty::getInstance()), procedureType(procedureType)
    {
    }
public:
    static std::shared_ptr<TypeProcedure> make(Location location, ProcedureType procedureType, std::vector<std::shared_ptr<const Type>> args, std::shared_ptr<const Type> returnType)
    {
        return std::shared_ptr<TypeProcedure>(new TypeProcedure(location, procedureType, args, returnType));
    }
    virtual std::wstring toString() const override
    {
        std::wostringstream ss;
        ss << getProcedureTypeString(procedureType) << L" (";
        std::wstring seperator = L"";
        for(std::shared_ptr<const Type> arg : args)
        {
            ss << seperator << L"As " << arg->toString();
            seperator = L", ";
        }
        ss << L")";
        if(getProcedureHasReturnValue(procedureType))
            ss << L" As " << returnType->toString();
        return ss.str();
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override
    {
        return *this == *destType;
    }
    virtual bool operator ==(const Type &rt_in) const override
    {
        const TypeProcedure *prt = dynamic_cast<const TypeProcedure *>(&rt_in);
        if(prt == nullptr)
            return false;
        const TypeProcedure &rt = *prt;
        if(procedureType != rt.procedureType)
            return false;
        if(getProcedureHasReturnValue(procedureType))
        {
            if(*returnType != *rt.returnType)
                return false;
        }
        if(args.size() != rt.args.size())
            return false;
        for(size_t i = 0; i < args.size(); i++)
        {
            if(*args[i] != *rt.args[i])
                return false;
        }
        return true;
    }
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
protected:
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt) const override
    {
        if(*this == *rt)
            return rt;
        return nullptr;
    }
};
}

#endif // TYPE_PROCEDURE_H_INCLUDED
