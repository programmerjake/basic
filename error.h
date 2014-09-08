#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include "location.h"
#include <stdexcept>
#include <cwchar>
#include <string>
#include <sstream>
#include "string_cast.h"

class InputError : public std::runtime_error
{
private:
    static std::string makeMessage(Location location, const std::wstring &msg)
    {
        std::wostringstream ss;
        ss << location.fileName << L":" << location.line << L":" << location.col << L": error: " << msg;
        return string_cast<std::string>(ss.str());
    }
public:
    Location location;
    std::wstring msg;
    InputError(Location location, const std::wstring &msg)
        : runtime_error(makeMessage(location, msg)), location(location), msg(msg)
    {
    }
};

class TokenizerError final : public InputError
{
public:
    TokenizerError(Location location, const std::wstring &msg)
        : InputError(location, msg)
    {
    }
};

class ParserError final : public InputError
{
public:
    ParserError(Location location, const std::wstring &msg)
        : InputError(location, msg)
    {
    }
};

#endif // ERROR_H_INCLUDED
