#ifndef __COLOR_H__
#define __COLOR_H__

class color
{
public:
	
	static bool isDntCare(int,int,int);	//rgb

	virtual double operator-(const color&) const;
	virtual int toInt() const;
	virtual void print() const;
};

bool color::isDntCare(int _r, int _g, int _b)
{
	return ((_r + 256) % 256 == 64) && ((_g + 256) % 256 == 52) && ((_b + 256) % 256 == 48);
}

#endif