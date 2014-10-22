#include <iostream>
#include <fstream>
#include <sstream>
#include "basic.parser.h"
#include "string_cast.h"
#include "code_writer_c.h"
#include "code_writer_dump.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <html5.h>
#endif // __EMSCRIPTEN__

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

void writeRawError(string msg)
{
#ifdef __EMSCRIPTEN__
    string new_msg = "";
    for(char ch : msg)
    {
        if(ch == '\\')
            new_msg += "\\\\";
        else if(ch == '\"')
            new_msg += "\\\"";
        else
            new_msg += ch;
    }
    emscripten_run_script(("(function(v){console.log(v);alert(v);})(\"" + new_msg + "\")").c_str());
#else
    cerr << msg << endl;
#endif // __EMSCRIPTEN__
}

void writeError(wstring msg)
{
    writeRawError("error: " + string_cast<string>(msg));
}

#ifdef __EMSCRIPTEN__
static int my_main(int argc, const char *const *argv);
static EM_BOOL main_callback(int, const EmscriptenMouseEvent *, void *)
{
    static const char *const argv[] = {"basic", "input.bas", nullptr};
    emscripten_run_script("document.getElementById('output').value=''; FS.writeFile('input.bas', document.getElementById('input').value)");
    if(0 == my_main(sizeof(argv) / sizeof(argv[0]) - 1, argv))
    {
        emscripten_run_script("document.getElementById('output').value = FS.readFile('input.bas.cpp', {encoding:'utf8'})");
    }
    return true;
}
int main(int argc, char **argv)
{
    emscripten_run_script("document.getElementById('canvas').parentNode.innerHTML='<textarea id=\"input\" rows=\"12\" style=\"white-space:nowrap;overflow:scroll;width:100%;\"></textarea><br/><button id=\"run_button\">Run</button>'");
    emscripten_set_click_callback("run_button", nullptr, false, main_callback);
    emscripten_exit_with_live_runtime();
    return 0;
}
static int my_main(int argc, const char *const *argv)
{
#else
int main(int argc, char **argv)
{
#endif // __EMSCRIPTEN__
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
        writeRawError(e.what());
        return 1;
    }
    return 0;
}
