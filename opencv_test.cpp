// opencv_test.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "colorMap.h"
#include "hsvColor.h"
#include "grader.h"
#include "database.h"
#include "tester.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	tester test(fstream("database\\db_save_prop", ios::in), fstream("database\\selectedList", ios::in));
	cout << "loaded" << endl;

	for (int i = 1; i <= 100; i++)
	{
		cout << i << " : " << endl;
		test.test(colorMap("target-2016-04-26\\"+to_string(i)+".jpg.png"));
	}

	cout << test.getScore() << endl;
	
	/*
	database db;
	
	db.insertDir("database");
	db.save(fstream("database\\db_save_prop",ios::out));*/
	system("PAUSE");
	
	/*hsvColor::init();
	vector<colorMap> db;
	vector<colorMap> target;
	
	for (int i = 1; i <= 100; i++)
	{
		db.push_back(colorMap(string("image\\") + to_string(i) + string(".jpg"), true));
		cout << i << " loaded" << endl;
		
		for (int j = 0; j < 64; j++)
		{
			if (j % 8 == 0)
				cout << endl;

			double sum = 0;
			for (auto c : db.back().cv)
				sum += c.map[j];

			printf("%.3f ",sum);
			
		}
		
	}
	

	for (int i = 40; i <= 43; i++)
	{
		target.push_back(colorMap(string("target\\") + to_string(i) + string(".png"), false));
		cout << "target" << i << "loaded" << endl;
	}

	cout << "load complete" << endl;

	while (true)
	{
		int id;
		string cmd;
		cin >> cmd;

		if (cmd.compare("log") == 0)
		{
			//log each color map
			vector<fstream> dbFiles;
			vector<fstream> tarFiles;
			fstream logFile("log\\log.csv", ios::out);
			grader grader("target\\ans.txt");
			logFile << "map size" << endl;


			for (int i = 0; i < db.size(); i++)
			{
				string tmp = db[i].fileName;
				replace(tmp.begin(), tmp.end(), '\\', '_');
				dbFiles.push_back(fstream(string("log\\") + tmp + string(".csv"), ios::out));

				logFile << i << "," << db[i].cv.size() << endl;
				db[i].printColors(dbFiles[i]);
				dbFiles[i] << endl << endl << endl;
				
				cout << "db print colors	" << i+1 << "/" << db.size() << endl;
			}

			for (int i = 0; i < target.size(); i++)
			{
				string tmp = target[i].fileName;
				replace(tmp.begin(), tmp.end(), '\\', '_');
				tarFiles.push_back(fstream(string("log\\") + tmp + string(".csv"), ios::out));

				target[i].printColors(tarFiles[i]);
				tarFiles[i] << endl << endl << endl;

				cout << "target print colors	" << i+1 << "/" << target.size() << endl;
			}

			logFile << endl << endl << endl;
			for (int i = 0; i < target.size(); i++)
			{
				string iName = target[i].fileName;
				replace(iName.begin(), iName.end(), '\\', '_');

				vector<pair<double, int> > result;

				logFile << "result " << iName << endl;

				for (int j = 0; j < db.size(); j++)
				{
					string jName = db[j].fileName;
					replace(jName.begin(), jName.end(), '\\', '_');

					dbFiles[j]  << "diff" << "," << iName << endl;
					tarFiles[i] << "diff" << "," << jName << endl;

					double tem = target[i].debugShow(db[j], tarFiles[i]);
					//db[j].debugShow(target[i], dbFiles[j]);

					tarFiles[i] << endl;
					dbFiles[j] << endl;

					result.push_back(pair<double, int>(tem, j));

					cout << "diff	target" << i << " - " << "db" << j << endl;
				}

				sort(result.rbegin(), result.rend());
				grader.update(result,i);

				for (auto &p : result)
				{
					tarFiles[i] << "," << db[p.second].fileName << "," << p.first << endl;
					logFile << "," << db[p.second].fileName << "," << p.first << endl;
				}
					

				tarFiles[i] << endl;
				logFile << endl;

			}

			logFile << grader << endl;
			logFile.close();

			for (auto& file : dbFiles)
				file.close();
				
			cout << "log done" << endl;
		}
	}*/

	return 0;
}

