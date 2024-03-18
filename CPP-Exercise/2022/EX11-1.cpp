/*
任务描述

使用 STL 容器存储输入的一组字符串，并进行去重、排序处理。

相关知识

（1）C++ STL 容器的使用。
（2）简单的字符串处理。

编程要求

（1）通过键盘获得一组字符串，删除完全相等的重复元素。
（2）对剩余元素进行排序。
（3）输出排序后的元素。

注意

（1）输入的每一行代表一个字符串元素。
（2）排序操作通过对容器应用自定义的比较器实现。
（3）排序存在两条规则：
    a.大小写敏感的正序;
    b.大小写不敏感的逆序。
（4）大小写不敏感条件下的“相等”字符串，顺序关系等同输入顺序。
（5）根据规则a, b逐行输出排序后结果，结尾无多余换行。

测试说明

平台会对你编写的代码进行测试：

测试输入：
aaa
aaaa
aBaa
aaaa
Abaa

预期输出：
rule a:
Abaa
aBaa
aaa
aaaa
rule b:
Abaa
aBaa
aaaa
aaa

开始你的任务吧，祝你成功！
*/
#include "std_lib_facilities.h"
#include <algorithm>
#include <set>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
using namespace std;
bool cmp(string x, string y)
{
    transform(x.begin(), x.end(), x.begin(), ::toupper);
    transform(y.begin(), y.end(), y.begin(), ::toupper);
    return x > y;
}


int main()
{
    vector <string> vec1;
    string s;
    while(getline(cin,s))
    {
        auto iter = find(vec1.begin(), vec1.end(), s);
        if(iter != vec1.end())
            continue;
        vec1.push_back(s);
    }
    vector <string> vec2(vec1);
    reverse(vec2.begin(), vec2.end());
    sort(vec1.begin(), vec1.end());
    sort(vec2.begin(), vec2.end(), cmp);
    
    cout<<"rule a:\n";
    for(const auto &i : vec1)
        cout<<i<<'\n';

    cout<<"rule b:\n";
    for(const auto &i : vec2)
        cout<<i<<'\n';
  	return 0;
}