#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <memory>
#include "location.h"

class CodeWriter;

namespace AST
{
class Base : public std::enable_shared_from_this<Base>
{
    void operator =(const Base &) = delete;
    Location location_;
protected:
    Base(const Base &rt)
        : location_(rt.location_)
    {
    }
    void location(Location l)
    {
        location_ = l;
    }
public:
    const Location &location() const
    {
        return location_;
    }
    explicit Base(Location location)
        : location_(location)
    {
    }
    virtual ~Base()
    {
    }
    virtual void writeCode(CodeWriter &cw) const = 0;
};
}

#endif // BASE_H_INCLUDED
