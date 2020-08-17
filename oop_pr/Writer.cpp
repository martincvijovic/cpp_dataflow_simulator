#include "Writer.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

Writer* Writer::Instance;
vector<int> Writer::idtowrite;
vector<int> Writer::start;
vector<int> Writer::end;

string Writer::filename;
ofstream Writer::out;

Writer * Writer::getInstance()
{
	if (Instance == nullptr)
	{
		Instance = new Writer();
	}
	return Instance;
}

void Writer::go()
{
	ofstream out1;
	out1.open(filename);
	cout << "WRITING TO: ";
	cout << filename << endl;
	for (int i = 0; i < idtowrite.size(); i++)
	{
		cout << "WRITING..." << endl;
		if(i!=idtowrite.size()-2)
		out1 << "(" << idtowrite[i] << ")" << "\t" << start[i] << "ns/" << end[i] << "ns" << endl;
	}
	out1.close();
	cout << "SUCCESS" << endl;
}
