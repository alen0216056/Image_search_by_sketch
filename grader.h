#ifndef __GRADER_H__
#define __GRADER_H__

#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

class grader
{
private:
	vector<set<int> > vec;

	

public:
	vector<int> first;
	vector<int> top5;
	vector<int> top10;
	vector<int> fail;

	grader(string);

	void update(const vector<pair<double,int> > &,int);
};

ostream& operator<<(ostream&,const grader&);


#endif