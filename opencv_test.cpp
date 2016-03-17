// opencv_test.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "colorMap.h"
#include "labColor.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	labColor::init();
	/*
	int maxL = -10000,maxA = -10000,maxB = -10000;
	int minL =  10000,minA =  10000,minB =  10000;

	for (int r = 0; r <= 255; r++)
		for (int g = 0; g <= 255; g++)
			for (int b = 0; b <= 255; b++)
			{
				labColor lab(r,g,b,0);
				
				maxL = max(lab.l,maxL);
				maxA = max(lab.a, maxA);
				maxB = max(lab.b, maxB);

				minL = min(lab.l, minL);
				minA = min(lab.a, minA);
				minB = min(lab.b, minB);
			}

	cout << minL << " " << maxL << endl;
	cout << minA << " " << maxA << endl;
	cout << minB << " " << maxB << endl;

	system("PAUSE");
	return 0;
	*/
	vector<colorMap> db;
	vector<colorMap> target;
	
	for (int i = 1; i <= 100; i++)
	{
		db.push_back(colorMap(string("image\\") + to_string(i) + string(".jpg"), true));
		cout << i << " loaded" << endl;
	}

	for (int i = 1; i <= 30; i++)
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

			logFile << "map size" << endl;


			for (int i = 0; i < db.size(); i++)
			{
				string tmp = db[i].fileName;
				replace(tmp.begin(), tmp.end(), '\\', '_');
				dbFiles.push_back(fstream(string("log\\") + tmp + string(".csv"), ios::out));

				logFile << i << "," << db[i].cv.size() << endl;
				db[i].printColors(dbFiles[i]);
				dbFiles[i] << endl << endl << endl;
				
				cout << "db print colors	" << i << "/" << db.size() << endl;
			}

			for (int i = 0; i < target.size(); i++)
			{
				string tmp = target[i].fileName;
				replace(tmp.begin(), tmp.end(), '\\', '_');
				tarFiles.push_back(fstream(string("log\\") + tmp + string(".csv"), ios::out));

				target[i].printColors(tarFiles[i]);
				tarFiles[i] << endl << endl << endl;

				cout << "target print colors	" << i << "/" << target.size() << endl;
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

					target[i].debugShow(db[j], tarFiles[i]);
					db[j].debugShow(target[i], dbFiles[j]);

					tarFiles[i] << endl;
					dbFiles[j] << endl;

					result.push_back(pair<double, int>(db[j] - target[i], j));

					cout << "diff	target" << i << " - " << "db" << j << endl;
				}

				sort(result.begin(), result.end());

				for (auto &p : result)
				{
					tarFiles[i] << "," << db[p.second].fileName << "," << p.first << endl;
					logFile << "," << db[p.second].fileName << "," << p.first << endl;
				}
					

				tarFiles[i] << endl;
				logFile << endl;

			}

			logFile.close();

			for (auto& file : dbFiles)
				file.close();
				
			cout << "log done" << endl;
		}
		/*else if (cmd.compare("map") == 0)
		{
			if (id > 0)
				db[id - 1].printColors();
			else if ( id < 0 )
				target[-id-1].printColors();
		}
		else if (cmd.compare("diff") == 0)
		{
			int id2;
			cin >> id2;
			db[id - 1].debugShow(target[-id2-1]);
		}
		else if (cmd.compare("result") == 0)
		{
			vector<pair<double, int> > result;

			for (int i = 0; i < db.size(); i++)
			{
				result.push_back(pair<double, int>(db[i] - target[-id - 1], i));
				cout << "done " << i << " - tar[" << -id << "]" << endl;
			}
				

			sort(result.begin(), result.end());

			for (auto &p : result)
				cout << p.second + 1 << " : " << p.first << endl;

			db[result.back().second].show(string("result"));
			
		}
		else if (cmd.compare("show") == 0)
		{
			if (id > 0)
				db[id - 1].show(string("show"));
			else if ( id < 0 )
				target[-id-1].show(string("show"));
		}
		else if (cmd.compare("clear") == 0)
		{
			for (int i = 0; i < 40; i++)
				puts("");
		}
		else if (cmd.compare("exit") == 0)
			break;*/
	}

	return 0;
}

