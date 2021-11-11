#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "Types.h"
#include <iostream>
#include <string>

using namespace std;

 
class Compiler //singleton DP
{
public:
	static Compiler * getInstance();
	bool operator=(Compiler&) = delete;
	static void Compile(ifstream& program, string name); // postorder traversal
private:
	Compiler() {};
	Compiler(Compiler&) = delete;
	static Compiler* Instance;
};


#endif