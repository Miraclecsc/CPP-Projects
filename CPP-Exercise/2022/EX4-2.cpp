/*
任务描述

编写程序实现对一元多项式的计算，并输出计算结果。多项式系数与变量的值均为整数或小数（精确度为小数点后4位）。

相关知识

一元多项式解析与求值。
其中：
    解析：复用Ex2第二关程序对输入的多项表达式(如2+3x+5x+6x^2)进行解析并化简；
    求值：一元多项式的求值计算，带入变量的值(如x=1.1)计算多项式结果。

编程要求

实践中需要定义一个多项式类Polynomial，该类支持如下操作：
    (1) 根据用户输入的表达式识别并整理一个多项式至标准形式（系数的精度设为小数点后4位）；
    (2) 支持多项式计算操作，可以根据表达式与变量的值计算出多项式结果。

本题中需读入一个多项式及对应变量的值；代入变量值计算并输出多项式结果。

main函数中，用户提供输入、输出如下：
    程序的输入为两行：第一行为多项表达式，第二行为变量的值；
    程序的输出为一行计算结果。如果输入格式不满足要求，则输出error。具体样例参照测试用例。

注意：
   （1）本课程作业中，如无特殊说明，小数一般为理论计算后的结果截位所得，例如，计算的理论值为5.32167，则计算结果截取4位，输出形式为5.3216；-5.32167截取4位后，输出-5.3216。（请思考为什么会这样设定？）
    (2) 用户输入的合法表达式仅包含.，+，^，-符号和数字以及字母x，每一项之间用 +或- 隔开，每一项的指数为非负整数，系数为整数或小数，小数精度设置为小数点后4位（事实上，测试输入中系数不超过小数点后4位）；
    (3) 凡是输出中含有表达式的，都是标准形式并保留4位小数，例如输出格式为：1.0000x^2+1.0000；
    (4) 若结果只有一位常数项，如0，则输出为0.0000。
    (5) 输出的表达式中不带空格。
    (6) 尽可能复用Ex2的第二题程序（识别多项式），计算中可以引入数学库完成。

测试说明

测试输入：
-5x-1+2.2x^2
0
预期输出：
-1.0000

*/

/*
实践中需要定义一个多项式类Polynomial，该类支持如下操作：
    (1) 根据用户输入的表达式初始化一个多项式并将其化简为标准形式，系数的精度设为小数点后4位；
    (2) 支持标准多项式除法操作，可以根据被除式与除式计算出商式表达式和余式表达式。

本题中需读入两个多项式并进行化简，再将其做除法，输出商式表达式和余式表达式。

main函数中，用户提供输入、输出如下：
    程序的输入为两行：第一行表示被除式，第二行表示除式；
    程序的输出为四行：
        第一行为化简后的被除式；
        第二行为化简后的除式；
        第三行为商式；
        第四行为余式。如果输入格式不满足要求，或不满足除法运算法则，则后四行输出四行error。具体样例参照测试用例。

注意：
   （1）本课程作业中，如无特殊说明，小数一般为理论计算后的结果截位所得，例如，计算的理论值为5.32167，则计算结果截取4位，输出形式为5.3216；-5.32167截取4位后，输出-5.3216。（请思考为什么会这样设定？）
    (2) 用户输入的合法表达式仅包含.，+，^，-符号和数字以及字母x，每一项之间用 +或- 隔开，每一项的指数为非负整数，系数为整数或小数，小数精度设置为小数点后4位（事实上，测试输入中系数不超过小数点后4位）；
    (3) 凡是输出中含有表达式的，都是标准形式并保留4位小数，例如输出格式为：1.0000x^2+1.0000；
    (4) 若结果只有一位常数项，如0，则输出为0.0000。
    (5) 输出的表达式中不带空格。
    (6) 输出字符数需统计'\n'等不可见字符，不统计第一行输出。

测试说明

测试输入：
-5x-1+2x^2
x-3
预期输出：
2.0000x^2-5.0000x-1.0000
1.0000x-3.0000
2.0000x+1.0000
2.0000
上述结果代表：(2x^2-5x-1)/(x-3)=(2x+1)余2。
*/
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

string init_numbers(double a)
{
    string s = to_string(a);
    for(int i = 0; i < s.length(); ++i)
    {
        if(s[i] == '.')
        {
            s.erase(i + 5);
            break;
        }
    }
    return s;
}

class Polynimial
{
private:
    bool valid = true;
    vector<pair<int, double>> terms;   
    void init_terms(const string &expression); 
    void sort_terms();
public:
    Polynimial() = default;
    Polynimial(const string &expression);
    Polynimial(const Polynimial &p);
    bool is_valid();
    bool is_zero();
    void print_poly();
    double calculate(double x);
    friend pair<Polynimial, Polynimial> operator/(const Polynimial &p1, const Polynimial &p2);
};

void Polynimial::init_terms(const string &expression)
{
    terms.clear();
    int i = 0;
    double base = 1.0;
    bool flag = true;
    double a = 0.0;
    int exp = 0;

    if (expression[i] == '-')
    {
        flag = false;
        ++i;
    }
    for(; i < expression.length(); ++i)
    {
        if(isdigit(expression[i]))
        {
            int num = expression[i] - '0';
            if(base == 1.0)
            {
                a = a * 10 + num;
            }
            else
            {
                a += num * base;
                base /= 10;
            }
        }
        else if(expression[i] == '.')
        {
            if(base == 1.0)
            {
                base /= 10;
            }
            else
            {
                valid = false;
                return;
            }
        }
        else if(expression[i] == 'x')
        {
            if(i == 0 || expression[i - 1] == '+' || expression[i - 1] == '-')
            {
                a = 1.0;
            }
            if(i + 1 < expression.length() && expression[i + 1] == '^')
            {
                i++;
                if(i + 1 == expression.length() || !isdigit(expression[i + 1]))
                {
                    valid = false;
                    return;
                }
                
                while(i + 1 < expression.length() && isdigit(expression[i + 1]))
                {
                    exp = exp * 10 + expression[i + 1] - '0';
                    ++i;
                }
                if(i + 1 < expression.length() && expression[i + 1] != '+' && expression[i + 1] != '-')
                {
                    valid = false;
                    return;
                }
            }
            else
            {
                if(i + 1 < expression.length() && expression[i + 1] != '+' && expression[i + 1] != '-')
                {
                    valid = false;
                    return;
                }
                exp = 1;
            }
        }
        else if(expression[i] == '+' || expression[i] == '-')
        {
            if(i && !isdigit(expression[i - 1]) && expression[i - 1] != 'x')
            {
                valid = false;
                return;
            }
            a = flag ? a : -a;
            terms.emplace_back(make_pair(exp, a));
            flag = expression[i] == '+';
            a = 0.0;
            base = 1.0;
            exp = 0;
        }
        else
        {
            valid = false;
            return;
        }
    }
    a = flag ? a : -a;
    terms.emplace_back(make_pair(exp, a));
}


void Polynimial::sort_terms()
{
    sort(terms.begin(), terms.end(), [](pair<int, double> a, pair<int, double> b) { return a.first > b.first; });
        vector<pair<int, double>> temp;
        temp.clear();
        for (int i = 0; i < terms.size(); ++i)
        {
            int j = i;
            double coeff = terms[i].second;
            while (j + 1 < terms.size() && terms[j + 1].first == terms[i].first)
            {
                ++j;
                coeff += terms[j].second;
            }
            i = j;
            if (coeff < 1e-5 && coeff > -1e-5)
                continue;
            temp.emplace_back(terms[i].first, coeff);
        }
        terms.clear();
        terms = move(temp);
}

Polynimial::Polynimial(const string &expression)
{
    init_terms(expression);
    sort_terms();
}

Polynimial::Polynimial(const Polynimial &p)
{
    terms.clear();
    for(const auto &term: p.terms)
        terms.emplace_back(term);
}


pair<Polynimial, Polynimial> operator/(const Polynimial &p1, const Polynimial &p2)
{
    if(p1.terms[0].first < p2.terms[0].first)
    {
        return make_pair(Polynimial(), p1);
    }
    Polynimial quotient, remainder(p1);
    while(!remainder.terms.empty() && remainder.terms[0].first >= p2.terms[0].first)
    {
        int delta = remainder.terms[0].first - p2.terms[0].first;
        double r = remainder.terms[0].second / p2.terms[0].second;
        quotient.terms.emplace_back(make_pair(delta, r));
        for(const auto &term : p2.terms)
        {
            remainder.terms.emplace_back(make_pair(term.first + delta, term.second * (-r)));
        }
        remainder.sort_terms();
    }
    quotient.sort_terms();
    return make_pair(quotient, remainder);
}

bool Polynimial::is_valid()
{
    return valid;
}

bool Polynimial::is_zero()
{
    if(terms.empty())
        return true;
    return false;
}


void Polynimial::print_poly()
{
    if(terms.empty())
    {
        cout << "0.0000\n";
        return;
    }
    for(int i = 0; i < terms.size(); i++)
    {
        string s = init_numbers(terms[i].second);
        if(i && s[0] != '-')
        {
            s = "+" + s;
        }
        if(terms[i].first == 0)
            cout << s;
        else if(terms[i].first == 1)
            cout << s << "x";
        else
            cout << s << "x^" << terms[i].first;
    }
    cout << endl;
}

double Polynimial::calculate(double x)
{
    double sum = 0.0;
    for(const auto &term : terms)
    {
        sum += term.second * pow(x, term.first);
    }
    return sum;
}

int main()
{
    string exp;
    getline(cin, exp);
    Polynimial p(exp);
    double x = 0;
    cin >> x;

    if(!p.is_valid())
    {
        cout << "error" << endl;
        return 0;
    }
    cout << init_numbers(p.calculate(x)) << endl;
    return 0;
}