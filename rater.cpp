#include "stdafx.h"
#include "rater.h"

#include <iostream>
#include <sstream>
using namespace std;

rater::rater(istream& in) : score(0)
{
	string line;
	while (getline(in,line))
	{
		/*istringstream is(line);
		
		set<string> list;
		string arg;
		while (is >> arg)
			list.insert(arg);*/
		
		set<string> list;
		list.insert(line);
		ansList.push(list);
	}
}

rater::~rater()
{
}

int rater::getScore() const
{
	return score;
}

void rater::grade(const vector<string>& result)
{
	if (ansList.front().size() == 1)
	{
		for (int i = 0; i < result.size(); i++)
			if (*ansList.front().begin() == result[i])
			{
				score++;
				cout << "find" << endl;
				break;
			}
		ansList.pop();
	}
	else
	{
		/*haven't implement yet*/
	}
}

set<string> rater::getAns() const
{
	return ansList.front();
}