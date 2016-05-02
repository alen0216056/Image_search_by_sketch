#include "stdafx.h"
#include "database.h"

#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

database::database()
{
}

database::database(istream& file)
{
	while (file.good())
	{
		db.insert(colorMap(file));
	}
}

database::~database()
{
}

void database::insertDir(const string& dir)
{
	const static set<string> extList = {"jpg","png","gif","JPG","GIF","PNG"};
	WIN32_FIND_DATA fdata;
	HANDLE handle = INVALID_HANDLE_VALUE;
	LARGE_INTEGER filesize;
	TCHAR tDir[64];
	
	string searchDir = dir + "\\*";
		
	copy(searchDir.begin(), searchDir.end(), tDir);
	tDir[searchDir.size()] = 0;

	handle = FindFirstFile(tDir,&fdata);

	if (handle == INVALID_HANDLE_VALUE)
	{
		cerr << "couldn't find first file" << endl;
		return;
	}

	do
	{
		wstring ws(fdata.cFileName);
		string str(ws.begin(), ws.end());
		
		int dotPos = str.find_first_of(".");
		if ( dotPos != string::npos)
		{
			string ext = str.substr(dotPos+1,str.size()-dotPos);
			if (extList.find(ext) != extList.end())
			{
				//cout << str << endl;

				colorMap cm(dir + "\\" + str);

				auto iter = db.find(cm);

				if (iter != db.end())
					cout << str << " is identical to " << iter->fileName << endl;

				insert(cm);
			}
		}

		if (db.size()%10 == 0)
			cout << "done " << db.size() << endl;
	} while (FindNextFile(handle, &fdata) && db.size() < 5000);

	FindClose(handle);
	
	return;
}

void database::insert(string& filename)
{
	db.insert(colorMap(filename));
}

void database::insert(const colorMap& cm)
{
	db.insert(cm);
}

vector<string> database::select(int n) const
{
	vector<string> res;
	int cnt = 0;
	for (auto iter = db.begin(); iter != db.end(); iter++)
	{
		double prob = (double)(n - res.size()) / (db.size()-cnt);
		cnt++;
		if ((rand() % 100) / 100.0 < prob)
		{
			res.push_back(iter->fileName);
		}
	}
	return res;
}

void database::save(ostream& file) const
{
	for (auto &cm : db)
	{
		cm.save(file);
	}
}

void database::show() const
{
	for (auto& cm: db)
		cm.printColors();
}

set<colorMap> database::getContents() const
{
	return db;
}