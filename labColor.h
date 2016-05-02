#ifndef __LABCOLOR_H__
#define __LABCOLOR_H__

#include "color.h"

#include <iostream>
using namespace std;

class labColor : public color
{
private:
public:
	const static int number = 15;
	int l, a, b;
	labColor();
	labColor(int, int, int);
	labColor(int, int, int,int);//test
	labColor(int);
	virtual double operator-(const labColor&) const;
	virtual int toInt() const;
	static void init();
	bool isWhite() const;
	labColor real() const;
	virtual void print(ostream&) const;
};

ostream& operator<<(ostream &os, const labColor &rhs);

#endif