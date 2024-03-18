#include "std_lib_facilities.h"
#include <iostream>
#include <algorithm>
#include <vector>
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

class Solution
{
private:
    double x0, y0;
	vector<double> x_list;
	vector<double> y_list;
public:
	Solution();
};
Solution::Solution() : x0(0), y0(0)
{
	cin >> x0;
	double x, y;
	while (cin >> x >> y)
	{
		if(x == x0)
		{
            cout << init_numbers(y) << endl;
			return;
		}

		x_list.push_back(x);
		y_list.push_back(y);
	}
	for(int i = 0; i < x_list.size(); i++)
	{
		double temp = 1;
		for(int j = 0; j < x_list.size(); j++)
		{
			if(i != j)
			{
				temp *= (x0 - x_list[j]) / (x_list[i] - x_list[j]);
			}
		}
		y0 += temp * y_list[i];
	}
	cout << init_numbers(y0) << endl;
}


int main()
{
	Solution();
	return 0;
}