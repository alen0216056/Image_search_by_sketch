#ifndef __LABCOLOR_H__
#define __LABCOLOR_H__

#include <iostream>
using namespace std;

class labColor
{
private:
public:
	const static int number = 15;
	int l, a, b;
	labColor(int, int, int);
	labColor(int, int, int,int);//test
	labColor(int);
	double operator-(const labColor&) const;
	int toInt() const;
	static void init();
	static bool isDntCare(int, int, int);

};

ostream& operator<<(ostream &os, const labColor &rhs);

#endif