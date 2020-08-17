#ifndef _MACHINE_H_
#define _MACHINE_H_

#include "Operation.h"
#include "MemorySingleton.h"
#include <iostream>


using namespace std;


class Machine //reads IMF file, singleton DP?
{
public:
	static Machine * getInstance();
	bool operator=(Machine&) = delete;
	static void exec(string name);
	friend class Memory;
	friend class Sum;
	friend class Mul;
	friend class Exp;
	friend class Write;
private:
	Machine() {};
	Machine(Machine&) = delete;
	static Machine* Instance;
	static list<ArithmeticOperation*> queued;
	static list<ArithmeticOperation*> ongoing;
	static list<ArithmeticOperation*> finished; //do i even need this?
	
};

#endif
