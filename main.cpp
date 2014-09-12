#include <iostream>
#include <fstream>
#include <sstream>
#include "basic.parser.h"
#include "string_cast.h"
#include "code_writer_c.h"
#include "code_writer_dump.h"

using namespace std;

class FileInputStream : public InputStream
{
    wstring fileName;
    shared_ptr<istream> input;
public:
    FileInputStream()
        : fileName(L"stdin"), input(&cin, [](istream *){})
    {
    }
    explicit FileInputStream(wstring fileName)
        : fileName(fileName)
    {
        string s = string_cast<string>(fileName);
        input = make_shared<ifstream>(s.c_str());
        if(!*input)
            throw ParserBase::ParseError("can't open " + s);
    }
    explicit FileInputStream(wstring fileName, shared_ptr<istream> input)
        : fileName(fileName), input(input)
    {
    }
    virtual int getChar() override
    {
        int ch = input->get();
        if(ch == EOF)
            return EOF;
        if(ch == '\r')
        {
            ch = '\n';
            if(input->peek() == '\n')
                input->get();
            return ch;
        }
        if(ch < 0x80)
            return ch;
        if((ch & 0xE0) == 0xC0)
        {
            ch &= 0x1F;
            ch <<= 6;
            ch |= input->get() & 0x3F;
            return ch;
        }
        if((ch & 0xF0) == 0xE0)
        {
            ch &= 0xF;
            ch <<= 6;
            ch |= input->get() & 0x3F;
            ch <<= 6;
            ch |= input->get() & 0x3F;
            return ch;
        }
        if((ch & 0xF1) == 0xF0)
        {
            ch &= 0x7;
            ch <<= 6;
            ch |= input->get() & 0x3F;
            ch <<= 6;
            ch |= input->get() & 0x3F;
            ch <<= 6;
            ch |= input->get() & 0x3F;
            return ch;
        }
        return ch;
    }
    virtual wstring getName() override
    {
        return fileName;
    }
};

struct MemoryInputStream : public FileInputStream
{
    explicit MemoryInputStream(string buffer)
        : FileInputStream(L"memory", make_shared<istringstream>(buffer))
    {
    }
    explicit MemoryInputStream(wstring fileName, string buffer)
        : FileInputStream(fileName, make_shared<istringstream>(buffer))
    {
    }
    explicit MemoryInputStream(wstring buffer)
        : FileInputStream(L"memory", make_shared<istringstream>(string_cast<string>(buffer)))
    {
    }
    explicit MemoryInputStream(wstring fileName, wstring buffer)
        : FileInputStream(fileName, make_shared<istringstream>(string_cast<string>(buffer)))
    {
    }
};

string getInput()
{
    return string(R"a(
Dim a As Integer
Dim b As ByRef Integer = a
Dim c As Single
c = b
)a").substr(1);
}

void dumpCode(shared_ptr<AST::Base> code)
{
    CodeWriterDump cw(shared_ptr<ostream>(&cout, [](ostream *){}));
    code->writeCode(cw);
    cw.finish();
}

void help()
{
    cout << "Usage: basic [<options>] [--] [<inputfile.bas>]\n\nOptions:\n-h\t\tShow this help.\n--help\n-?\n\n--dump\t\tDump AST to stdout.\n";
}

void writeError(wstring msg)
{
    cerr << L"error: " << string_cast<string>(msg) << endl;
}

int main(int argc, char **argv)
{
    wstring fileName = L"";
    bool useDump = false, gettingOptions = true;
    for(int i = 1; i < argc; i++)
    {
        wstring arg = string_cast<wstring>(argv[i]);
        if(gettingOptions && arg.substr(0, 1) == L"-")
        {
            if(arg == L"-h" || arg == L"--help" || arg == L"-?")
            {
                help();
                return 0;
            }
            else if(arg == L"--dump")
            {
                useDump = true;
            }
            else if(arg == L"--")
            {
                gettingOptions = false;
            }
            else
            {
                writeError(L"invalid option : " + arg);
                return 1;
            }
        }
        else if(fileName != L"")
        {
            writeError(L"can't specify two files at once");
            return 1;
        }
        else
            fileName = arg;
    }
    if(fileName.empty())
        fileName = L"-";
    try
    {
        shared_ptr<InputStream> is;
        if(fileName == L"-")
            is = make_shared<FileInputStream>();
        else
            is = make_shared<FileInputStream>(fileName);
        shared_ptr<AST::CodeBlock> code = parseAll(is);
        is = nullptr;
        if(useDump)
        {
            dumpCode(code);
            return 0;
        }
        shared_ptr<CodeWriter> cw;
        shared_ptr<ostream> os;
        if(fileName == L"-")
            os = shared_ptr<ostream>(&cout, [](ostream *){});
        else
            os = make_shared<ofstream>(string_cast<string>(fileName + L".cpp").c_str());
        if(!*os)
            throw runtime_error("error: can't open " + string_cast<string>(fileName + L".cpp") + "for output");
        cw = make_shared<CodeWriterC>(os);
        code->writeCode(*cw);
        cw->finish();
        cw = nullptr;
    }
    catch(exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
