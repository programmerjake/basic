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
#include <cmath>
#include "string_cast.h"
#include "location.h"
#include "error.h"

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
    TTArray,
    TTAs,
    TTAsc,
    TTATn,
    TTBoolean,
    TTByRef,
    TTByVal,
    TTCase,
    TTCast,
    TTCBool,
    TTCDbl,
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
    TTNext,
    TTNot,
    TTOct,
    TTOf,
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
private:
    static bool iswodigit(std::wint_t ch)
    {
        return ch >= '0' && ch <= '7';
    }
public:
    double getFloatingPointLiteralValue() const
    {
        if(svalue.empty())
            return NAN;
        size_t stringIndex = 0;
        int peekChar = svalue[stringIndex++];
        auto getChar = [&]()->int
        {
            int retval = peekChar;
            if(stringIndex >= svalue.size())
                peekChar = EOF;
            else
                peekChar = svalue[stringIndex++];
            return retval;
        };
        double retval = 0;
        if(peekChar == '&')
        {
            getChar();
            if(peekChar == 'h' || peekChar == 'H')
            {
                getChar();
                if(!std::iswxdigit(peekChar))
                    return NAN;
                while(std::iswxdigit(peekChar))
                {
                    int digit;
                    if(std::iswdigit(peekChar))
                        digit = peekChar - '0';
                    else if(std::iswlower(peekChar))
                        digit = peekChar - 'a' + 0xA;
                    else
                        digit = peekChar - 'A' + 0xA;
                    retval *= 0x10;
                    retval += digit;
                    getChar();
                }
                if(peekChar == '_')
                    return NAN;
                if(peekChar != EOF)
                    return NAN;
                return retval;
            }
            else if(iswodigit(peekChar) || peekChar == 'o' || peekChar == 'O')
            {
                if(peekChar == 'o' || peekChar == 'O')
                    getChar();
                if(!iswodigit(peekChar))
                {
                    return NAN;
                }
                while(iswodigit(peekChar))
                {
                    int digit = peekChar - '0';
                    retval *= 8;
                    retval += digit;
                    getChar();
                }
                if(peekChar != EOF)
                    return NAN;
                return retval;
            }
            return NAN;
        }
        bool gotAnyIntegerDigits = false;
        while(std::iswdigit(peekChar))
        {
            int digit = getChar() - '0';
            retval *= 10;
            retval += digit;
            gotAnyIntegerDigits = true;
        }
        if(peekChar == '.')
        {
            getChar();
            double factor = 1;
            if(!gotAnyIntegerDigits && !std::iswdigit(peekChar))
            {
                return NAN;
            }
            while(std::iswdigit(peekChar))
            {
                int digit = getChar() - '0';
                factor *= 0.1;
                retval += digit * factor;
            }
        }
        if(peekChar == 'e' || peekChar == 'E' || peekChar == 'f' || peekChar == 'F')
        {
            getChar();
            bool isExponentNegative = false;
            if(peekChar == '+' || peekChar == '-')
                isExponentNegative = (getChar() == '-');
            if(!std::iswdigit(peekChar))
                return NAN;
            int exponent = 0;
            while(std::iswdigit(peekChar))
            {
                int digit = getChar() - '0';
                if(exponent >= 10000000)
                    continue;
                exponent *= 10;
                exponent += digit;
            }
            if(isExponentNegative)
                exponent = -exponent;
            retval *= std::pow(10.0L, exponent);
        }
        if(peekChar != EOF)
            return NAN;
        return retval;
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

class Tokenizer final
{
    Tokenizer(const Tokenizer &rt) = delete;
    void operator =(const Tokenizer &rt) = delete;
    Location location;
    std::shared_ptr<InputStream> input;
    int peekChar;
    bool isExtraChar = false; // used for duplicating end line characters for interactive input
    bool isInteractive;
    bool gotEof = false;
    static void handleError(Location location, const std::wstring &msg)
    {
        throw TokenizerError(location, msg);
    }
    static constexpr size_t TabWidth = 8;
    void nextChar()
    {
        if(isExtraChar)
        {
            isExtraChar = false;
            return;
        }
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
        if(peekChar == '\n' && isInteractive)
            isExtraChar = true; // if it's interactive input duplicate end line character so it finishes parsing this line
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
    Token readLiteralType(Token token)
    {
        if(token.type != TokenType::TTIntegerLiteral)
            handleError(location, L"can not use a literal type on a floating-point number");
        std::wstring literalType = L"";
        token.addChar(getChar());
        Location literalTypeLocation = location;
        if(peekChar == 'u' || peekChar == 'U')
        {
            literalType += L"U";
            token.addChar('U');
            nextChar();
        }
        else if(peekChar == 'b' || peekChar == 'B')
        {
            token.svalue += L"U8";
            nextChar();
            return token;
        }
        while(std::iswdigit(peekChar))
        {
            literalType += (wchar_t)peekChar;
            token.addChar(getChar());
        }
        if(literalType != L"U8" && literalType != L"8" && literalType != L"U16" && literalType != L"16" && literalType != L"U32" && literalType != L"32" && literalType != L"U64" && literalType != L"64")
            handleError(literalTypeLocation, L"unknown literal type");
        return token;
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
                if(peekChar == '_')
                    return readLiteralType(retval);
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
                if(peekChar == '_')
                    return readLiteralType(retval);
                return retval;
            }
            retval.type = (TokenType)retval.svalue[0];
            if(peekChar == '_')
                return readLiteralType(retval);
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
                retval.type = TokenType::TTDoubleLiteral;
                break;
            default:
                assert(false);
            }
        }
        else if(peekChar == '_')
            return readLiteralType(retval);
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
            {L"Array", TokenType::TTArray},
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
            {L"CDbl", TokenType::TTCDbl},
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
            {L"Next", TokenType::TTNext},
            {L"Not", TokenType::TTNot},
            {L"Oct", TokenType::TTOct},
            {L"Of", TokenType::TTOf},
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
        isInteractive = (input->getName() == L"stdin");
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
                if(gotEof)
                    return Token(TokenType::TTEOF, tokenLocation);
                gotEof = true;
                return Token((TokenType)'\n', tokenLocation, L"\n");
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
