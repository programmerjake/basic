#ifndef STATEMENT_H_INCLUDED
#define STATEMENT_H_INCLUDED

#include "ast/base.h"
#include "location.h"

namespace AST
{
class Statement : public Base
{
public:
    explicit Statement(Location location)
        : Base(location)
    {
    }
};
}

#endif // STATEMENT_H_INCLUDED
