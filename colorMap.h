#ifndef __COLORMAP_H__
#define __COLORMAP_H__

#include "grid.h"

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
	grid<double> gd;

	colorWithMap(int c, const grid<double> &g) : color(c), gd(g){}
	colorWithMap(int c, const vector<double> &vec) : color(c), gd(grid<double>(vec)){}
	bool operator<(const colorWithMap& rhs) const
	{
		if ( color != rhs.color )
			return color < rhs.color;
		return gd < rhs.gd;
	}

	bool operator==(const colorWithMap& rhs) const
	{
		return color == rhs.color && gd == rhs.gd;
	}

	bool operator!=(const colorWithMap& rhs) const
	{
		return !(*this == rhs);
	}
};

class colorMap
{
private:
	
public:
	string fileName;
	vector<colorWithMap> cv;

	colorMap(istream&);
	colorMap(string&,bool = true, bool fill = true);

	double operator- (const colorMap&) const;
	bool operator< (const colorMap&) const;
	bool operator== (const colorMap&) const;

	void show(string&);

	void save(ostream&) const;
	void printColors(ostream& = cout) const;
	double debugShow(const colorMap&, ostream& = cout) const;
};


#endif