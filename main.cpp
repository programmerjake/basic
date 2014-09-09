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

int main(int argc, char **argv)
{
    wstring fileName = L"";
    if(argc > 1)
        fileName = string_cast<wstring>(argv[1]);
    if(fileName == L"-h" || fileName == L"--help" || fileName == L"-?")
    {
        cout << "Usage: basic [<inputfile.bas>]\n";
        return 0;
    }
    try
    {
        shared_ptr<InputStream> is;
        if(fileName != L"")
            is = make_shared<FileInputStream>(fileName);
        else
            is = make_shared<FileInputStream>();
        shared_ptr<AST::CodeBlock> code = parseAll(is);
        is = nullptr;
        shared_ptr<CodeWriter> cw;
#if 0
        cw = make_shared<CodeWriterDump>(shared_ptr<ostream>(&cout, [](ostream *){}));
        code->writeCode(*cw);
        cw->finish();
#endif
        cw = make_shared<CodeWriterC>(shared_ptr<ostream>(&cout, [](ostream *){}));
        code->writeCode(*cw);
        cw->finish();
    }
    catch(exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
