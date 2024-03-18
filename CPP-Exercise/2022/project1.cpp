/*
本课程我们会有两个小型的project。（1）第一个project是前期作业的扩充，期望各位同学利用STL对计算器功能予以扩充。（2）除了计算器的扩展功能，你是否可以利用其它语言机制改善计算器的代码？（3）Project 1截止时间同第二个Project截止时间，为第16周周五。预期我们将开展现场检查，询问两个Project程序中的更多的细节。

任务描述

在Ex5第2关“计算器的阶乘功能实现”的基础上，添加索引功能。

相关知识

简单计算器的实现，STL的应用。

编程要求

输入：第一行为多个带索引的表达式，以q结尾，如[chen] 3+2; [shen] 3+2; q，总字符数不超过1000。第二行为查询的目标索引。
输出：第一行是查询的索引；第二行及之后为查询的索引对应的表达式及结果。

注意：
（1）如果第二行无输入索引，则输出所有的计算结果。
（2）如果第二行为一个不存在的索引，则仅输出索引。
（3）输出按照索引的字典序排序，同一索引的表达式按照实际输出的字典序排序。
（4）输入中，一个索引后只有一个表达式，即格式固定为：[index]expression;。
（5）输入的多个表达式以q结尾，之后的内容为无效内容，不予计算。
（6）索引中可能存在空格，但不存在\n、[、]。索引头尾的空格忽略，即[ index ] 和 [index]一致。
（7）输出的索引与表达式前后均无空格。
（8）如果索引或表达式格式不正确，输出时忽略该行。
（9）小数计算结果保留六位有效数字（为保持和原程序一致）。

测试说明

平台会对你编写的代码进行测试：

测试输入：
[chen] 3+2; [chen] 5+7; [shen] 3+2; [shen] 2+6; q
chen
预期输出：
[chen]
3+2=5
5+7=12

测试输入：
[chen] 3+2; [chen] 5+7; [shen] 3+2; [shen] 2+6; q
预期输出：
[chen]
3+2=5
5+7=12
[shen]
2+6=8
3+2=5

开始你的任务吧，祝你成功！
*/
#include<bits/stdc++.h>
#include"std_lib_facilities.h"
using namespace std;
istringstream is;
struct Token
{
    char kind;
    double value;
    Token(char s):kind(s),value(0){}
    Token(char s, double val):kind(s),value(val){}

};
double fact(double val)
{
    double ans=1.0;
    int n = (int)val;
    if(n==0||n==1)
        ans=1;
    else 
        ans=fact(n-1)*n;
    return ans;
}
class Token_stream
{
    bool full;
    Token buffer;
public:
    Token_stream(): full(false),buffer(0){}
    Token get()
    {
        if(full)
        {
            full = false;
            return buffer;
        }
        char s;
        if(is>>s){
        switch(s)
        {
            case '(':
	        case ')':
	        case '+':
	        case '-':
            case '!':
	        case '*':
	        case '/':
	        case '=':
	        case 'q':
	        case ';':
                return Token(s);
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
                is.unget();
                double val;
                is>>val;
                return Token('8',val);
            }
            default:
                error("1");
        }
        }
        else
        {
            return Token('\0');
        }
    }
    void clear()
    {
        full = false;
        buffer = 0;
    }
    void putback(Token t)
    {
        if(full)
            error("0");
        full = true;
        buffer = t;
    }
    void ignore()
    {
        if(full && buffer.kind == ';')
        {
            full = false;
            return;
        }
        full = false;
        char s = 0;
        while(is>>s)
        {
            if(s == ';')
                return;
        }
    }
};
Token_stream ts;
map<string,vector<string>> table;
double expression();
double primary()
{
    Token t = ts.get();
    if(t.kind == '(')
    {
        double left = expression();
        t = ts.get();
        if(t.kind != ')')
            error("3");
        t = ts.get();
        if (t.kind == '!')
        {
            if(left < 0 || left != (int)left )
                error("4");
            return fact(left);
        }
        ts.putback(t);
        return left;
    }
    else if(t.kind == '+')
    {
        return primary();
    }
    else if(t.kind == '-')
    {
        return - primary();
    }
    else if(t.kind == '8')
    {
        double val = t.value;
        t = ts.get();
        if(t.kind == '!')
        {
            return fact(val);
        }
        ts.putback(t);
        return val;
    }
    else error("2");
}
double term()
{
    double left = primary();
    Token t = ts.get();
    while(1)
    {
        switch(t.kind)
        {
            case '*':
            {
                double right = primary();
                left *= right;
                t = ts.get();
                break;
            }
            case '/':
            {
                double right = primary();
                if(right == 0)
                    error("5");
                left /= right;
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
    while(1) {
        switch(t.kind) {
        case '+':
        {
            double right = term();
            left += right;    
            t = ts.get();
            break;
        }
            
        case '-':
        {
            double right = term();
            left -= right;    
            t = ts.get();
            break;
        }
            
        default:
            ts.putback(t);     
            return left;       
        }
    }
}

auto get_ans() -> std::pair<std::string, bool>
{
    while (is)
    {
        try
        {
            double result = expression();
            ts.clear();
            std::string ans;

            if (result == static_cast<int>(result))
            {
                ans = std::to_string(static_cast<int>(result));
            }
            else
            {
                ans = std::to_string(result);
                int pos = ans.find('.');
                int len = ans.length();
                int e = len - 1;
                while (e >= pos)
                {
                    if ((ans[e] >= '1' && ans[e] <= '9') || ans[e] == '.')
                        break;
                    e--;
                }
                ans = ans[e] == '.' ? ans.substr(0, e) : ans.substr(0, e + 1);
            }
            return std::make_pair(ans, true);
        }
        catch (const std::exception& e)
        {
            ts.ignore();
            return std::make_pair("E", false);
        }
    }
}


int file_input()
{
    bool flag = false;
    while(1)
    {
        if(flag)
            return 0;
        is.clear();
        char s;
        s = getchar();
        while(s == ' ')
            s = getchar();
        if(s == 'q')
        {
            while(s = getchar())
            {
                if(s == '\n')
                    return 1;
                if(s == EOF)
                {
                    flag = true;
                    return 0;
                }
            }
        }
        else
            cin.putback(s);
        string equ_index,equ;
        s = getchar();
        if(s != '[')
        {
            char c;
            while(c = getchar())
            {
                if(c == '[')
                    break;
                if(c == EOF)
                {
                    flag = true;
                    break;
                }
                if(c == 'q')
                {
                    c = getchar();
                    while(c !='\n' && c != EOF)
                        c = getchar();
                    if(c == '\n')
                        return 1;
                    if(c == EOF)
                        return 0;
                }
            }
            cin.putback(c);
            continue;
        }
        s = getchar();
        while(s == ' ')
            s = getchar();
        while(s != ']')
        {
            equ_index += s;
            s = getchar();
        }
        int len =  equ_index.length();
        int e = 0;
        int i = len-1;
        while(i>=0)
        {
            if(equ_index[i] != ' ')
            {
                e = i;
                break;
            }
            i--;
        }
        equ_index = equ_index.substr(0,e+1); 
        s = getchar();
        while(s == ' ')
            s = getchar();
        while(s !='[' && s != '\n' && s !='q' && s !=EOF)
        {
            equ +=s;
            s = getchar();
        }
        if(s == EOF)
            flag = true;
        else equ += s;
        len = equ.length();
        cin.putback(equ[len-1]);
        e = 0;
        i = len-1;
        while(i >=0)
        {
            if(equ[i] == ';')
            {
                e = i;
                break;
            }
            i--;
        }
        equ=equ.substr(0,e);
        len = equ.length();
        i = len-1;
        while(i>=0)
        {
            if(equ[i] != ' ')
            {
                e = i;
                break;
            }
            i--;
        }
        equ = equ.substr(0,e+1);
        len = equ.length();
        bool fg=true;
        i = len-1;
        while(i>=0)
        {
            if(equ[i] == ';')
            {
                fg = false;
                break;
            }
            i--;
        }
        if(!fg)
        {
            auto it = table.find(equ);
            if(it != table.end())
            {
                (*it).second.push_back("E");
                sort((*it).second.begin(),(*it).second.end());
            }
            else
            {
                vector<string> arr;
                arr.push_back("E");
                table.insert({equ_index,arr});
            }
            continue;
        }
        string tmp(equ);
        auto k = tmp.begin();
        while(k != tmp.end())
        {
            if((*k) == ' ')
                tmp.erase(k);
            else k++;
        }
        is.str(tmp);
        equ+='=';
        auto ans = get_ans();
        if(!ans.second)
        {
            auto it = table.find(equ_index);
            if(it != table.end())
            {
                (*it).second.push_back("E");
                sort((*it).second.begin(),(*it).second.end());
            }
            else
            {
                vector<string> arr;
                arr.push_back("E");
                table.insert({equ_index,arr});  
            }
            continue;
        }
        string ow = ans.first;
        double wo = stod(ow);
        ow = to_string(wo);
        if(fabs(100000*wo-int(1000*wo)*100)>=0.1)
            {
                string owo = ans.first.substr(0,7);
                int wow = abs(int(1000*wo));
                wow %= 10;
                owo += to_string(wow+int(wow/5));
                equ += owo;
            }
        else
            equ+=ans.first;
        auto it = table.find(equ_index);
        if(it!=table.end())
        {
            (*it).second.push_back(equ);
            sort((*it).second.begin(),(*it).second.end());
        }
        else
        {
            vector<string> arr;
            arr.push_back(equ);
            table.insert({equ_index,arr});
        }
    }
}
void file_prt1()
{
    auto i1 = table.begin();
    while(i1 != table.end())
    {
        cout<<'['<<(*i1).first<<']'<<endl;
        auto i2 = (*i1).second.begin();
        while(i2 != (*i1).second.end())
        {
            if(*i2 != "E")
                cout<<*i2<<endl;
            i2++;
        }
        i1++;
    }
}
void file_prt2()
{
    char s = getchar();
    while(s == ' ')
        s = getchar();
    if(s == EOF)
        file_prt1();
    else
    {
        string str;
        while(s != EOF && s != '\n')
        {
            str+=s;
            s = getchar();
        }
        int len = str.length();
        int e = 0;
        int i = len-1;
        while(i>=0)
        {
            if(str[i] != ' ')
            {
                e = i+1;
                break;
            }
            i--;
        }
        str = str.substr(0,e);
        auto i1 = table.find(str);
        cout<<'['<<str<<']'<<endl;
        if(i1!=table.end())
        {
            auto i2 = (*i1).second.begin();
            while(i2 != (*i1).second.end())
            {
                if(*i2 != "E")
                    cout<<*i2<<endl;
                i2++;
            }
        }
    }
}
int main()
{
    int n = file_input();
    switch(n)
    {
        case 0:file_prt1();break;
        case 1:file_prt2();break;
        default:break;
    }   
    return 0;
}