#include "stdafx.h"
#include "colorMap.h"
#include "labColor.h"

#include <map>
#include <cmath>
#include <iostream>
#include <iomanip>

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

void propagate(const vector<double> &ori, vector<double> &res, int c, int x, int y, int depth, int size)
{
	for (int i = x - depth; i <= x + depth; i++)
	{
		int rem = depth - abs(x - i);
		if (rem == 0)
		{
			if (i >= 0 && i < size)
				res[i*size + y] = max(res[i*size + y], ori[x*size + y] * decayFunc(abs(i - x)));
				//res[i*size + y] += ori[x*size + y] * decayFunc(abs(i - x));
		}
		else
		{
			for (int j = y - rem; j <= y + rem; j++)
				if (i < 0 || j < 0 || i >= size || j >= size)
					continue;
				else
					res[i*size + j] = max(res[i*size + j],ori[x*size + y] * decayFunc(abs(i - x) + abs(j - y)));
					//res[i*size + j] += ori[x*size + y] * decayFunc(abs(i - x) + abs(j - y));
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

	for (auto& i : cv)
	{
		labColor ci(i.color);
		for (auto& j : rhs.cv)
		{
			labColor cj(j.color);
			if (ci-cj > 0.0)
				ans += (ci-cj) * inner_product(i.map, j.map, 64);
		}
	}

	return ans;
}

void colorMap::debugShow(const colorMap& rhs, ostream & out) const
{
	double ans = 0.0;
	vector<pair<double, pair<const colorWithMap*, const colorWithMap*> > > debug;

	for (auto& i : cv)
	{
		labColor ci(i.color);
		for (auto& j : rhs.cv)
		{
			labColor cj(j.color);
			double res = (ci - cj) * inner_product(i.map, j.map, 64);
			if (ci - cj > 0.0 && res >= 0.01)
				debug.push_back(pair<double, pair<const colorWithMap*, const colorWithMap*> >(res, pair<const colorWithMap*, const colorWithMap*>(&i, &j)));
			if (ci - cj > 0.0)
				ans += (ci - cj) * inner_product(i.map, j.map, 64);
		}
	}
	
	sort(debug.rbegin(), debug.rend());

	for (auto &p : debug)
	{
		labColor i = p.second.first->color, j = p.second.second->color;
		out << i << "," << j << ", => , " << (i - j) << " , * , " << inner_product(p.second.first->map, p.second.second->map, 64) << " , = , " << p.first << endl;
	} 

	out << "result , " << ans << endl;
}

colorMap::colorMap(string &img, bool intention) : fileName(img)
{
	const int GRID = 8;
	const int INF = 1 << 30;
	
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

	int gridWidth = cols / GRID + 1;
	int gridHeight = rows / GRID + 1;
	int gridSize = gridWidth*gridHeight;

	vector<vector<vector<colorEntity> > > colorFreq(GRID, vector<vector<colorEntity> >(GRID, vector<colorEntity>(1080)));

	for (int i = 0; i < GRID; i++)
		for (int j = 0; j < GRID; j++)
			for (int k = 0; k < 1080; k++)
					colorFreq[i][j][k] = colorEntity(k);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			int r, g, b;
			b = data[i*step + 3 * j + 0];
			g = data[i*step + 3 * j + 1];
			r = data[i*step + 3 * j + 2];

			labColor lab(r,g,b);
			
			if (intention || !labColor::isDntCare(r, g, b))
				colorFreq[i / gridHeight][j / gridWidth][lab.toInt()].freq++;
		}

	for (int i = 0; i < GRID; i++)
		for (int j = 0; j < GRID; j++)
			sort(colorFreq[i][j].begin(), colorFreq[i][j].end());
	

	vector<vector<double> > cvTmp(1080, vector<double>(GRID*GRID, 0.0));
	vector<bool> cvTmpEmpty(1080,true);

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
					last = c.freq;
					cnt+=c.freq;
				}


			for (colorEntity& c : colorFreq[i][j])
				if (c.freq * 2 < last)
					break;
				else
				{
					cvTmp[c.color][i * GRID + j] = intention ? (double)c.freq / cnt : 1.0;
					cvTmpEmpty[c.color] = false;
					last = c.freq;
				}
		}
	
	for (int i = 0; i < 1080; i++)
	{
		if (!cvTmpEmpty[i])
			cv.push_back(colorWithMap(i,cvTmp[i]));
	}

	//intention map
	if (intention)
	{
		const int propDepth = 1;

		vector<colorWithMap> tmp;
		vector<double> clearMap(GRID*GRID, 0.0);

		for (auto& map : cv)
			tmp.push_back(colorWithMap(map.color, clearMap));

		for (int i = 0; i < cv.size(); i++)
			for (int j = 0; j < GRID; j++)
				for (int k = 0; k < GRID; k++)
					if (cv[i].map[j*GRID+k] > 0)
						propagate(cv[i].map, tmp[i].map, cv[i].color, j, k, propDepth,GRID);	

		cv = tmp;
	}
	cvReleaseImage(&src);
}

void colorMap::show(string& title)
{
	src = cvLoadImage(fileName.c_str(), 1);
	cvShowImage(title.c_str(), src);
	cvWaitKey(0);
	cvDestroyWindow(title.c_str());
	cvReleaseImage(&src);
}

void colorMap::printColors(ostream& out)
{
	for (auto& i : cv)
	{
		out << "color " << labColor(i.color) << endl;

		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
				out << setprecision(3) << i.map[j * 8 + k] << ",";
			out << endl;
		}
		out << endl;
	}
}