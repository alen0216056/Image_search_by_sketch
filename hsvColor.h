#ifndef __HSVCOLOR_H__
#define __HSVCOLOR_H__

#include <iostream>
#include <vector>
using namespace std;

class hsvColor
{
public:
	int h, s, v;

	//from hsv
	hsvColor(int, int, int);
	hsvColor(int);
	
	static void init();
	static bool isHSVDntCare(int,int,int);
	int toInt() const;
	vector<double> real() const;
	double operator-(const hsvColor&) const;
	void print(ostream&) const;
	void save(ostream&) const;
};

ostream& operator<<(ostream &os, const hsvColor &rhs);



#endif