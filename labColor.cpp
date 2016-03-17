#include "stdafx.h"
#include "labColor.h"
#include <iostream>
#include <cmath>

using namespace std;

double simTable[1080][1080];

double diff(const labColor&lhs, const labColor&rhs)
{
	int dl = (lhs.l - rhs.l);
	int da = (lhs.a - rhs.a);
	int db = (lhs.b - rhs.b);

	int sum = dl*dl + da*da + db*db;

	if (sum >= 6)
		return 0.0;

	return 1 - sqrt(sum) / sqrt(8.0);
}

void labColor::init()
{
	for (int i = 0; i < 1080; i++)
		for (int j = 0; j < 1080; j++)
		{
			simTable[i][j] = diff(labColor(i) , labColor(j));
		}
}

inline int min(int a, int b)
{
	return a<b ? a : b;
}

double f(double t)
{
	if (t > 0.008856)
		return pow(t, 1 / 3.0); 
	return 7.787*t + 16 / 116.0;
}

double rgb2xyz(int n)
{
	double t = n / 256.0;
	if (t > 0.04045)
		return pow((t + 0.055) / 1.055, 2.4);
	return t / 12.92;
}

labColor::labColor(int _r, int _g, int _b)
{
	const static double xn = 0.9515, yn = 1, zn = 1.0886;
	
	//const static int lStep = 100 / number, aStep = 1000/number, bStep = 400/number;
	//3*30*12
	double rr, gg, bb;
	rr = rgb2xyz((_r + 256) % 256);
	gg = rgb2xyz((_g + 256) % 256);
	bb = rgb2xyz((_b + 256) % 256);
	

	double x, y, z;
	
	x = 0.412453*rr + 0.357580*gg + 0.180423*bb;
	y = 0.212671*rr + 0.715160*gg + 0.072169*bb;
	z = 0.019334*rr + 0.119193*gg + 0.950227*bb;

	x /= xn;
	y /= yn;
	z /= zn;

	double _l, _a, _bb;

	_l = 116 * f(y) - 16;
	_a = 500 * (f(x) - f(y));
	_bb = 200 * (f(y) - f(z));

	l = _l / number;

	a = min(floor((_a + (number/2))/ number)  + 15,29);
	b = min(floor((_bb + (number / 2)) / number) + 6, 11);
}

labColor::labColor(int n)
{
	l = n / (30*12);
	a = n % (30*12) / 12;
	b = n % 12;
}

double labColor::operator-(const labColor&rhs) const
{
	return simTable[toInt()][rhs.toInt()];
}

int labColor::toInt() const
{
	return l * 30*12 + a * 12 + b;
}

bool labColor::isDntCare(int _r, int _g, int _b)
{
	return ((_r + 256) % 256 == 64) && ((_g + 256) % 256 == 52) && ((_b + 256) % 256 == 48);
}

ostream& operator<<(ostream &os, const labColor &rhs)
{
	os << "(" << rhs.l*34 + 16 << " " << (rhs.a-15)*34 << " " << (rhs.b-6)*34 << ")";
	return os;
}