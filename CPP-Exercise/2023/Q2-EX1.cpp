/*
任务描述

根据题目要求，设计类内成员函数，完成文本处理与单词统计。
具体要求如下：

按格式要求输出给定单词的出现次数；
相关知识

向量的使用。
编程要求

标准输入：
输入分为2行：
第一行： 英文文本，单个测试用例的单词数不超过200个。
   空格或标点（除连字符-下划线_外）均作为单词间的分割标志（如例中说明）。
   空格与任何单独的标点不为单词；
   仅大小写不同看作相同单词。
   例如：
   （空格）_（空格） 与  （空格）-（空格）不为单词；
   a_b记作一个单词；
   a-b记作一个单词；
   A_C与a_c记作同一个单词。
   a—（空格）与a_（空格）为单词a。

第二行：需要输出出现次数的单词（不超过10个) ，以空格隔开。

标准输出：

输出：给定单词的出现次数：
  输出之间以空格隔开。
  若输入第二行为：Am go life；
  则输出可为：（Am，3）（go，4）（life，1）。

测试说明

测试输入：
 Both the sender and receiver Create a UD Queue Pair and join a specified multicast group. If the join is successful, the sender must Create an Address Handle.
group pair create

预期输出：

(group,1) (pair,1) (create,2)

开始你的任务吧，祝你成功！

*/

#include "std_lib_facilities.h"
#include <map>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

void trans_name(string &s)
{
    string temp;
    for (int i = 0; i < s.size(); i++)
    {
        if(s[i] >= 'a' && s[i] <= 'z')
            temp += (s[i] - 'a' + 'A');
        else
            temp += s[i];
    }
    if(s[s.size() - 1] == '-' || s[s.size() - 1] == '_')
        temp.pop_back();
    s = temp;

}
bool is_divide(string s)
{
    for(int i = 0; i < s.size(); i++)
    {
        if(!isalnum(s[i]) && s[i] != '-' && s[i] != '_')
            return true;
    }
    return false;
}

int main()
{
    string text, words;
    getline(cin, text);
    getline(cin, words);

    vector<string> text_stream;
    vector<string> words_stream;

    istringstream text_iss(text);
    istringstream words_iss(words);

    string temp;
    while (text_iss >> temp)
    {
        text_stream.push_back(temp);
    }
    while (words_iss >> temp)
    {
        words_stream.push_back(temp);
    }

    map<string, int> table;
    for(auto i = text_stream.begin(); i != text_stream.end(); i++)
    {
        temp = *i;
        if(is_divide(temp))
        {
            vector<int> pos;
            pos.push_back(-1);
            for(int j = 0; j < temp.size(); j++)
            {
                if(!isalnum(temp[j]) && temp[j] != '-' && temp[j] != '_')
                    pos.push_back(j);
            }
            pos.push_back(temp.size());
            for(int j = 0; j < pos.size() - 1; j++)
            {
                string tmp = temp.substr(pos[j] + 1, pos[j + 1] - pos[j] - 1);
                if(tmp == "")
                    continue;
                trans_name(tmp);
                if(table.find(tmp) == table.end())
                    table.insert(make_pair(tmp, 1));
                else
                    table[tmp]++;
            }
        }
        else
        {
            trans_name(temp);
            if(table.find(temp) == table.end())
                table.insert(make_pair(temp, 1));
            else
                table[temp]++;
        }
    }
    
    for(int i = 0; i < words_stream.size(); i++)
    {
        string tmp = words_stream[i];
        trans_name(tmp);
        if(table.find(tmp) == table.end())
            cout << "(" << words_stream[i] << ",0) ";
        else
            cout << "(" << words_stream[i] << "," << table[tmp] << ") ";
    }
    return 0;
}