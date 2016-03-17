#include <iostream>
#include <cmath>
#include <vector>
#define PI 3.14159265358979323846

using namespace std;

double table[192][192];

double color_similarity(int i, int j)
{
	int h[2] = { (i / 16) * 30, (j / 16) * 30 };
	double s[2] = { (i % 16)/ 4 * (1 / 3.0), (j % 16) / 4 * (1 / 3.0) };
	double v[2] = { (i % 16 - 12.0*s[0]) * (1 / 3.0), (j % 16 - 12.0*s[1]) * (1 / 3.0) };
    cout << h[0] << "," << s[0] << "," << v[0] << endl;
    cout << h[1] << "," << s[1] << "," << v[1] << endl;

	return 1 - (1 / sqrt(5.0)) * \
		sqrt( (v[0] - v[1])*(v[0] - v[1]) + \
		(s[0] * cos(h[0] * PI / 180.0)- s[1] * cos(h[1] * PI / 180.0))*(s[0] * cos(h[0] * PI / 180.0) - s[1] * cos(h[1] * PI / 180.0)) + \
		(s[0] * sin(h[0] * PI / 180.0) - s[1] * sin(h[1] * PI / 180.0))*(s[0] * sin(h[0] * PI / 180.0) - s[1] * sin(h[1] * PI / 180.0)) );
}

void color_similarity_map()
{
	for (int i = 0; i < 192; i++)
		for (int j = 0; j < 192; j++)
			table[i][j] = color_similarity(i, j);
}

double inner_product(vector<double>& x, vector<double>& y, int size)
{
	double ans = 0.0;
	for (int i = 0; i < size; i++)
	{
		ans += x[i] * y[i];
	}
	return ans;
}

double similarity(vector<vector<double> >& source, vector<vector<double> >& target)
{
	double ans = 0.0;
	for (int i = 0; i < 192; i++)
	{
		for (int j = 0; j < 192; j++)
		{
			ans += table[i][j] * inner_product(source[i], target[j], 64);
		}
	}
	return ans;
}

int main()
{
	//color_similarity_map();

    cout << color_similarity(15, 79) << endl;
	cout << color_similarity(79, 143) << endl;

	return 0;
}
