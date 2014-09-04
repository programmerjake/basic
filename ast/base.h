#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <memory>

namespace AST
{
class Base : public std::enable_shared_from_this<Base>
{
    void operator =(const Base &) = delete;
protected:
    Base(const Base &)
    {
    }
public:
    Base()
    {
    }
    virtual ~Base()
    {
    }
    virtual std::shared_ptr<Base> dup() const = 0;
};
}

#endif // BASE_H_INCLUDED
