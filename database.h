#pragma once

#include "colorMap.h"

#include <string>
#include <set>

using namespace std;

class database
{
private:
	set<colorMap> db;
public:
	database();
	database(istream&);
	~database();
	void insertDir(const string&);
	void insert(string&);
	void insert(const colorMap&);

	vector<string> select(int n) const;

	void save(ostream&) const;

	void show() const;

	set<colorMap> getContents() const;

};

