#include "stdafx.h"
#include "hsvColor.h"
#include "cmath"

hsvColor::hsvColor(int _h, int _s, int _v)
{
	const static int hStep = 360 / 12;
	const static int sStep = 256 / 4;
	const static int vStep = 256 / 4;

	h = ((_h + 256) % 256 * 2 + hStep / 2) % 360 / hStep;
	s = (_s + 256) % 256 / sStep;
	v = (_v + 256) % 256 / vStep;

	if (s == 0 || v == 0)
		h = 0;
	if (h == 0 && v == 0)
		s = 0;
}

hsvColor::hsvColor(int n)
{
	h = n / 16;
	s = n % 16 / 4;
	v = n % 4;
}

int hsvColor::toInt() const
{
	return h * 16 + s * 4 + v;
}

vector<double> hsvColor::getHSV() const
{
	vector<double> res(3);
	res[0] = h * 30.0;
	res[1] = 0.125 + s*0.25;
	res[2] = 0.125 + v*0.25;

	return res;
}

double hsvColor::operator-(const hsvColor& rhs) const
{
	const static double PI = atan(1.0) * 4;

	int i = this->toInt();
	int j = rhs.toInt();

	if (simTable[i][j] >= -1e-3)
		return simTable[i][j];

	vector<double> myHsv = getHSV();
	vector<double> tarHsv = rhs.getHSV();

	double dh = myHsv[2] - tarHsv[2];
	double dx = myHsv[1] * cos(myHsv[0] * PI / 180.0) - tarHsv[1] * cos(tarHsv[0] * PI / 180.0);
	double dy = myHsv[1] * sin(myHsv[0] * PI / 180.0) - tarHsv[1] * sin(tarHsv[0] * PI / 180.0);
	double res = 1 - (1 / sqrt(5.0)) * sqrt(dh*dh + dx*dx + dy*dy);

	return simTable[i][j] = res;
}

bool hsvColor::isDntCare(int _h, int _s, int _v)
{
	const static int h = 15, s = 256 / 4, v = 256 / 4;
	return (abs((_h + 256) % 256 * 2 - h) <= 1) && (abs((_s + 256) % 256 - s) <= 1) && (abs((_v + 256) % 256 - v) <= 1);
}

bool hsvColor::operator==(const hsvColor& rhs) const
{
	return *this - rhs >= 0.99;
}

ostream& operator<<(ostream &os, const hsvColor &rhs)
{
	os << "(" << rhs.h * 30 << "," << 12.5 + rhs.s*25.0 << "," << 12.5 + rhs.v*25.0 << ")";
	return os;
}
