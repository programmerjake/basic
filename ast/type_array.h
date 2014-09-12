#ifndef TYPE_ARRAY_H_INCLUDED
#define TYPE_ARRAY_H_INCLUDED

#include "type.h"
#include "error.h"
#include <memory>
#include <cstdint>
#include <sstream>
#include <tuple>
#include <vector>
#include <cassert>

namespace AST
{
class TypeArray final : public Type
{
public:
    typedef std::pair<std::int64_t, std::int64_t> IndexRange;
    static constexpr IndexRange defaultEmptyRange = IndexRange(2, 0); // definition in type_builtin.cpp
    static constexpr std::int64_t defaultRangeStart = 1; // definition in type_builtin.cpp
    static constexpr std::int64_t getRangeSize(IndexRange ir)
    {
        return std::get<1>(ir) - std::get<0>(ir) + 1;
    }
    static constexpr bool isEmptyRange(IndexRange ir)
    {
        return getRangeSize(ir) < 0;
    }
private:
    std::shared_ptr<const Type> elementType_;
    std::vector<IndexRange> indexRanges_;
    TypeArray(Location location, std::shared_ptr<const Type> elementType, std::vector<IndexRange> indexRanges)
        : Type(location), elementType_(elementType), indexRanges_(indexRanges)
    {
        if(elementType->isLValue())
            throw ParserError(location, L"can't create array of ByRef type");
        if(this->indexRanges().empty())
            indexRangesRef().assign(1, defaultEmptyRange);
    }
    std::vector<IndexRange> &indexRangesRef()
    {
        return indexRanges_;
    }
    void elementType(std::shared_ptr<const Type> v)
    {
        elementType_ = v;
    }
public:
    std::shared_ptr<const Type> elementType() const
    {
        return elementType_;
    }
    const std::vector<IndexRange> &indexRanges() const
    {
        return indexRanges_;
    }
    bool isTotallySpecified() const
    {
        for(IndexRange ir : indexRanges())
        {
            if(isEmptyRange(ir))
                return false;
        }
        return true;
    }
    int64_t elementCount() const
    {
        int64_t retval = 1;
        for(IndexRange ir : indexRanges())
        {
            if(isEmptyRange(ir))
                return 0;
            retval *= getRangeSize(ir);
        }
        return retval;
    }
    static std::shared_ptr<TypeArray> make(Location location, std::shared_ptr<const Type> elementType, std::vector<IndexRange> indexRanges)
    {
        return std::shared_ptr<TypeArray>(new TypeArray(location, elementType, indexRanges));
    }
    virtual std::wstring toString() const override
    {
        std::wostringstream ss;
        ss << L"Array";
        assert(indexRanges().size() >= 1);
        if(indexRanges().size() > 1 || !isEmptyRange(indexRanges()[0]))
        {
            ss << L"(";
            std::wstring seperator = L"";
            for(IndexRange ir : indexRanges())
            {
                ss << seperator;
                seperator = L", ";
                if(!isEmptyRange(ir))
                {
                    if(std::get<0>(ir) != defaultRangeStart)
                        ss << std::get<0>(ir) << L" To ";
                    ss << std::get<1>(ir);
                }
            }
            ss << L")";
        }
        ss << L" Of " << elementType()->toString();
        return ss.str();
    }
    virtual bool canCastTo(std::shared_ptr<const Type> destType, bool isImplicitCast = true) const override
    {
        const TypeArray *prt = dynamic_cast<const TypeArray *>(destType->getAbsoluteBaseType().get());
        if(prt == nullptr)
            return false;
        const TypeArray &rt = *prt;
        if(rt.indexRanges().size() != indexRanges().size())
            return false;
        for(size_t i = 0; i < indexRanges().size(); i++)
        {
            IndexRange ir = indexRanges()[i], rtir = rt.indexRanges()[i];
            if(!isEmptyRange(rtir) && isEmptyRange(ir))
                return false;
            if(!isEmptyRange(rtir) && ir != rtir)
                return false;
        }
        #warning change to check for allowed constancy changes
        return *elementType()->getAbsoluteBaseType() == *rt.elementType();
    }
    virtual bool operator ==(const Type &rt_in) const override
    {
        const TypeArray *prt = dynamic_cast<const TypeArray *>(&rt_in);
        if(prt == nullptr)
            return false;
        const TypeArray &rt = *prt;
        if(rt.indexRanges().size() != indexRanges().size())
            return false;
        for(size_t i = 0; i < indexRanges().size(); i++)
        {
            IndexRange ir = indexRanges()[i], rtir = rt.indexRanges()[i];
            if(isEmptyRange(rtir) != isEmptyRange(ir))
                return false;
            if(!isEmptyRange(rtir) && ir != rtir)
                return false;
        }
        return *elementType() == *rt.elementType();
    }
    virtual void writeCode(CodeWriter &cw) const override; // in type_builtin.cpp
protected:
    virtual std::shared_ptr<const Type> getCommonType(std::shared_ptr<const Type> rt_in) const override
    {
        const TypeArray *prt = dynamic_cast<const TypeArray *>(rt_in.get());
        if(prt == nullptr)
            return nullptr;
        const TypeArray &rt = *prt;
        if(rt.indexRanges().size() != indexRanges().size())
            return nullptr;
        std::vector<IndexRange> newIndexRanges = indexRanges();
        for(size_t i = 0; i < indexRanges().size(); i++)
        {
            IndexRange ir = indexRanges()[i], rtir = rt.indexRanges()[i];
            if(isEmptyRange(ir) || ir != rtir)
                newIndexRanges[i] = defaultEmptyRange;
        }
        #warning change to check for allowed constancy changes
        std::shared_ptr<const Type> newElementType = Type::getCommonType(elementType(), rt.elementType());
        if(*elementType()->getAbsoluteBaseType() != *newElementType->getAbsoluteBaseType())
            return nullptr;
        return make(location(), newElementType, newIndexRanges);
    }
};
}

#endif // TYPE_ARRAY_H_INCLUDED
