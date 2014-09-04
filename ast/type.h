#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#include "ast/base.h"
#include <cwchar>
#include <string>

namespace AST
{
class Type : public Base
{
public:
    virtual std::wstring toString() const = 0;
};
}

#endif // TYPE_H_INCLUDED
