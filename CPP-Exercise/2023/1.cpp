#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
	double x, y, sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0, sum_y2 = 0;
	int n = 0;
	vector<pair<double, double>> points;

	// Read input
	cin >> x;
	while (cin >> x >> y) {
		points.push_back({x, y});
		sum_x += x;
		sum_y += y;
		sum_xy += x * y;
		sum_x2 += x * x;
		sum_y2 += y * y;
		n++;
	}

	// Calculate correlation coefficient
	double r = (n * sum_xy - sum_x * sum_y) / sqrt((n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y));

	// Check if correlation coefficient is valid
	if (abs(r) < 0.75) {
		cout << "error" << endl << "error" << endl;
		return 0;
	}

	// Calculate regression equation
	double a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
	double b = (sum_y - a * sum_x) / n;

	// Output results
	cout << fixed << setprecision(4) << r << endl;
	cout << "y=" << a << "x+" << b << endl;
	cout << a * x + b << endl;

	return 0;
}
