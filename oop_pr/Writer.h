#ifndef _WRITER_H_
#define _WRITER_H_

#include "Types.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;


class Writer
{
public:
	static Writer* getInstance();
	static void addId(int ID) { idtowrite.push_back(ID); };
	static void addStart(int st) { start.push_back(st); };
	static void addEnd(int en) { end.push_back(en); };
	static void setFileName(string name) { filename = name; };
	static void go();
private:
	Writer() {};
	static ofstream out;

	static vector<int> idtowrite;
	static vector<int> start;
	static vector<int> end;

	static string filename; //with .log extension
	static Writer* Instance;
};



#endif