#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Event.h"
#include "Sched.h"
#include "Types.h"
#include "Operation.h"
#include <cmath>
#include <iostream>
#include <list>
#include <vector>
#include <string>

using namespace std;

class Memory : public ITimedElement //SINGLETON DP
{
public:
	bool operator=(Memory&) = delete;
	static Memory* getInstance();
	static void set(string varName, double val);
	static double get(string varName); //Variable not found Exception
	static void addToReadingQueue(ArithmeticOperation* reader, string variable) { WhoWantsToRead.push_back(reader); NamesToRead.push_back(variable); };
	static void setNw(int nw);
	static int getNw();
	static void setTw(int nw);
	static int getTw();
	static void flushLeft();
	static void flushRight();
	void notify(ID id); //obavestava da je ubacena operacija, tad proveravaj namestowrite i namestoread
	//maybe needs a to-write getter?
	//TESTING PURPOSES
	static vector<string> getNamesToRead() { return NamesToRead; };
	static vector<ArithmeticOperation*> getWhowants() { return WhoWantsToRead; };
private:
	Memory() {}
	Memory(Memory&) = delete;
	static Memory* Instance;
	static vector<ArithmeticOperation*> WhoWantsToRead;
	static vector<string> NamesToRead;
	static vector<int> left;
	static vector<int> right;
	static vector<string> NamesToWrite; // push anytime, process when possible (nw > 0)
	static vector<double> ValuesToWrite; // first in first out!

	static vector<string> names; //actual memory
	static vector<double> values;

	static int Nw;
	static int Tw;
};

#endif