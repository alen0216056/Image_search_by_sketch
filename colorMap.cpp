#include "stdafx.h"
#include "grid.h"
#include "colorMap.h"
#include "hsvColor.h"
#include "constants.h"

#include <map>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <queue>
#include <tuple>

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

double max(double a, double b)
{
	return a > b ? a : b;
}

double min(double a, double b)
{
	return a < b ? a : b;
}

double colorMap::operator-(const colorMap& rhs) const
{
	grid<double> ans(GRID,GRID);

	for (auto& i : cv)
	{
		grid<double> tmp(GRID,GRID);
		hsvColor ci(i.color);
		for (auto& j : rhs.cv)
		{
			hsvColor cj(j.color);

			double rlv = (ci-cj) < 0.3? 1-(ci-cj) : 0;

			if (rlv > 0.0)
				tmp.max(i.gd.mul(j.gd)*rlv);
		}
		ans = ans + tmp;
	}

	return ans.sum();
}

bool colorMap::operator< (const colorMap& rhs) const
{
	if (cv.size() != rhs.cv.size())
		return cv.size() < rhs.cv.size();

	for (int i = 0; i < cv.size(); i++)
		if ( cv[i] != rhs.cv[i] )
			return cv[i] < rhs.cv[i];

	return false;
}

bool colorMap::operator== (const colorMap& rhs) const
{
	for (auto& i : cv)
		for (auto& j : rhs.cv)
			if ( i != j )
				return false;
	return true;
}

double colorMap::debugShow(const colorMap& rhs, ostream & out) const
{
	grid<double> ans(GRID, GRID);

	for (auto& i : cv)
	{
		grid<double> tmp(GRID, GRID);
		hsvColor ci(i.color);
		for (auto& j : rhs.cv)
		{
			hsvColor cj(j.color);

			double rlv = (ci - cj) < 0.3 ? 1 - (ci - cj) : 0;
			grid<double> tmp2(GRID,GRID);
			if (rlv > 0.0)
			{
				tmp2 = i.gd.mul(j.gd);
				tmp2 = tmp2 * rlv;
				tmp.max(tmp2);
				//tmp.max(i.gd*j.gd*rlv);
			}
			
			if (rlv > 0.0)
			{
				out << ci << "," << cj << ", rlv = ," << rlv << endl;
				out << tmp << endl;
			}
			else
			{
				out << ci << "," << cj << ", rlv = ," << ci-cj << endl;
			}
		}
		ans = ans + tmp;

		out << ci << endl;
		out << tmp << endl;
	}

	out << "grid result" << endl;

	out << ans << endl;

	return ans.sum();
}

colorMap::colorMap(istream& file)
{
	string line;

	getline(file,line);
	fileName = line;

	getline(file,line);
	istringstream is(line);

	int color;
	vector<double> v(GRID*GRID);

	while (is >> color)
	{
		for (int i = 0; i < GRID*GRID; i++)
			is >> v[i];
		
		cv.push_back(colorWithMap(color, v));
	}
	
		
}


colorMap::colorMap(string &img, bool intention, bool fill) : fileName(img)
{
	IplImage *src = cvLoadImage(img.c_str(), 1);
	
	if (src == NULL)
	{
		cout << img << " not found" << endl;
		return;
	}
	IplImage *hsv = toHSV(src);
	
	int rows = src->height;
	int cols = src->width;
	int step = src->widthStep;
	char *data = hsv->imageData;

	int gridWidth = ceil((double)cols / GRID);
	int gridHeight = ceil((double)rows / GRID);
	int gridSize = gridWidth*gridHeight;

	vector<vector<colorEntity> > colorFreq(GRID*GRID, vector<colorEntity>(TOTAL_COLORS));

	for (int i = 0; i < GRID*GRID; i++)
		for (int j = 0; j < TOTAL_COLORS; j++)
			colorFreq[i][j] = colorEntity(j);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			int h, s, v;
			h = data[i*step + 3 * j + 0];
			s = data[i*step + 3 * j + 1];
			v = data[i*step + 3 * j + 2];

			hsvColor hsv(h,s,v);
			
			if (intention || !hsvColor::isHSVDntCare(h, s, v))
				colorFreq[(i / gridHeight)*GRID + j / gridWidth][hsv.toInt()].freq++;
		}

	for (int i = 0; i < GRID*GRID; i++)
		sort(colorFreq[i].begin(), colorFreq[i].end());
	
	vector<vector<double> > cvTmp(TOTAL_COLORS, vector<double>(GRID*GRID, 0.0));
	vector<bool> cvTmpEmpty(TOTAL_COLORS, true);

	for (int i = 0; i < GRID*GRID; i++)
	{
		int cnt = 0;

		if (intention)
		{
			/*for (colorEntity& c : colorFreq[i])
				if (c.freq <= 0.1*gridSize)
					break;
				else
				{
					cvTmp[c.color][i] = 1;
					cvTmpEmpty[c.color] = false;
				}
			*/
			int last = 1;
			for (colorEntity& c : colorFreq[i])
				if (c.freq * 2 < last || c.freq < gridSize*0.05)
					break;
				else
				{
					cvTmp[c.color][i] = 1;
					cvTmpEmpty[c.color] = false;
					last = c.freq;
				}
			
		}
		else
		{
			
			for (colorEntity& c : colorFreq[i])
			{
				if (c.freq <= 0)
					break;
				cnt += c.freq;
			}
			
			for (colorEntity& c : colorFreq[i])
			{
				if ( c.freq <= 0 )
					break;
				cvTmp[c.color][i] = (double)c.freq / cnt;
				cvTmpEmpty[c.color] = false;
			}
		}
	}

	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		if (!cvTmpEmpty[i])
			cv.push_back(colorWithMap(i,cvTmp[i]));
	}

	//intention map
	if (intention)
	{
		double decay[] = {1,0.5};
		for (auto &cm : cv)
			cm.gd.propagate(INTENTION_PROPAGATE_DEPTH, decay);
	}
	else if (fill)
	{
		/*double decay[] = { 1, 0.5 };
		for (auto &cm : cv)
			cm.gd.propagate(INTENTION_PROPAGATE_DEPTH, decay);*/
		
		/*double penalty[] = {1,0,-0.5,-0.5};
		for (int s = 0, S = cv.size(); s < S; s++)
		{
			//cv[s].gd.closedFill();
			
			for (int i = 0; i < GRID; i++)
				for (int j = 0; j < GRID; j++)
					if (cv[s].gd.get(i, j) == 0)
					{
						int nearest = cv[s].gd.findNearest(i, j, 3);
						if (nearest <= 3)
							cv[s].gd.set(i, j, penalty[nearest]);
					}
		}
		/*
		grid<double> sum(GRID*GRID);
		
		for (auto &gr : cv)
		{
			sum = sum + gr.gd;
		}

		for (auto &gr : cv)
		{
			for (int i = 0; i < GRID*GRID; i++)
				if ( sum.get(i) > 0 )
					gr.gd.set(i, gr.gd.get(i) / sum.get(i));
		}*/
	}
	cvReleaseImage(&src);
}

void colorMap::show(string& title)
{
	IplImage *src = cvLoadImage(fileName.c_str(), 1);
	cvShowImage(title.c_str(), src);
	cvWaitKey(0);
	cvDestroyWindow(title.c_str());
	cvReleaseImage(&src);
}

void colorMap::printColors(ostream& out) const
{
	for (auto& i : cv)
	{
		out << "color " << hsvColor(i.color) << endl;

		out << i.gd << endl;
	}
}

void colorMap::save(ostream& file) const
{
	file << fileName << endl;
	for (auto& i : cv)
	{
		file << i.color << " ";
		i.gd.save(file);
	}
	file << endl;
}