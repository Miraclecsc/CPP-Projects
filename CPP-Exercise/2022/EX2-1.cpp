/*
编写程序计算正整数因子个数，并列出其所有的因子。分别采用枚举法与因式分解两种方法求解正整数的因子。

实践中需要定义两个函数，分别使用枚举法与因式分解法对读入的正整数求解其所有因子，然后输出因子数量，并按照从小到大的顺序输出其所有因子。

main函数中，用户提供输入、输出如下：
    程序的输入为一行：一个正整数；
    程序的输出为两行：第一行为枚举法结果，第二行为因式分解法结果。

注意：（1）输出时，同一行的数字之间以一个空格分隔，最后一个数字后没有空格。
      (2)每一行末尾除了回车，均无不可见字符。
      (3) 除了特殊说明外，课程作业题目的输入、输出的最后一行均无回车。
      (4) 针对因式分解法，程序至少需要包含因式分解、因子组合相乘、结果排序三个单独函数，缺少相关函数及调用，则第二问不得分。
      (5) 输出字符数需统计'\n'等不可见字符，不统计第一行输出。

测试输入：
10
预期输出：
4 1 2 5 10
4 1 2 5 10

测试输入：
1
预期输出：
1 1
1 1
*/

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

int n = 0;

class Sulotion_one
{
private:
    int n;
    stringstream ss;
public:
    Sulotion_one(int _n) : n(_n), ss()
    {
        int num = 0;
        ss.str("");
        for(int i = 1; i < n; i++)
        {
            if(n % i == 0)
            {    
                num++;
                ss << i << " ";
            }
        }
        num++;
        ss << n;

        cout << num << " " << ss.str() << endl;

    }
};
class Sulotion_two
{
private:
    int n;
    stringstream ss;
    vector<int> prime;
    vector<vector<int>> factor;
    vector<int> array;

    void init_prime()
    {
        for(int i = 2; i <= n; i++)
        {
            while(n % i == 0)
            {
                n /= i;
                prime.push_back(i);
            }
        }
    }
    void init_factor()
    {
        for(int i = 0; i < pow(2, prime.size()); i++)
        {
            vector<int> temp;
            for(int j = 0; j < prime.size(); j++)
            {
                if(i & (1 << j))
                {
                    temp.push_back(prime[j]);
                }
            }
            if(temp.size() != 0)
                factor.push_back(temp);
        }
    }
    void init_array()
    {
        array.push_back(1);
        for(int i = 0; i < factor.size(); i++)
        {
            int temp = 1;
            for(int j = 0; j < factor[i].size(); j++)
            {
                temp *= factor[i][j];
            }
            array.push_back(temp);
        }
        sort(array.begin(), array.end());
        auto it = unique(array.begin(), array.end());
        array.erase(it, array.end());
    }

public:
    Sulotion_two(int _n) : n(_n), ss()
    {
        ss.str("");
        init_prime();
        init_factor();
        init_array();
        
        for(int i = 0; i < array.size() - 1; i++)
        {
            ss << array[i] << " ";
        }
        ss << array[array.size() - 1];
        cout << array.size() << " " << ss.str();
    }

};

int main()
{
    cin >> n;
    Sulotion_one s1(n);
    Sulotion_two s2(n);
    return 0;
}