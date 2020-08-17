#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "Event.h"
#include "Sched.h"
#include "Types.h"
#include <cmath>
#include <iostream>
#include <list>
#include <string>

using namespace std;

// OPERATION ID = 0 => INPUT PORT UPDATED, OPERATION ID = 1 => OPERATION IS EXECUTED


class Operation : public ITimedElement
{
public:
	Operation() {};
	
	//void setT(Time t) { this->T = t; };
	//
	virtual void notify(ID id) = 0; //inpurt port update or execution finished
protected:
	static int opcount;
	static Time T;
	int inputPorts;
	list<string> op;
	double result;
};

class ArithmeticOperation : public Operation // max 2 input ports, only 1 output port
{
public:
	ArithmeticOperation() { opcount = 2; goRight = false; next = nullptr; };
	void addOperand(string temp) { this->op.push_back(temp); this->inputPorts++; };
	void addLeftOperand(string temp);
	void addRightOperand(string temp);
	virtual void notify(ID id) = 0;	 //dont forget to use the next and notify it!
	void setReceiver(ArithmeticOperation* caller) { next = caller; };
	friend void swapOperands(ArithmeticOperation* current);
	ID getId() { return id1; };
	void setId(ID newID) { id1 = newID; };
	void isRightOperand() { goRight = true; }
	void setUID(char c) { UID = c; }
	char getUID() { return UID; }

	string getLeftOperand() { return op.front(); };
	string getRightOperand() { return op.back(); };

	static void setT(Time t) { T = t; };
	static Time getT();


	//just testing
	ArithmeticOperation* getNext() { return next; };
	int getUsedPorts() { return inputPorts; };
protected:
	//bool isReversed;

	bool goRight; // need this in order to know where is the output supposed to go, as left or as right operand of the next operation
	ArithmeticOperation* next;
	ID id1; // not really needed but fine
	char UID;

private:
	static Time T;
};

class Sum : public ArithmeticOperation
{
public:
	Sum() { opcount = 2; inputPorts = 0; result = 0; };
	void notify(ID id);
	static void setT(Time t);
	static Time getT();
private:
	static Time T;
};

class Mul : public ArithmeticOperation
{
public:
	Mul() { opcount = 2; inputPorts = 0; result = 1; };
	void notify(ID id);
	static void setT(Time t);
	static Time getT();
	
private:
	static Time T;
};

class Exp : public ArithmeticOperation
{
public:
	Exp() { opcount = 2; inputPorts = 0; result = 0; };
	void notify(ID id);
	static void setT(Time t);
	static Time getT();
private:
	static Time T;

};

class Write : public ArithmeticOperation
{
public:
	Write() { T = 0; opcount = 2; inputPorts = 0; result = 0;};
	void notify(ID id);
	virtual void setT(Time t);
	virtual Time getT();
private:
	static Time T;
	// delay 0 -> Tw defined in memory
};



#endif