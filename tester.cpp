#include "stdafx.h"
#include "tester.h"
#include "hsvColor.h"

#include <iostream>
#include <fstream>

using namespace std;

tester::tester(istream& db_in, istream& rater_in) : db(db_in), rt(rater_in)
{
	hsvColor::init();
}

tester::~tester()
{
}

void tester::test(const colorMap& target)
{
	vector<pair<double,string>> scores;

	for (auto &dbImage : db.getContents())
		scores.push_back({ target - dbImage, dbImage.fileName });
		
	sort(scores.rbegin(), scores.rend());

	string targetAns = *rt.getAns().begin();
	for (int i = 0; i < scores.size(); i++)
	{
		if (scores[i].second.compare(targetAns) == 0)
			cout << scores[i].first << "  " << i+1 << endl;
	}

	vector<string> top20;

	for (int i = 0; i < 20; i++)
		top20.push_back(scores[i].second);

	rt.grade(top20);
}

int tester::getScore() const
{
	return rt.getScore();
}