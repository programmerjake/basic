#ifndef LOCATION_H_INCLUDED
#define LOCATION_H_INCLUDED

#include <cwchar>
#include <string>

struct Location final
{
    size_t line, col;
    std::wstring fileName;
    Location()
        : line(1), col(1), fileName(L"")
    {
    }
    Location(size_t line, size_t col, std::wstring fileName)
        : line(line), col(col), fileName(fileName)
    {
    }
};

#endif // LOCATION_H_INCLUDED
