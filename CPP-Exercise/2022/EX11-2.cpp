/*
任务描述

实现一个针对二维坐标系中的坐标点集合的处理程序，支持在该点集中添加点、查找点、筛选点集、排序与输出。

相关知识

STL算法库，详情可以参考https://blog.csdn.net/u014183456/article/details/82835281。

编程要求

实践中需要使用STL容器存储一个二维坐标点的集合（点的横纵坐标均为整数），并支持如下操作：
（1）add n x1 y1 x2 y2 ... xn yn：在集合中添加n个坐标点，坐标分别为(x1, y1)、...、(xn, yn)，并对当前集合进行去重，输出去重后点的数量。
（2）find x0 y0：查找集合中是否存在点(x0, y0)，如果有则输出find，否则输出not find。
（3）filter x0 y0 d：对集合进行筛选，保留所有与点(x0, y0)的距离小于等于d的点，输出筛选后点的数量。
（4）sort x0 y0：对集合优先按照点到点(x0, y0)的距离升序排序，若距离相同则按x坐标升序排序，并输出排序结果。输出格式为(x1, y1), (x2, y2), ...，注意逗号后的空格。

main函数中，用户提供输入、输出如下：
    程序的输入为多行指令，不超过20行，指令格式如上所述。
    程序的输出的行数与输入指令行数相同，每一行指令的输出格式如上所述。如果输入指令不符合格式，则该行输出invalid input，并跳过该指令。

注意：
  （1）本次作业尽量使用STL算法库提供的算法，比如find、unique、partition、sort等。
  （2）在使用STL算法函数时通过传入自定义的比较操作来减少代码复杂度。

测试说明

测试输入：
add 3 1 1 2 2 3 3
find 1 1
filter 1 1 2
sort 3 3
预期输出：
3
find
2
(2, 2), (1, 1)

开始你的任务吧，祝你成功！
*/
#include <bits/stdc++.h>
using namespace std;
vector<pair<int,int>> mat;
int xx = 0,yy = 0;
bool cmp(pair<int,int> s,pair<int,int> t)
{
    int d1 = 0,d2 = 0;
    d1 = (s.first-xx)*(s.first-xx)+(s.second-yy)*(s.second-yy);
    d2 = (t.first-xx)*(t.first-xx)+(t.second-yy)*(t.second-yy);
    if(d1 != d2)
        return d1 < d2;
    else
        return s.first < t.first;
}
int main()
{
    /********** Begin **********/
    //读取测试输入
    char ch[10];
    while(cin>>ch)
    {
        string opt(ch);
        //1.
        if(opt == "add")
        {
            char s[1000];
            cin.getline(s,999);
            string str(s);
            stringstream is(str);
            int n;
            is>>n;
            int x[n], y[n];
            bool flag= true;
            for(int i=0;i<n;i++)
            {
                is>>x[i];
                if(is.eof())
                {
                    cout << "invalid input\n";
                    flag=false;
                    break;
                }   
                is>>y[i];
                if(is.eof() && i!=n-1)
                {
                    cout << "invalid input\n";
                    flag=false;
                    break;
                } 
            }
            if(!is.eof())
            {
                cout << "invalid input\n";
                flag= false;
            }
            if(flag)
            {
                int i = 0;
                while(i < n)
                {
                    auto iter = find(mat.begin(), mat.end(), make_pair(x[i], y[i]));
                    if(iter == mat.end())
                    {
                        mat.emplace_back(x[i], y[i]);
                    }
                    i++;
                }
                cout<<mat.size()<<'\n';
            }
        }
        //2.
        else if(opt == "find")
        {
            char s[1000];
            cin.getline(s,999);
            string str(s);
            stringstream is(str);
            int x,y;
            is >> x;
            if(is.eof())
            {
                cout << "invalid input\n";
                continue;
            }
            is >> y;
            if(!is.eof())
            {
                cout << "invalid input\n";
                continue;
            }
            auto iter = find (mat.begin(), mat.end(), make_pair(x,y));
            if(iter == mat.end())
            {
                cout << "not find\n";
            }
            else
            {
                cout << "find\n";
            }
        }
        //3.
        else if(opt == "filter")
        {
            char s[1000];
            cin.getline(s,999);
            string str(s);
            stringstream is(str);
            int x,y,d;
            is>>x;
            if(is.eof())
            {
                cout << "invalid input\n";
                continue;
            }
            is>>y;
            if(is.eof())
            {
                cout << "invalid input\n";
                continue;
            }
            is>>d;
            if(!is.eof())
            {
                cout << "invalid input\n";
                continue;
            }
            auto iter = mat.begin();
            while(iter != mat.end())
            {
                if((iter->first-x)*(iter->first-x)+(iter->second-y)*(iter->second-y)>d*d)
                {
                    iter = mat.erase(iter);
                    iter --;
                }
                iter++;
            }
            cout << mat.size()<<'\n'; 
        }
        //4.
        else if(opt == "sort")
        {
            char s[1000];
            cin.getline(s,999);
            string str(s);
            stringstream is(str);
            is >> xx;
            if(is.eof())
            {
                cout << "invalid input\n";
                continue;
            }
            is >> yy;
            if(!is.eof())
            {
                cout << "invalid input\n";
                continue;
            }   
            std::sort(mat.begin(), mat.end(), cmp);
            if(!mat.empty())
            {
            auto iter = mat.begin();
            /*for(; iter != mat.end()-1; iter++)
            {
                cout<<'('<< iter->first <<", "<< iter->second<<')'<<", ";
            }*/
            while(iter < mat.end()-1)
            {
                cout<<'('<< iter->first <<", "<< iter->second<<')'<<", ";
                iter++;
            }
            cout<<'('<< iter->first <<", "<< iter->second<<')'; 
            }
            cout<<endl;
        }
        //5.
        else
        {
            char s[1000];
            cin.getline(s,999);
            cout << "invalid input\n";
        }
    }
    //请打印输出结果


	/********** End **********/
  	return 0;
}