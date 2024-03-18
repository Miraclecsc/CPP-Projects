/*
本关任务：预习计算器例子，并修改程序，使之能顺利运行。

相关知识

简单计算器的实现。

编程要求

右侧编辑器中已给出一个包含若干bug的简单计算器功能的实现代码。
根据提示，在右侧编辑器中修改、补充代码，使程序能够正常运行，实现基本的计算器功能。

提示

本题源自《C++程序设计原理与实践》一书，为锻炼各位同学调试、测试程序能力，请：（1）仅参考课本和课内ppt，无须借助其它网络资料，进行程序找错；（2）educoder平台调试功能不够完善，请各位同学借助本地调试器进行程序调试。
初始代码同时包含影响编译和功能逻辑的error；建议先于测试平台上修改代码使其能够正常编译运行，之后进行功能完善；
无需修改头文件；
根据初始代码注释，最后键入字符'q'后程序退出；统一起见，本关卡中的所有测试输入（正常输入或异常输入，例如除零计算）都将以字符'q'作为结束（参考测试输入示例）。
测试说明

平台会对你编写的代码进行测试：

测试输入：
1-2*6;
q
预期输出：
=-11

测试输入：
3.2/0;
q
预期输出：
error: divide by zero
*/
#include"std_lib_facilities.h"


class Token{
public:
    char kind;       
    double value;     
    Token(char ch) : kind(ch), value(0) {}
    Token(char ch, double val) : kind(ch), value(val) {}
};

class Token_stream {
public:
    Token_stream();   
    Token get();     
    void putback(Token t);    
private:
    bool full;        
    Token buffer;  
};


Token_stream::Token_stream() : full(false), buffer(0) {}

void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;      
    full = true;      
}

Token Token_stream::get()
{
    if (full) {    
        full = false;
        return buffer;
    }
    char ch;
    cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)
    switch (ch) {
    case ';':   
    case 'q':    
    case '(': 
    case ')': 
    case '+': 
    case '-': 
    case '*': 
    case '/':
        return Token(ch);
    case '.':
    case '0': 
    case '1': 
    case '2': 
    case '3': 
    case '4':
    case '5': 
    case '6': 
    case '7': 
    case '8': 
    case '9':
    {
        cin.putback(ch);        
        double val;
        cin >> val;              
        return Token('8', val);
    }
    default:
        error("Bad token");
    }
}


Token_stream ts;       


double expression();   

double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(':    
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
            return d;
    }
    case '8':            
        return t.value;  
    case '-':
        return - primary();
    case '+':
        return primary();
    default:
        error("primary expected");
    }
}


double term()
{
    double left = primary();
    Token t = ts.get();  

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
            ts.putback(t);   
            return left;
        }
    }
}

double expression()
{
    double left = term();      
    Token t = ts.get();       

    while (true) {
        switch (t.kind) {
        case '+':
            left += term();    
            t = ts.get();
            break;
        case '-':
            left -= term();  
            t = ts.get();
            break;
        default:
            ts.putback(t);     
            return left;       
        }
    }
}

int main()
try
{
    double val=0;
    while (cin) {
        Token t = ts.get();
        while(t.kind==';') 
            t=ts.get();
        if (t.kind == 'q')
        {
            return 0;
        } 
        ts.putback(t);
        val = expression();
        cout<<"="<<val;   
    }
    return 0;
}
catch (exception& e) {
    cout << "error: " << e.what() ;
    return 1;
}
catch (...) {
    cout << "Oops: unknown exception!";
    return 2;
}

//------------------------------------------------------------------------------