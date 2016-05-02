#include "stdafx.h"
#include "hsvColor.h"
#include "cmath"

#include <algorithm>
using namespace std;

double simTable[336][336];

hsvColor::hsvColor(int _h, int _s, int _v)
{
	const static int hStep = 360 / 20;
	const static int sStep = 256 / 4;
	const static int vStep = 256 / 4;

	h = ((_h + 256) % 256 * 2 + hStep / 2) % 360;
	s = (_s + 256) % 256;
	v = (_v + 256) % 256;

	bool gray = s < 10;

	h /= hStep;
	s /= sStep;
	v /= vStep;

	if ( gray )
		h = 20;
}

hsvColor::hsvColor(int n)
{
	h = n / 16;
	s = n % 16 / 4;
	v = n % 4;
}

void hsvColor::init()
{
	const static double PI = atan(1.0) * 4;
	for (int i = 0; i < 336; i++)
		for (int j = 0; j < 336; j++)
		{
			hsvColor ci(i), cj(j);
			
			bool ciGray = ci.h == 20;
			bool cjGray = cj.h == 20;

			double dh = min(abs(ci.h-cj.h),20-abs(ci.h-cj.h));
			double ds = ci.s-cj.s; 
			double dv = ci.v-cj.v;

			if (ciGray != cjGray)
				dh = max(ci.v,cj.v);
			
			if (dh < 3)
				dh *= 0.6;
			simTable[i][j] = sqrt(dh*dh+ds*ds+dv*dv)/sqrt(54);
			/*
			vector<double> myHsv = ci.real();
			vector<double> tarHsv = cj.real()
			double dx = myHsv[1] * cos(myHsv[0] * PI / 180.0) - tarHsv[1] * cos(tarHsv[0] * PI / 180.0);
			double dy = myHsv[1] * sin(myHsv[0] * PI / 180.0) - tarHsv[1] * sin(tarHsv[0] * PI / 180.0);
			simTable[i][j] = (1 / sqrt(5.0)) * sqrt(dh*dh + dx*dx + dy*dy);*/
		}
}

int hsvColor::toInt() const
{
	return h * 16 + s * 4 + v;
}

vector<double> hsvColor::real() const
{
	vector<double> res(3);
	res[0] = h * 18.0;
	res[1] = 0.125 + s*0.25;
	res[2] = 0.125 + v*0.25;

	return res;
}

bool hsvColor::isHSVDntCare(int _h, int _s, int _v)
{
	return ((_h)+256) % 256 == 8 && ((_s)+256) % 256 == 64 && ((_v)+256) % 256 == 64;
}

double hsvColor::operator-(const hsvColor& rhs) const
{
	return simTable[this->toInt()][rhs.toInt()];
}

void hsvColor::print(ostream &out) const
{
	vector<double>res = real();
	out << "(" << res[0] << " " << res[1]*100 << " " << res[2]*100 << ")";
}

void hsvColor::save(ostream &file) const
{
	file << h << " " << s << " " << v << endl;
}

ostream& operator<<(ostream &os, const hsvColor &rhs)
{
	rhs.print(os);
	return os;
}
