#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

#include <cstdint>
#include <cwchar>
#include <string>
#include <stdexcept>
#include <cwctype>
#include <cassert>
#include <memory>
#include <sstream>
#include "string_cast.h"

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

enum class TokenType : std::int_fast32_t
{
    TTEOF = -0x10000,
    TTId,
    TTStringLiteral,
    TTIntegerLiteral,
    TTSingleLiteral,
    TTDoubleLiteral,

    TTLogicalGE,
    TTLogicalLE,
    TTLogicalNE,

    TTAbs,
    TTAnd,
    TTAs,
    TTAsc,
    TTATn,
    TTBoolean,
    TTByRef,
    TTByVal,
    TTCase,
    TTCast,
    TTCBool,
    TTCDouble,
    TTCInt,
    TTCI8,
    TTCI16,
    TTCI32,
    TTCI64,
    TTCPtr,
    TTCSng,
    TTCStr,
    TTCU8,
    TTCByte = TTCU8,
    TTCU16,
    TTCU32,
    TTCU64,
    TTChr,
    TTCos,
    TTDim,
    TTDeclare,
    TTDo,
    TTDouble,
    TTElse,
    TTElseIf,
    TTEnd,
    TTExit,
    TTExp,
    TTFalse,
    TTFor,
    TTFunction,
    TTHex,
    TTIf,
    TTInStr,
    TTInt,
    TTInteger,
    TTInt8,
    TTInt16,
    TTInt32,
    TTInt64,
    TTIs,
    TTLBound,
    TTLCase,
    TTLeft,
    TTLen,
    TTLog,
    TTLoop,
    TTLTrim,
    TTMid,
    TTMod,
    TTNot,
    TTOct,
    TTOr,
    TTPointer,
    TTRight,
    TTRTrim,
    TTSelect,
    TTSgn,
    TTSin,
    TTSingle,
    TTSpace,
    TTSqr,
    TTStatic,
    TTStep,
    TTStr,
    TTString,
    TTSub,
    TTTan,
    TTThen,
    TTTo,
    TTTrue,
    TTType,
    TTTypeOf,
    TTUBound,
    TTUCase,
    TTUInt8,
    TTByte = TTUInt8,
    TTUInt16,
    TTUInt32,
    TTUInt64,
    TTUntil,
    TTVal,
    TTWEnd,
    TTWhile,
    TTXor,
};

struct Token final
{
    Location location;
    TokenType type;
    std::wstring svalue;
    explicit Token(TokenType type = TokenType::TTEOF, Location location = Location(), std::wstring svalue = L"")
        : location(location), type(type), svalue(svalue)
    {
    }
    void addChar(int ch)
    {
        wchar_t wch = ch;
        if(ch != (int)wch) // if wchar_t is 16 bit and ch won't fit then encode using surrogate pair
        {
            ch -= 0x10000;
            svalue += (wchar_t)(0xD800 + ((ch >> 10) & 0x3FF));
            svalue += (wchar_t)(0xDC00 + (ch & 0x3FF));
        }
        else
            svalue += wch;
    }
};

struct InputStream
{
    InputStream(const InputStream &rt) = delete;
    void operator =(const InputStream &rt) = delete;
    InputStream()
    {
    }
    virtual ~InputStream()
    {
    }
    virtual int getChar() = 0;
    virtual std::wstring getName() = 0;
};

class TokenizerError final : public std::runtime_error
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
    TokenizerError(Location location, const std::wstring &msg)
        : runtime_error(makeMessage(location, msg)), location(location), msg(msg)
    {
    }
};

class Tokenizer final
{
    Tokenizer(const Tokenizer &rt) = delete;
    void operator =(const Tokenizer &rt) = delete;
    Location location;
    std::shared_ptr<InputStream> input;
    int peekChar;
    static void handleError(Location location, const std::wstring &msg)
    {
        throw TokenizerError(location, msg);
    }
    static constexpr size_t TabWidth = 8;
    void nextChar()
    {
        switch(peekChar)
        {
        case '\t':
            location.col -= (location.col - 1) % TabWidth;
            location.col += TabWidth;
            break;
        case '\n':
            location.line++;
            location.col = 1;
            break;
        default:
            location.col++;
            break;
        }
        peekChar = input->getChar();
    }
    int getChar()
    {
        int retval = peekChar;
        nextChar();
        return retval;
    }
    static bool iswodigit(int v)
    {
        if(v >= '0' && v <= '7')
            return true;
        return false;
    }
    Token readNumber(Location tokenLocation)
    {
        Token retval = Token(TokenType::TTIntegerLiteral, tokenLocation);
        if(peekChar == '&')
        {
            retval.addChar(getChar());
            if(peekChar == 'h' || peekChar == 'H')
            {
                retval.addChar(towupper(getChar()));
                if(!std::iswxdigit(peekChar))
                    handleError(location, L"missing hexadecimal digit");
                while(std::iswxdigit(peekChar))
                {
                    retval.addChar(towupper(getChar()));
                }
                return retval;
            }
            else if(iswodigit(peekChar) || peekChar == 'o' || peekChar == 'O')
            {
                if(peekChar == 'o' || peekChar == 'O')
                    nextChar();
                retval.addChar(L'O');
                if(!iswodigit(peekChar))
                {
                    if(std::iswdigit(peekChar))
                        handleError(location, L"digit is not valid octal digit");
                    else
                        handleError(location, L"missing octal digit");
                }
                while(iswodigit(peekChar))
                {
                    retval.addChar(getChar());
                }
                if(std::iswdigit(peekChar))
                    handleError(location, L"digit is not valid octal digit");
                return retval;
            }
            retval.type = (TokenType)retval.svalue[0];
            return retval;
        }
        while(std::iswdigit(peekChar))
        {
            retval.addChar(getChar());
        }
        if(peekChar == '.')
        {
            retval.type = TokenType::TTDoubleLiteral;
            retval.addChar(getChar());
            if(retval.svalue == L"." && !std::iswdigit(peekChar))
            {
                retval.type = (TokenType)'.';
                return retval;
            }
            while(std::iswdigit(peekChar))
            {
                retval.addChar(getChar());
            }
        }
        if(peekChar == 'e' || peekChar == 'E' || peekChar == 'f' || peekChar == 'F')
        {
            if(peekChar == 'f' || peekChar == 'F')
                retval.type = TokenType::TTSingleLiteral;
            else
                retval.type = TokenType::TTDoubleLiteral;
            retval.addChar(getChar());
            if(peekChar == '+' || peekChar == '-')
                retval.addChar(getChar());
            if(!std::iswdigit(peekChar))
                handleError(location, L"missing decimal digit");
            while(std::iswdigit(peekChar))
            {
                retval.addChar(getChar());
            }
        }
        if(peekChar == '!' || peekChar == '#')
        {
            switch(getChar())
            {
            case '!':
                retval.type = TokenType::TTSingleLiteral;
                break;
            case '#':
                retval.type = TokenType::TTSingleLiteral;
                break;
            default:
                assert(false);
            }
        }
        return retval;
    }
    static int compareCaseInsensitive(const std::wstring &a, const std::wstring &b)
    {
        for(size_t i = 0; i < a.size() && i < b.size(); i++)
        {
            wchar_t ach = (wchar_t)std::towlower(a[i]);
            wchar_t bch = (wchar_t)std::towlower(b[i]);
            if(ach < bch)
                return -1;
            if(ach > bch)
                return 1;
        }
        if(a.size() < b.size())
            return -1;
        if(a.size() > b.size())
            return 1;
        return 0;
    }
    void translateKeyword(Token &token)
    {
        struct Translation final
        {
            std::wstring name;
            TokenType type;
        };
        static const Translation translations[] = // must be kept in sorted order
        {
            {L"Abs", TokenType::TTAbs},
            {L"And", TokenType::TTAnd},
            {L"As", TokenType::TTAs},
            {L"Asc", TokenType::TTAsc},
            {L"ATn", TokenType::TTATn},
            {L"Boolean", TokenType::TTBoolean},
            {L"ByRef", TokenType::TTByRef},
            {L"Byte", TokenType::TTByte},
            {L"ByVal", TokenType::TTByVal},
            {L"Case", TokenType::TTCase},
            {L"Cast", TokenType::TTCast},
            {L"CBool", TokenType::TTCBool},
            {L"CByte", TokenType::TTCByte},
            {L"CDouble", TokenType::TTCDouble},
            {L"Chr", TokenType::TTChr},
            {L"CI16", TokenType::TTCI16},
            {L"CI32", TokenType::TTCI32},
            {L"CI64", TokenType::TTCI64},
            {L"CI8", TokenType::TTCI8},
            {L"CInt", TokenType::TTCInt},
            {L"Cos", TokenType::TTCos},
            {L"CPtr", TokenType::TTCPtr},
            {L"CSng", TokenType::TTCSng},
            {L"CStr", TokenType::TTCStr},
            {L"CU16", TokenType::TTCU16},
            {L"CU32", TokenType::TTCU32},
            {L"CU64", TokenType::TTCU64},
            {L"CU8", TokenType::TTCU8},
            {L"Declare", TokenType::TTDeclare},
            {L"Dim", TokenType::TTDim},
            {L"Do", TokenType::TTDo},
            {L"Double", TokenType::TTDouble},
            {L"Else", TokenType::TTElse},
            {L"ElseIf", TokenType::TTElseIf},
            {L"End", TokenType::TTEnd},
            {L"Exit", TokenType::TTExit},
            {L"Exp", TokenType::TTExp},
            {L"False", TokenType::TTFalse},
            {L"For", TokenType::TTFor},
            {L"Function", TokenType::TTFunction},
            {L"Hex", TokenType::TTHex},
            {L"If", TokenType::TTIf},
            {L"InStr", TokenType::TTInStr},
            {L"Int", TokenType::TTInt},
            {L"Int16", TokenType::TTInt16},
            {L"Int32", TokenType::TTInt32},
            {L"Int64", TokenType::TTInt64},
            {L"Int8", TokenType::TTInt8},
            {L"Integer", TokenType::TTInteger},
            {L"Is", TokenType::TTIs},
            {L"LBound", TokenType::TTLBound},
            {L"LCase", TokenType::TTLCase},
            {L"Left", TokenType::TTLeft},
            {L"Len", TokenType::TTLen},
            {L"Log", TokenType::TTLog},
            {L"Loop", TokenType::TTLoop},
            {L"LTrim", TokenType::TTLTrim},
            {L"Mid", TokenType::TTMid},
            {L"Mod", TokenType::TTMod},
            {L"Not", TokenType::TTNot},
            {L"Oct", TokenType::TTOct},
            {L"Or", TokenType::TTOr},
            {L"Pointer", TokenType::TTPointer},
            {L"Right", TokenType::TTRight},
            {L"RTrim", TokenType::TTRTrim},
            {L"Select", TokenType::TTSelect},
            {L"Sgn", TokenType::TTSgn},
            {L"Sin", TokenType::TTSin},
            {L"Single", TokenType::TTSingle},
            {L"Space", TokenType::TTSpace},
            {L"Sqr", TokenType::TTSqr},
            {L"Static", TokenType::TTStatic},
            {L"Step", TokenType::TTStep},
            {L"Str", TokenType::TTStr},
            {L"String", TokenType::TTString},
            {L"Sub", TokenType::TTSub},
            {L"Tan", TokenType::TTTan},
            {L"Then", TokenType::TTThen},
            {L"To", TokenType::TTTo},
            {L"True", TokenType::TTTrue},
            {L"Type", TokenType::TTType},
            {L"TypeOf", TokenType::TTTypeOf},
            {L"UBound", TokenType::TTUBound},
            {L"UCase", TokenType::TTUCase},
            {L"UInt16", TokenType::TTUInt16},
            {L"UInt32", TokenType::TTUInt32},
            {L"UInt64", TokenType::TTUInt64},
            {L"UInt8", TokenType::TTUInt8},
            {L"Until", TokenType::TTUntil},
            {L"Val", TokenType::TTVal},
            {L"WEnd", TokenType::TTWEnd},
            {L"While", TokenType::TTWhile},
            {L"Xor", TokenType::TTXor},
        };
        size_t translationCount = sizeof(translations) / sizeof(translations[0]);
        int start = 0, end = translationCount - 1;
        while(start <= end)
        {
            int middle = start + (end - start) / 2;
            int comparisonResult = compareCaseInsensitive(token.svalue, translations[middle].name);
            if(comparisonResult == 0)
            {
                token.svalue = translations[middle].name;
                token.type = translations[middle].type;
                break;
            }
            else if(comparisonResult < 0)
            {
                end = middle - 1;
            }
            else
            {
                start = middle + 1;
            }
        }
    }
public:
    explicit Tokenizer(std::shared_ptr<InputStream> input)
        : location(1, 1, input->getName()), input(input), peekChar(input->getChar())
    {
    }
    Token getToken()
    {
        for(;;)
        {
            while(std::iswblank(peekChar))
                nextChar();
            Location tokenLocation = location;
            if(std::iswdigit(peekChar))
                return readNumber(tokenLocation);
            if(std::iswalpha(peekChar) || peekChar == '_')
            {
                Token retval = Token(TokenType::TTId, tokenLocation);
                while(std::iswalnum(peekChar) || peekChar == '_')
                {
                    retval.addChar(getChar());
                }
                translateKeyword(retval);
                if(retval.svalue == L"_")
                {
                    while(std::iswblank(peekChar))
                        nextChar();
                    if(peekChar == '\n')
                    {
                        nextChar();
                        continue;
                    }
                }
                return retval;
            }
            switch(peekChar)
            {
            case EOF:
                return Token(TokenType::TTEOF, tokenLocation);
            case '\n':
            case '(':
            case ')':
            case '^':
            case '*':
            case '@':
            case '#':
            case '-':
            case '+':
            case '=':
            case '\\':
            case '/':
            case ';':
            case ':':
            case ',':
            {
                wchar_t ch = getChar();
                return Token((TokenType)ch, tokenLocation, std::wstring(1, ch));
            }
            case '.':
            case '&':
                return readNumber(tokenLocation);
            case '\"':
            {
                Token retval(TokenType::TTStringLiteral, tokenLocation);
                nextChar();
                while(peekChar != EOF && peekChar != '\n')
                {
                    if(peekChar == '\"')
                    {
                        nextChar();
                        if(peekChar != '\"')
                        {
                            return retval;
                        }
                    }
                    retval.addChar(getChar());
                }
                handleError(location, L"missing closing \"");
                return Token(TokenType::TTEOF, tokenLocation);
            }
            case '>':
                nextChar();
                if(peekChar == '=')
                {
                    nextChar();
                    return Token(TokenType::TTLogicalGE, tokenLocation, L">=");
                }
                return Token((TokenType)'>', tokenLocation, L">");
            case '<':
                nextChar();
                if(peekChar == '=')
                {
                    nextChar();
                    return Token(TokenType::TTLogicalLE, tokenLocation, L"<=");
                }
                if(peekChar == '>')
                {
                    nextChar();
                    return Token(TokenType::TTLogicalNE, tokenLocation, L"<>");
                }
                return Token((TokenType)'<', tokenLocation, L"<");
            case '\'':
                while(peekChar != '\n' && peekChar != EOF)
                    nextChar();
                continue;
            default:
                handleError(location, L"invalid character");
                return Token(TokenType::TTEOF, tokenLocation);
            }
            assert(false);
            break;
        }
    }
};

#endif // TOKENIZER_H_INCLUDED
