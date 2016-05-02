#pragma once

#include <queue>
#include <string>
#include <set>

using namespace std;

class rater
{
private:
	int score;
	queue<set<string>> ansList;
public:
	rater(istream&);
	~rater();

	int getScore() const;
	void grade(const vector<string>&);
	set<string> getAns() const;
};

