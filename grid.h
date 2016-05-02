#ifndef __GRID_H__
#define __GRID_H__

#include <queue>
#include <cmath>
#include <vector>
using namespace std;

template<class T>
class grid
{
private:
	int w,h,size;
	vector<T> map;
public:
	grid(int);		//given size(default square)
	grid(int,int);	//given width & height
	grid(const vector<T>&);	//given vector

	int width() const;
	int height() const;

	void set(int,T);
	void set(int,int,T);
	T get(int) const;
	T get(int,int) const;
	T& operator[](int);
	T sum() const;

	int findNearest(int,int,int) const;

	bool operator<(const grid&) const;
	bool operator==(const grid&) const;
	bool legal(int,int) const;
	bool border(int,int) const;

	grid<T> mul(const grid<T>&) const;
	grid<T> operator+(const grid<T>&) const;
	grid<T> operator*(int) const;
	grid<T> operator*(double) const;
	T operator*(const grid<T>&) const;
	void max(const grid<T>&);

	void propagate(int, T[]);
	void closedFill();

	void save(ostream&) const;
};

template<class T>
ostream& operator<<(ostream &os, const grid<T> &rhs);


template<class T>
grid<T>::grid(int s) : size(s)
{
	map = vector<T>(size, 0);
	w = round(sqrt(size));
	h = size / w;
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
	w = sqrt(size);
	h = size / w;
}

template<class T>
int grid<T>::width() const
{
	return w;
}

template<class T>
int grid<T>::height() const
{
	return h;
}

template<class T>
void grid<T>::set(int idx, T val)
{
	map[idx] = val;
}

template<class T>
void grid<T>::set(int i, int j, T val)
{
	map[i*w + j] = val;
}

template<class T>
T grid<T>::get(int idx) const
{
	return map[idx];
}
template<class T>
T grid<T>::get(int i, int j) const
{
	return map[i*w + j];
}

template<class T>
T& grid<T>::operator[](int idx)
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
int grid<T>::findNearest(int x, int y, int limit) const
{
	int res = limit + 1;
	for (int nx = x - limit; nx <= x + limit; nx++)
	{
		int rem = limit - abs(nx - x);
		for (int ny = y - rem; ny <= y + rem; ny++)
		if (legal(nx, ny) && get(nx, ny) == 1)
			res = min(res, abs(nx - x) + abs(ny - y));
	}
	return res;
}

template<class T>
bool grid<T>::operator<(const grid& rhs) const
{
	if (w != rhs.w || h != rhs.h)
		return false;

	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			if (get(i, j) != rhs.get(i, j))
				return get(i, j) < rhs.get(i, j);

	return false;
}

template<class T>
bool grid<T>::operator==(const grid& rhs) const
{
	if ( w != rhs.w || h != rhs.h )
		return false;

	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			if( get(i,j) != rhs.get(i,j) )
				return false;
	
	return true;
}

template<class T>
bool grid<T>::legal(int i, int j) const
{
	return i < h && j < w && i >= 0 && j >= 0;
}

template<class T>
bool grid<T>::border(int i, int j) const
{
	return i == 0 || i == h - 1 || j == 0 || j == w - 1;
}

template<class T>
grid<T> grid<T>::mul(const grid<T>& rhs) const
{
	grid<T> res(map.size());

	for (int i = 0; i < map.size(); i++)
		res[i] = map[i] * rhs.get(i);

	return res;
}

template<class T>
grid<T> grid<T>::operator+(const grid<T>& rhs) const
{
	grid<T> res(map.size());

	for (int i = 0; i < map.size(); i++)
		res[i] = map[i] + rhs.get(i);

	return res;
}

template<class T>
grid<T> grid<T>::operator*(int fac) const
{
	grid<T> res(map.size());

	for (int i = 0; i < map.size(); i++)
		res[i] = map[i] * fac;

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
		res += map[i] * rhs.get(i);

	return res;
}

template<class T>
void grid<T>::max(const grid<T>& rhs)
{
	for (int i = 0; i < size; i++)
	{
		if (map[i] <= 0 && rhs.get(i) <= 0)
			map[i] = map[i]<rhs.get(i)? map[i] : rhs.get(i);
		else 
			map[i] = map[i]>rhs.get(i)? map[i] : rhs.get(i);
	}
}

template<class T>
void grid<T>::propagate(int depth, T decay[])
{
	grid<T> tmp = *this;

	for (int i = 0; i < size; i++)
	{
		int cx = i / w;
		int cy = i%w;

		for (int x = cx - depth; x <= cx + depth; x++)
		{
			int rem = depth - abs(x - cx);
			for (int y = cy - rem; y <= cy + rem; y++)
			{
				if (legal(x, y) && (tmp.get(x, y) < decay[abs(x - cx) + abs(y - cy)] * map[i]))
					tmp.set(x, y, decay[abs(x - cx) + abs(y - cy)] * map[i]);
			}
		}
	}

	for (int i = 0; i < size; i++)
		set(i, tmp[i]);
}

template<class T>
void grid<T>::closedFill()
{
	const static int neiborX[] = { 1, -1, 0, 0 };
	const static int neiborY[] = { 0, 0, 1, -1 };

	vector<vector<bool>> visited(h, vector<bool>(w, false));
	for (int i = 0; i < h; i++)
	for (int j = 0; j < w; j++)
	{
		if (visited[i][j] || get(i,j) != 0)
			continue;

		bool needFill = !border(i,j);
		T neiMin = 1;
		queue<int> Q;
		queue<int> fill;
		fill.push(i*w+j);

		Q.push(i*w + j);
		while (!Q.empty())
		{
			int top = Q.front();
			Q.pop();

			visited[top/w][top%w] = true;

			for (int k = 0; k < 4; k++)
			{
				int nx = top/w + neiborX[k], ny = top%w + neiborY[k];
				if (legal(nx, ny))
				{
					if (get(nx,ny) != 0)
						neiMin = min(neiMin,get(nx,ny));
					else if (!visited[nx][ny])
					{
						Q.push(nx*w + ny);
						if (needFill &= !border(nx, ny))
							fill.push(nx*w + ny);
					}
					
				}
			}
		}
		needFill &= neiMin >= 0.5;
		if (needFill)
		{
			while (!fill.empty())
			{
				map[fill.front()] = 1;
				fill.pop();
			}
		}
	}
}

template<class T>
void grid<T>::save (ostream &file) const
{
	for (int i = 0; i < size; i++)
		file << setprecision(3) << get(i) << " ";
}

template<class T>
ostream& operator<< (ostream &out, const grid<T>& rhs)
{
	for (int i = 0; i < rhs.height(); i++)
	{
		for (int j = 0; j < rhs.width(); j++)
			out << setprecision(3) << rhs.get(i, j) << ",";
		out << endl;
	}
	out << endl;
	return out;
}



#endif