#ifndef __COLORMAP_H__
#define __COLORMAP_H__

#include <cv.h>
#include <highgui.h>
#include <opencv2\highgui\highgui.hpp>
#include <highgui/highgui.hpp>

#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct colorWithMap
{
	int color;
	vector<double> map;

	colorWithMap(int c, const vector<double> &m) : color(c), map(m){}

	bool operator<(const colorWithMap& rhs) const
	{
		return color < rhs.color;
	}
};

class colorMap
{
private:
	IplImage *src;
	
public:
	string fileName;
	vector<colorWithMap> cv;
	colorMap(string&,bool = true);
	double operator- (const colorMap&) const;
	void show(string&);
	void printColors(ostream& = cout);
	void debugShow(const colorMap&, ostream& = cout) const;
};


#endif