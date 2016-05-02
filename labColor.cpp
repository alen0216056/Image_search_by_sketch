#include "stdafx.h"
#include "labColor.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

double simTable[1274][1274];

double CIE94(const labColor&lhs, const labColor&rhs)
{
	labColor lreal = lhs.real();
	labColor rreal = rhs.real();

	int dl = (lreal.l - rreal.l);
	int da = (lreal.a - rreal.a);
	int db = (lreal.b - rreal.b);
	double c1 = sqrt(lreal.a*lreal.a + lreal.b*lreal.b);
	double c2 = sqrt(rreal.a*rreal.a + rreal.b*rreal.b);
	double dc = c1 - c2;
	double dhab = sqrt(max(0.0, da*da + db*db - dc*dc));
	double sl = 2;
	double sc = 1 + 0.048*c1;
	double sh = 1 + 0.014*c1;

	return sqrt((dl / sl)*(dl / sl) + (dc / sc)*(dc / sc) + (dhab / sh)*(dhab / sh)) / sqrt(10398.2);
}
/*
double CIE2000(const labColor&lhs, const labColor&rhs)
{
	const static double PI = atan(1)*4.0;

	labColor lreal = lhs.real();
	labColor rreal = rhs.real();

	double l1 = lreal.l, l2 = rreal.l;
	double a1 = lreal.a, a2 = rreal.a;
	double b1 = lreal.b, b2 = rreal.b;

	double lAvg = (l1+l2)/2.0;
	double c1 = sqrt(a1*a1+b1*b1);
	double c2 = sqrt(a2*a2+b2*b2);
	double cAvg = (c1+c2)/2.0;
	double g = (1-sqrt(pow(cAvg,7)/(pow(cAvg,7)+pow(25,7))))/2.0;
	double a1p = a1*(1+g);
	double a2p = a2*(1+g);
	double c1p = sqrt(a1p*a1p+b1*b1);
	double c2p = sqrt(a2p*a2p+b2*b2);
	double cpAvg = (c1p+c2p)/2.0;
	double h1p = atan2(b1 , a1p);
	if (h1p < 0)
		h1p += 2*PI;
	double h2p = atan2(b2 , a2p);
	if (h2p < 0)
		h2p += 2*PI;
	double hAvg;
	if (fabs(h1p-h2p) > PI)
		hAvg = (h1p + h2p + 2*PI)/2;
	else
		hAvg = (h1p + h2p) / 2;
	double t = 1 - 0.17*cos(hAvg - PI / 6) + 0.24*cos(2 * hAvg) + 0.32*(3 * hAvg + PI / 30) - 0.2*cos(4*hAvg-PI*(63/180));

	double dhp;

	if (fabs(h1p - h2p) <= PI)
		dhp = h2p-h1p;
	else if (h2p<=h1p)
		dhp = h2p-h1p+2*PI;
	else
		dhp = h2p-h1p-2*PI;
	double dl = l2-l1;
	double dcp = c2p-c1p;
	dhp = 2 * sqrt(c1p*c2p)*sin(dhp/2);
	double sl = 1 + (0.015*pow(lAvg-50,2)) /sqrt(20+pow(lAvg-50,2));
	double sc = 1+0.045*cAvg;
	double sh = 1+0.015*cAvg*t;
	double dTheta = (PI / 6)*exp(-pow((dhp-PI*(275/180))/25,2));
	double rc = 2 * sqrt(pow(dcp,7)/(pow(dcp,7)+pow(25,7)));
	double rt = -rc*sin(2*dTheta);

	return sqrt((dl/sl)*(dl/sl)+(dcp/sc)*(dcp/sc)+(dhp/sh)*(dhp/sh)+rt*(dcp/sc)*(dhp/sh));
}
*/

double diff(const labColor&lhs, const labColor&rhs)
{
	return CIE94(lhs,rhs);
}

void labColor::init()
{
	//double mi = 100;
	for (int i = 0; i < 1274; i++)
		for (int j = 0; j < 1274; j++)
			{
				simTable[i][j] = diff(labColor(i) , labColor(j));

				if (j < i)
					simTable[i][j] = simTable[j][i] = max(simTable[i][j], simTable[j][i]);
			}
	/*
	for (int i = 0; i < 1274; i++)
		for (int j = 0; j < 1274; j++)
			mi = min(mi, simTable[i][j]);
	cout << mi << endl;*/
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

labColor::labColor()
{

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

	l = min(floor(_l / number),6);
	a = min(floor((_a + (number/2))/ number) + 6,12);
	b = min(floor((_bb + (number / 2)) / number) + 7, 13);
}

labColor::labColor(int _r, int _g, int _b,int test)
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

	l = _l;

	a = _a;
	b = _bb;
}

labColor::labColor(int n)
{
	l = n / (13*14);
	a = n % (13*14) / 14;
	b = n % 14;
}

double labColor::operator-(const labColor&rhs) const
{
	return simTable[toInt()][rhs.toInt()];
}

int labColor::toInt() const
{
	return l * 13*14 + a * 14 + b;
}

bool labColor::isWhite() const
{
	int diff = abs(a - 6) + abs(b - 7);

	if (l == 0 || l == 6)
		return diff <= 1;
	return diff == 0;
}

labColor labColor::real() const
{
	labColor res;

	res.l = l*15+7;
	res.a = (a-6)*15;
	res.b = (b-7)*15;

	return res;
}

void labColor::print(ostream& out) const
{
	labColor res = real();
	out << "," << res.l << " " << res.a << " " << res.b << ")";
}

ostream& operator<<(ostream &os, const labColor &rhs)
{
	rhs.print(os);
	return os;
}
