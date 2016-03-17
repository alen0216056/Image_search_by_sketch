#include "stdafx.h"
#include "colorMap.h"
#include "labColor.h"

#include <map>
#include <cmath>
#include <iostream>
using namespace std;

struct colorEntity
{
	int color;
	int freq;

	colorEntity(int c = 0, int f = 0) :color(c), freq(f){}

	bool operator<(const colorEntity& rhs) const
	{
		return this->freq > rhs.freq;
	}
};

IplImage* toHSV(IplImage* ori)
{
	IplImage *res = cvCreateImage(cvGetSize(ori), IPL_DEPTH_8U, 3);
	cvCvtColor(ori, res, CV_BGR2HSV);

	return res;
}

inline int abs(int a)
{
	return a > 0 ? a : -a;
}

inline double max(double a, double b)
{
	return a > b ? a : b;
}

inline double decayFunc(int n)
{
	return pow(0.5, n);
}

void propagate(const vector<vector<double> > &ori, vector<vector<double> > &res, int c, int x, int y, int depth, int size)
{
	for (int i = x - depth; i <= x + depth; i++)
	{
		int rem = depth - abs(x - i);
		if (rem == 0)
		{
			if (i >= 0 && i < size)
				//res[c][i*size + y] = max(res[c][i*size + y], ori[c][x*size + y] * decayFunc(abs(i - x)));
				res[c][i*size + y] += ori[c][x*size + y] * decayFunc(abs(i - x));
		}
		else
		{
			for (int j = y - rem; j <= y + rem; j++)
				if (i < 0 || j < 0 || i >= size || j >= size)
					continue;
				else
					//res[c][i*size + j] = max(res[c][i*size + j],ori[c][x*size + y] * decayFunc(abs(i - x) + abs(j - y)));
					res[c][i*size + j] += ori[c][x*size + y] * decayFunc(abs(i - x) + abs(j - y));
		}
		
	}
}

double inner_product(const vector<double>& x, const vector<double>& y, int size)
{
	double ans = 0.0;
	for (int i = 0; i < size; i++)
	{
		ans += x[i] * y[i];
	}
	return ans;
}

double colorMap::operator-(const colorMap& rhs) const
{
	double ans = 0.0;

	for (int i = 0; i < 1000; i++)
	{
		labColor ci(i);
		for (int j = 0; j < 1000; j++)
		{
			labColor cj(j);
			//if (ci-cj >= 0.85)
				ans += (ci-cj) * inner_product(cv[i], rhs.cv[j], 64);
		}
	}

	return ans;
}

void colorMap::debugShow(const colorMap& rhs) const
{
	double ans = 0.0;
	vector<pair<double, int> > debug;

	for (int i = 0; i < 1000; i++)
	{
		labColor ci(i);
		for (int j = 0; j < 1000; j++)
		{
			labColor cj(j);
			if ((ci-cj) * inner_product(cv[i], rhs.cv[j], 64) >= 0.01)
				debug.push_back(pair<double, int>((ci - cj) * inner_product(cv[i], rhs.cv[j], 64), i * 1000 + j));
			//if (ci - cj >= 0.75)
				ans += (ci - cj) * inner_product(cv[i], rhs.cv[j], 64);			
		}
	}

	sort(debug.rbegin(), debug.rend());

	for (auto &p : debug)
	{
		int i = p.second / 1000, j = p.second % 1000;
		labColor ci(i),cj(j);
		cout << ci << cj << " : " << (ci - cj) << " * " << inner_product(cv[i], rhs.cv[j], 64) << " = " << p.first << endl;
	}

	cout << ans << endl;
}

colorMap::colorMap(string &img, bool intention)
{
	const int GRID = 8;
	const int INF = 1 << 30;
	cv = vector<vector<double> >(1000, vector<double>(GRID*GRID,0.0));
	
	src = cvLoadImage(img.c_str(), 1);
	
	if (src == NULL)
	{
		cout << img << " not found" << endl;
		return;
	}
	//IplImage *hsv = toHSV(src);

	int rows = src->height;
	int cols = src->width;
	int step = src->widthStep;
	char *data = src->imageData;

	int lStep = 100 / 10;
	int aStep = 1000 / 10;
	int bStep = 400 / 10;

	int gridWidth = cols / GRID + 1;
	int gridHeight = rows / GRID + 1;
	int gridSize = gridWidth*gridHeight;

	vector<vector<vector<colorEntity> > > colorFreq(GRID,vector<vector<colorEntity> >(GRID,vector<colorEntity>(1000)));

	for (int i = 0; i < GRID; i++)
		for (int j = 0; j < GRID; j++)
			for (int k = 0; k < 1000; k++)
				colorFreq[i][j][k] = colorEntity(k);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			int r, g, b;
			b = data[i*step + 3 * j + 0];
			g = data[i*step + 3 * j + 1];
			r = data[i*step + 3 * j + 2];

			labColor lab(r,g,b);
			
			if (intention || !labColor::isDntCare(r,g,b))
			//if (intention || !(r == 64 && g == 52 && b == 48) )
				colorFreq[i / gridHeight][j / gridWidth][lab.toInt()].freq++;
		}

	for (int i = 0; i < GRID; i++)
		for (int j = 0; j < GRID; j++)
			sort(colorFreq[i][j].begin(), colorFreq[i][j].end());

	for (int i = 0; i < GRID; i++)
		for (int j = 0; j < GRID; j++)
		{
			int last = 1;
			int cnt = 0;
			for (colorEntity& c : colorFreq[i][j])
				if (c.freq * 2 < last)
					break;
				else
				{
					cv[c.color][i * GRID + j] = 1;
					last = c.freq;
					cnt++;
				}

				if (intention)
					for (int c = 0; c < 1000; c++)
						cv[c][i*GRID + j] /= cnt;
		}
	
	//intention map
	if (intention)
	{
		const int propDepth = 1;
		const double tolerenceL = 0.99, tolerenceR = 1.0;
		const double factor = 0.15;
		vector<vector<double> >tmp = vector<vector<double> >(1000, vector<double>(GRID*GRID, 0.0));

		for (int i = 0; i < 1000; i++)
			for (int j = 0; j < GRID; j++)
				for (int k = 0; k < GRID; k++)
					if (cv[i][j*GRID+k] > 0)
						propagate(cv, tmp, i, j, k, propDepth,GRID);	

		vector<vector<double> >tmp2 = tmp;
		/*
		for (int i = 0; i < 192; i++)
			for (int j = 0; j < 192; j++)
				if (color_similarity(i, j) >= tolerenceL && color_similarity(i, j) <= tolerenceR)
					for (int k = 0; k < GRID*GRID; k++)
						tmp2[i][k] -= factor*tmp[j][k];
		*/
		cv = tmp2;
	}

}

void colorMap::show(string& title)
{
	cvShowImage(title.c_str(), src);
	cvWaitKey(0);
	cvDestroyWindow(title.c_str());
}

void colorMap::printColors()
{
	for (int i = 0; i < 1000; i++)
	{
		bool print = false;

		for (int j = 0; j < 64; j++)
			if (cv[i][j] >= 0.01)
			{
				print = true;
				break;
			}
		if (print)
		{
			cout << "color " << labColor(i) << endl;

			for (int j = 0; j < 8; j++)
			{
				for (int k = 0; k < 8; k++)
					printf("%3f  ", cv[i][j*8+k]);
				puts("");
			}
		}
	}


}