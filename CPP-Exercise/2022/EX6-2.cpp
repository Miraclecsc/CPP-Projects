/*
任务描述

在第一关基础上，添加阶乘功能。

本题源自《C++程序设计原理与实践》一书，为锻炼各位同学调试、测试程序能力，请：（1）仅参考课本和课内ppt，无须借助其它网络资料，进行程序找错；（2）educoder平台调试功能不够完善，请各位同学借助本地调试器进行程序调试。

相关知识

简单计算器的实现

编程要求

输入：多行输入流，总行数不超过10，总字符数不超过1000。详细输入格式请参照样例代码的注释部分。
输出：多行计算器输出。

注意：阶乘符号!仅能出现在数字或)后，需保证阶乘符号前的值为非负整数，否则输出!: invalid factorial。
例如：
0!的运算结果为1；
-1!的运算结果为-1；
(-1)!为错误输入，报错!: invalid factorial。
!为错误输入，报错primary expected。

测试说明

平台会对你编写的代码进行测试：

测试输入：
0!
;
quit
预期输出：
> = 1
> 
注意：最后一行的>后有空格。

开始你的任务吧，祝你成功！
*/

#include "std_lib_facilities.h"

int factorial(int n)
{
    if(n == 0)
        return 1;
    int ans = 1;
    for(int i = 1; i <= n; i++)
        ans *= i;
    return ans;
}

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
    Token(char ch,string n):kind(ch),name(n) { }
};

class Token_stream {
	bool full;
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();
	void unget(Token t) { buffer = t; full = true; }

	void ignore(char);
};

const char let = 'L';
const char quit = 'q';
const char print = ';';
const char number = '8';
const char name = 'a';

Token Token_stream::get()
{
	if (full) { full = false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case print:
	case '=':
    case quit:
	case '!':
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
	{	cin.unget();
	double val;
	cin >> val;
	return Token(number, val);
	}
	default:
		if (isalpha(ch)) {
			string s;
			s += ch;
			while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
			cin.unget();
			if (s == "let") return Token(let);
			if (s == "quit") return Token(name);
			return Token(name, s);
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin >> ch)
		if (ch == c) return;
}

struct Variable {
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names;

double get_value(string s)
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ", s);
}

void set_value(string s, double d)
{
	for (int i = 0; i <= names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ", s);
}

bool is_declared(string s)
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

Token_stream ts;

double expression();

double primary()
{
	double left;
	Token t = ts.get();
	switch (t.kind) {
	case '(':
	{	
		double d = expression();
		t = ts.get();
		if (t.kind != ')') error("'(' expected");
		left = d;
		break;
	}
	case '-':
		left = -primary();
		break;
	case '+':
		left = primary();
		break;
	case number:
		left = t.value;
		break;
	case name:
		left = get_value(t.name);
		break;
	default:
		error("primary expected");
	}
	t = ts.get();
	if(t.kind == '!')
	{
		if(left < 0 || left != int(left))
			error("!: invalid factorial");
		left = factorial(left);
	}
	else
		ts.unget(t);
	return left;
}

double term()
{
	double left = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{	
			double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression()
{
	double left = term();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}

double declaration()
{
	Token t = ts.get();
	if (t.kind != 'a') error("name expected in declaration");
	string name = t.name;
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of ", name);
	double d = expression();
	if (is_declared(name)) error(name, " declared twice");
	names.push_back(Variable(name, d));
	return d;
}

double statement()
{
	Token t = ts.get();
	switch (t.kind) {
	case let:
		return declaration();
	default:
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	while (cin) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t = ts.get();
		if (t.kind == quit) return;
		ts.unget(t);
		cout << result << statement() << endl;
	}
	catch (runtime_error& e) {
		cout << e.what() << endl;
		clean_up_mess();
	}
}

int main()

try {
	calculate();
	return 0;
}
catch (exception& e) {
	cout << "exception: " << e.what() << endl;
	return 1;
}
catch (...) {
	cout << "exception\n";
	return 2;
}