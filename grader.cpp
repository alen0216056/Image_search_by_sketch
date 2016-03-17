#include "stdafx.h"
#include "grader.h"

#include <set>
#include <fstream>
#include <sstream>
using namespace std;

grader::grader(string fname)
{
	fstream file(fname);

	string line;
	while (getline(file,line))
	{
		vec.push_back(set<int>());
		istringstream is(line);

		int can;
		while (is >> can)
			vec.back().insert(can);
	}
}

void grader::update(const vector<pair<double, int> > &result, int tar)
{
	for (int i = 0; i < 10 && i < result.size(); i++)
	{
		if (vec[tar].find(result[i].second+1) != vec[tar].end())
		{
			if ( i == 0 )
				first.push_back(tar+1);
			else if ( i < 5 )
				top5.push_back(tar+1);
			else
				top10.push_back(tar+1);

			return;
		}
	}
	fail.push_back(tar + 1);
}

ostream& operator<<(ostream& out, const grader& rhs)
{
	out << "first : ,";
	for (auto &i : rhs.first)
		out << i << ",";
	out << endl;

	out << "top 5 : ,";
	for (auto &i : rhs.top5)
		out << i << ",";
	out << endl;

	out << "top10 : ,";
	for (auto &i : rhs.top10)
		out << i << ",";
	out << endl;

	out << "fail : ,";
	for (auto &i : rhs.fail)
		out << i << ",";
	out << endl;

	return out;
}