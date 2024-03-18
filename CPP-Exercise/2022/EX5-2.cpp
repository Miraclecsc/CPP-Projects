/*
任务描述

本关任务：在第一关卡基础上添加“统计输入/输出字符”的功能。

相关知识

输入、输出的字符串处理。

编程要求

基于第一关实现的简单计算器程序，完成对输入、输出的统计，并于合适位置打印统计结果。

测试说明

平台会对你编写的代码进行测试：

测试输入：
1-2*6;
q
预期输出：
8:4
=-11

测试输入：
3.2/0;
q
预期输出：
8:21
error: divide by zero

开始你的任务吧，祝你成功！
*/
#include"std_lib_facilities.h"
//------------------------------------------------------------------------------
stringstream is;
stringstream os;

class Token{
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value 
    Token(char ch)    // make a Token from a char
        :kind(ch), value(0) { }
    Token(char ch, double val)     // make a Token from a char and a double
        :kind(ch), value(val) { }
};

//------------------------------------------------------------------------------

class Token_stream {
public:
    Token_stream();   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
    :full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get()
{
    if (full) {       // do we already have a Token ready?
        // remove token from buffer
        full = false;
        return buffer;
    }

    char ch;
    is >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)
    switch (ch) {
    case ';':    // for "print"
    case 'q':    // for "quit"
    case '(': case ')': case '+': case '-': case '*': case '/':
        return Token(ch);        // let each character represent itself
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        is.putback(ch);         // put digit back into the input stream
        double val;
        is >> val;              // read a floating-point number
        return Token('8', val);   // let '8' represent "a number"
    }
    default:
        error("Bad token");
    }
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback() 

//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

// deal with numbers and parentheses
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(':    // handle '(' expression ')'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
            return d;
    }
    case '8':            // we use '8' to represent a number
        return t.value;  // return the number's value
    case '-':
        return - primary();
    case '+':
        return primary();
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
    double left = primary();
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);     // put t back into the token stream
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------

int main()
try
{
    double val = 0;
    string str;
    while(cin >> str)
    {
        is << str;
        if(str == "q")
        {
            break;
        }
        is << endl;
    }
    while(is)
    {
        Token t = ts.get();
        while(t.kind==';') t=ts.get();
        if (t.kind == 'q')
        {
            return 0;
        }
        ts.putback(t);
        val = expression();
        os << "=" << val;   
        cout << is.str().length() << ":" << os.str().length() << '\n' << os.str();
    }
    return 0;
}
catch (exception& e) {
    os << "error: " << e.what();
    cout << is.str().length() << ":" << os.str().length() << '\n' << os.str();
    return 1;
}
catch (...) {
    os << "Oops: unknown exception!";
    cout << is.str().length() << ":" << os.str().length() << '\n' << os.str();
    return 2;
}

//------------------------------------------------------------------------------