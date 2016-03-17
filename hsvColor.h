#ifndef __HSVCOLOR_H__
#define __HSVCOLOR_H__

#include <iostream>
#include <vector>
using namespace std;

class hsvColor
{
private:
	
public:
	int h, s, v;
	static double simTable[192][192];

	//from rgb
	hsvColor(int, int, int);
	//from mix-hsv
	hsvColor(int);

	static bool isDntCare(int,int,int);
	int toInt() const;
	vector<double> getHSV() const;
	double operator-(const hsvColor&) const;
	bool operator== (const hsvColor&) const;
	void print();
	
};

ostream& operator<<(ostream &os, const hsvColor &rhs);



#endif