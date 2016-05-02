#include "stdafx.h"
#include "grid.h"
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

template<class T>
grid<T>::grid(int s) : size(s)
{
	map = vector<T>(size,0);
	w = round(sqrt(size));
	h = size/w;
}

template<class T>
grid<T>::grid(int height, int width) : w(width), h(height)
{
	size = w*h;
	map = vector<T>(size, 0);
}

template<class T>
grid<T>::grid(const vector<T>& vec)
{
	map = vec;
	size = vec.size();
	width = sqrt(size);
	height = size/width;
}

template<class T>
void grid<T>::set(int idx, T val)
{
	map[idx] = val;
}

template<class T>
void grid<T>::set(int i, int j, T val)
{
	map[i*w+j] = val;
}

template<class T>
T grid<T>::get(int idx) const
{
	return map[idx];
}
template<class T>
T grid<T>::get(int i, int j) const
{
	return map[i*w+j];
}

template<class T>
T& grid<T>::operator[](int idx) const
{
	return map[idx];
}

template<class T>
T grid<T>::sum() const
{
	T res = 0;
	for (int i = 0; i < size; i++)
		res += map[i];
	return res;
}

template<class T>
int grid<T>::findNearest(int x, int y, T val, int limit) const
{
	int res = limit+1;
	for (int nx = x-limit; nx <= x+limit; nx++)
	{
		int rem = limit - abs(nx-limit);
		for (int ny = y - rem; ny <= y + rem; ny++)
			if (legal(nx, ny) && get(nx,ny) == val)
				res = min(res, abs(nx - x) + abs(ny - y));
	}
	return res;
}

template<class T>
bool grid<T>::legal(int i, int j) const
{
	return i < h && j < w && i >= 0 && j >= 0;
}

template<class T>
bool grid<T>::border(int i, int j) const
{
	return i == 0 || i == h-1 || j == 0 || j == w-1;
}

template<class T>
grid<T> grid<T>::operator+(const grid<T>& rhs) const
{
	grid<T> res(map.size());

	for (int i = 0; i < map.size(); i++)
		res[i] = map[i] + rhs[i];

	return res;
}

template<class T>
grid<T> grid<T>::operator*(int fac) const
{
	grid<T> res(map.size());

	for (int i = 0; i < map.size(); i++)
		res[i] = map[i]*fac;

	return res;
}

template<class T>
grid<T> grid<T>::operator*(double fac) const
{
	grid<T> res(map.size());

	for (int i = 0; i < map.size(); i++)
		res[i] = map[i] * fac;

	return res;
}

template<class T>
T grid<T>::operator*(const grid<T>& rhs) const
{
	T res = 0;

	for (int i = 0; i < size; i++)
		res += map[i] * rhs[i];

	return res;
}

template<class T>
void grid<T>::max(const grid<T>& rhs)
{
	for (int i = 0; i < size; i++)
		map[i] = max(map[i], rhs[i]);
}

template<class T>
void grid<T>::propagate(int depth, T decay[])
{
	for (int i = 0; i < size; i++)
	{
		if (map[i] == 0)
			continue;
		int cx = i/w;
		int cy = i%w;

		for (int x = cx - depth; x <= cx + depth; x++)
		{
			int rem = depth - abs(x-cx);
			for (int y = cy - rem; y <= cy + rem; y++)
			{
				if (legal(x,y))
					set(x, y, max(get(x,y),decay[abs(x-cx)+abs(y-cy)]*map[i]));
			}
		}
	}
}

template<class T>
void grid<T>::closedFill()
{
	const static int neiborX[] = {1,-1,0,0};
	const static int neiborY[] = {0,0,1,-1};

	vector<vector<bool>> visited(h,vector<bool>(w,false));
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
		{
			if (visited[i][j])
				continue;

			bool needFill = true;
			queue<int> Q;
			queue<int> fill;

			Q.push(i*w+j);
			while (!Q.empty())
			{
				int top = Q.front();
				Q.pop();

				visited[top/w][top%w] = true;

				for (int k = 0; k < 4; k++)
				{
					int nx = i+neiborX, ny = j+neiborY;
					if (legal(nx,ny) && !visited[nx][ny] && get(nx,ny) == get(i,j))
					{
						Q.push(nx*w+ny);
						if (needFill &= !border(nx,ny))
							fill.push(nx*w+y);
					}
				}
			}

			if (needFill)
			{
				while (!fiil.empty())
				{
					map[fill.top()] = get(i,j);
					fill.pop();
				}
			}
		}
}

template<class T>
ostream& operator<< (ostream &out,const grid<T>& rhs)
{
	for (int i = 0; i < rhs.h; i++)
	{
		for (int j = 0; j < rhs.w; j++)
			out << setprecision(3) << rhs.get(i,j) << ",";
		out << endl;
	}
	out << endl;
	return out;
}

