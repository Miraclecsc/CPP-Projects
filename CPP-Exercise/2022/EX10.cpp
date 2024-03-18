/*
任务描述

输入两棵树（数据结构），判断这两棵树是否同构。

相关知识

树（数据结构） 树同构的定义 树同构的判定方法

编程要求

（1）输入
    树以0-1字符串形式输入，字符串代表由1、0分别替代(、)的树的括号序表示。例如，11011000和11100100可表示一对同构的树。
（2）判断输入树的合法性和同构性
（3）输出
    输出包含两个部分：
    1. 输入字符数:输出字符数，输出字符数需统计\n等不可见字符，不统计第一行输出。
    2. 不合法树的提示或树的同构性判断；对于输入的一对字符串A、B：
    （a）若A、B中存在不合法的树括号序表示，则按序输出不合法提示。
    （b）若A、B均为合法的树括号序表示且二者同构，则输出“A and B are isomorphic”。
    （c）若A、B均为合法的树括号序表示且二者不同构，则输出“A and B are non-isomorphic”。

注意：
（1）测试用例的输入不存在空字符串或非0-1字符串。
（2）不合法提示信息以“Invalid tree(s):”开头，后接相应字符串。开头与字符串、字符串与字符串之间用空格隔开。

测试说明

平台会对你编写的代码进行测试：

测试输入：
110
1111
预期输出：
8:25
Invalid tree(s): 110 1111

测试输入：
11011000
11100100
预期输出：
17:36
11011000 and 11100100 are isomorphic

开始你的任务吧，祝你成功！
*/

#include "std_lib_facilities.h"

stringstream is;
stringstream os;

bool is_legal(string str)
{
    int num = 0;
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] == '1')
            num++;
        else
            num--;
        if(num < 0)
            return false;
        if(num == 0 && i != str.size() - 1)
            return false;
    }
    if(num)
        return false;
    return true;
}

string tree_isomorphic(string str)
{
    if(str.length() == 2)
        return str;
    int num = 0;
    vector<string> sons;
    string son = "";
    for(int i = 1; i < str.length() - 1; i++)
    {
        if(str[i] == '1')
            num++;
        else
            num--;
        son += str[i];
        if(num == 0)
        {
            son = tree_isomorphic(son);
            sons.push_back(son);
            son = "";
        }
    }
    sort(sons.begin(), sons.end());
    string ans = "1";
    for(int i = 0; i < sons.size(); i++)
        ans += tree_isomorphic(sons[i]);
    ans += "0";
    return ans;
}

string is_isomorphic(string str1,string str2)
{
    if(!is_legal(str1) || !is_legal(str2))
    {
        if(!is_legal(str1) && !is_legal(str2))
            return "Invalid tree(s): " + str1 + " " + str2;
        if(!is_legal(str1))
            return "Invalid tree(s): " + str1;
        return "Invalid tree(s): " + str2;
    }
    if(tree_isomorphic(str1) == tree_isomorphic(str2))
        return str1 + " and " + str2 + " are isomorphic";
    return str1 + " and " + str2 + " are non-isomorphic";
}

int main()
{
	string str1, str2;
    cin >> str1 >> str2;
    is << str1 << '\n' << str2;
    os  << is_isomorphic(str1, str2);
    cout << is.str().length() << ":" << os.str().length() << '\n' << os.str();
	return 0;
}
