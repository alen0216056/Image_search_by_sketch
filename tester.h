#pragma once

#include "database.h"
#include "rater.h"

class tester
{
private:
	database db;
	rater rt;
public:
	tester(istream&,istream&);
	~tester();
	
	void test(const colorMap&);
	int getScore() const;
};

