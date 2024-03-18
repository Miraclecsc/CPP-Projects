#include "std_lib_facilities.h"
#include <iostream>
#include <algorithm>
#include <cmath>
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
	double a, b, r;
	vector <pair<double, double> > points;
public:
	Solution();
};

Solution::Solution() : x0(0), y0(0), a(0), b(0), r(0)
{
	cin >> x0;
	double x, y;
	double xbar, ybar, n;
    double sumx, sumy, sumxy;
	xbar = ybar = n = sumx = sumy = sumxy = 0;
	while(cin >> x >> y)
	{
		points.push_back(make_pair(x, y));
		xbar += x;
		ybar += y;
		n++;
	}
	xbar /= n;
	ybar /= n;
	for(int i = 0; i < points.size(); i++)
	{
		sumx += (points[i].first - xbar) * (points[i].first - xbar);
		sumy += (points[i].second - ybar) * (points[i].second - ybar);
		sumxy += (points[i].first - xbar) * (points[i].second - ybar);
	}
	r = sumxy / sqrt(sumx * sumy);
	cout << init_numbers(r) << endl;
	if(abs(r) < 0.75)
	{
		cout << "error" << endl << "error" << endl;
		return;
	}
	a = sumxy / sumx;
	b = ybar - a * xbar;
    if (b > 0)
	    cout << "y=" << init_numbers(a) << "*x+" << init_numbers(b) << endl;
    else
        cout << "y=" << init_numbers(a) << "*x" << init_numbers(b) << endl;
	cout << init_numbers(a * x0 + b) << endl;
}

int main()
{
	Solution();
	return 0;
}