#ifndef _CUSTOMEXCEPTIONS_H_
#define _CUSTOMEXCEPTIONS_H_

#include <exception>
#include <iostream>

using namespace std; 

class VariableNotFoundException : public exception
{
public:
	VariableNotFoundException(const char* msg) : exception(msg) {};
};


#endif
