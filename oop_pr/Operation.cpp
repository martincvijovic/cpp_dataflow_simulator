#include "Operation.h"
#include "Machine.h"
#include "MemorySingleton.h"
#include "Writer.h"
#include <ctype.h>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

int Operation::opcount;
Time Sum::T;
Time Mul::T;
Time Exp::T;
Time Write::T;
Time ArithmeticOperation::T;



void swapOperands(ArithmeticOperation* current) //will take care of the order (sadly, exp is not commutative)
{
	string temp = current->op.front();
	string temp1 = current->op.back();
	current->op.front() = temp1;
	current->op.back() = temp;
}


void Sum::notify(ID id) 
{
	if (id == 1) //operation finished
	{
		Writer::getInstance()->addId(this->id1);
		Writer::getInstance()->addEnd(Scheduler::Instance()->getCurTime());
		Writer::getInstance()->addStart(Scheduler::Instance()->getCurTime() - Sum::getT());
		Machine::finished.push_back(this);
		list<ArithmeticOperation*>::iterator i = Machine::ongoing.begin();
		while (i != Machine::ongoing.end())
		{
			if ((*i) == this)
			{
				i = Machine::ongoing.erase(i);
				//cout << "IZBRISAN IZ ONGOING ID " << this->getId() << endl;
			}
			else i++;
		}
		cout << "IZVRSENO " << this->id1 << ": " << this->getLeftOperand() << " + " << this->getRightOperand() << " SADA JE " << Scheduler::Instance()->getCurTime() << endl;
		if (next != nullptr)
		{
			if (goRight)
			{
				next->addRightOperand(to_string(this->result));
			}
			else
			{
				next->addLeftOperand(to_string(this->result));
			}
			next->notify(0);
		}
	}
	else // if full calculate result!
	{
		vector<ArithmeticOperation*> target = Memory::getInstance()->getWhowants();
		string tmp;
		for (unsigned int i = 0; i < target.size(); i++)
		{
			if (target[i] == this)
			{
				tmp = Memory::getInstance()->getNamesToRead()[i];
				double x = Memory::getInstance()->get(tmp);
				tmp = to_string(x);
			}
		}

		if (id == 2) //goleft
		{
			
			this->addLeftOperand(tmp);
		}
		if (id == 3)
		{
			this->addRightOperand(tmp);
		}

		if (inputPorts == 2)
		{
			//Event::create(this, T, 1); //sounds really bad but ok
			for (list<string>::iterator i = op.begin(); i != op.end(); i++)
			{
				this->result += stod((*i)); //string to integer
			}
		}
	}
}

void Sum::setT(Time t)
{
	T = t;
}

Time Sum::getT()
{
	return T;
}


void Mul::notify(ID id)
{
	if (id == 1) //operation finished
	{
		Writer::getInstance()->addId(this->id1);
		Writer::getInstance()->addEnd(Scheduler::Instance()->getCurTime());
		Writer::getInstance()->addStart(Scheduler::Instance()->getCurTime() - Mul::getT());
		Machine::finished.push_back(this);
		list<ArithmeticOperation*>::iterator i = Machine::ongoing.begin();
		while (i != Machine::ongoing.end())
		{
			if ((*i) == this)
			{
				i = Machine::ongoing.erase(i);
				//cout << "IZBRISAN IZ ONGOING ID " << this->getId() << endl;
			}
			else i++;
		}
		cout << "IZVRSENO " << this->id1 << ": " << this->getLeftOperand() << " * " << this->getRightOperand() << " SADA JE " << Scheduler::Instance()->getCurTime() << endl;
		if (next != nullptr)
		{
			if (goRight)
			{
				next->addRightOperand(to_string(this->result));
			}
			else
			{
				next->addLeftOperand(to_string(this->result));
			}
			next->notify(0);
		}
	}
	else // if full calculate result!
	{
		vector<ArithmeticOperation*> target = Memory::getInstance()->getWhowants();
		string tmp;
		for (unsigned int i = 0; i < target.size(); i++)
		{
			if (target[i] == this)
			{
				tmp = Memory::getInstance()->getNamesToRead()[i];
				double x = Memory::getInstance()->get(tmp);
				tmp = to_string(x);
			}
		}

		if (id == 2) //goleft
		{

			this->addLeftOperand(tmp);
		}
		if (id == 3)
		{
			this->addRightOperand(tmp);
		}

		if (inputPorts == 2)
		{
			//Event::create(this, T, 1); //sounds really bad but ok
			for (list<string>::iterator i = op.begin(); i != op.end(); i++)
			{
				this->result *= stod((*i)); //string to double
			}
		}
	}
}

void Mul::setT(Time t)
{
	T = t;
}

Time Mul::getT()
{
	return T;
}


void Exp::notify(ID id)
{
	if (id == 1) //operation finished
	{
		Writer::getInstance()->addId(this->id1);
		Writer::getInstance()->addEnd(Scheduler::Instance()->getCurTime());
		Writer::getInstance()->addStart(Scheduler::Instance()->getCurTime() - Exp::getT());
		Machine::finished.push_back(this);
		list<ArithmeticOperation*>::iterator i = Machine::ongoing.begin();
		while (i != Machine::ongoing.end())
		{
			if ((*i) == this)
			{
				i = Machine::ongoing.erase(i);
				//cout << "IZBRISAN IZ ONGOING ID " << this->getId() << endl;
			}
			else i++;
		}
		cout << "IZVRSENO " << this->id1 << ": " << this->getLeftOperand() << " * " << this->getRightOperand() << " SADA JE " << Scheduler::Instance()->getCurTime() << endl;
		if (next != nullptr)
		{
			if (goRight)
			{
				next->addRightOperand(to_string(this->result));
			}
			else
			{
				next->addLeftOperand(to_string(this->result));
			}
			next->notify(0);
		}
	}
	else // if full calculate result!
	{
		vector<ArithmeticOperation*> target = Memory::getInstance()->getWhowants();
		string tmp;
		for (unsigned int i = 0; i < target.size(); i++)
		{
			if (target[i] == this)
			{
				tmp = Memory::getInstance()->getNamesToRead()[i];
				double x = Memory::getInstance()->get(tmp);
				tmp = to_string(x);
				//cout << "^ MI HVATA " << tmp << endl;
			}
		}

		if (id == 2) //goleft
		{

			this->addLeftOperand(tmp);
		}
		if (id == 3)
		{
			this->addRightOperand(tmp);
		}

		if (inputPorts == 2)
		{
			//Event::create(this, T, 1); //sounds really bad but ok
			this->result = pow(stod(op.front()), stod(op.back()));
		}
	}
}

void Exp::setT(Time t)
{
	T = t;
}

Time Exp::getT()
{
	return T;
}


void Write::notify(ID id)
{
	// does nothing, for now
	// why?
	Writer::getInstance()->addId(this->id1);
	Writer::getInstance()->addEnd(Scheduler::Instance()->getCurTime());
	Writer::getInstance()->addStart(Scheduler::Instance()->getCurTime() - Memory::getTw());
	Machine::finished.push_back(this);
	list<ArithmeticOperation*>::iterator i = Machine::ongoing.begin();
	while( i != Machine::ongoing.end())
	{
		if ((*i) == this)
		{
			i = Machine::ongoing.erase(i);
			//cout << "IZBRISAN WRITEEEEEEEEEEEEEEEEEEEEE IZ ONGOING ID " << this->getId() << endl;
		}
		else i++;
	}
	cout << "IZVRSENO " << this->id1 << ": " << this->getLeftOperand() << " = " << this->getRightOperand() << " SADA JE " << Scheduler::Instance()->getCurTime() << endl;
	Memory::getInstance()->notify(id);
}

void Write::setT(Time t)
{
	T = t;
}

Time Write::getT()
{
	return T;
}

void ArithmeticOperation::addLeftOperand(string temp)
{
	this->addOperand(temp);
	if (this->inputPorts == 2)
	{
		swapOperands(this);
	}
}

void ArithmeticOperation::addRightOperand(string temp)
{
	this->addOperand(temp);
}

Time ArithmeticOperation::getT()
{
	return T;
}
