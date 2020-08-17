#include "customexceptions.h"
#include "Event.h"
#include "Sched.h"
#include "Machine.h"
#include "MemorySingleton.h"
#include "Operation.h"
#include "Writer.h"
#include <fstream>
#include <iostream>
#include <list>
#include <stack>
#include <string>

using namespace std;

Machine* Machine::Instance;



/*Time Write::T;
Time Exp::T;
Time Sum::T;
Time Mul::T;*/

list<ArithmeticOperation*> Machine::queued;
list<ArithmeticOperation*> Machine::ongoing;
list<ArithmeticOperation*> Machine::finished;


Machine * Machine::getInstance()
{
	if (Instance == nullptr)
	{
		Instance = new Machine();
	}
	return Instance;
}


typedef struct NodeMachine
{
	int qnumber;
	string value;
	NodeMachine* left;
	NodeMachine* right;
}TreeNodeM;


void Postorder(TreeNodeM* root)
{
	if (root == nullptr)
	{
		return;
	}
	Postorder(root->left);
	Postorder(root->right);

	cout << root->value;// << " ";
}


string swapchars(string temp)
{
	string temp1 = "";
	for (int i = temp.length()-1; i >= 0; i--)
	{
		temp1 += temp[i];
	}
	return temp1;
}

bool isOperatorM(string c)
{
	const char s = '+';
	const char m = '*';
	const char e = '^';
	const char w = '=';
	if (c[0] == s || c[0] == m || c[0] == e || c[0] == w)
	{
		return true;
	}
	return false;
}

typedef struct op
{
	int id;
	char type;
	string first, second;
}oper;






void Machine::exec(string name) //WORKS
//BUT CHANGE IT, AKO DVAPUTA ZOVES PROCESSNOW, IZMEDJU TIH POZIVA SVI EVENTOVI CE DA SE NASLAZU U ODNOSU NA ZAVRSETAK PRETHODNOG PROCESSNOW-A!
{	
	// three cases, constant (stod), variable(queue for read from memory), other operation (stod)
	// first go through the entire IMF file and create the operation 'tree', care about I/O ports, then execute them
	string name1 = "";
	for (int i = 0; i < name.length() - 4; i++)
	{
		name1 += name[i];
	}
	
	name1 += ".imf";
	ifstream file;
	file.open(name1);
	string c;

	

	
	while (getline(file, c)) //push->queued
	{ //care for multi-digit numbers
		ArithmeticOperation* temp;

		int index = 0;
		string scan = "";
		while (!isOperatorM(scan))
		{
			index++;
			scan = "";
			scan += c[index];
		}

		if (c[index] == '+')
		{
			temp = new Sum();
			temp->setUID('+');
		}
		else if (c[index] == '*')
		{
			temp = new Mul();
			temp->setUID('*');

		}
		else if (c[index] == '^')
		{
			temp = new Exp();
			temp->setUID('^');
		}
		else
		{
			temp = new Write();
			temp->setUID('=');
		}

		string id = ""; //setting ID
		int kk = 1;
		while (c[kk] != ')')
		{
			id += c[kk];
			kk++;
		}
		temp->setId(stoi(id));
		
		
		//lets check if there's any operation results as operands
		//first left

		//previous operation has a goright flag = false so now we are checking the left operand

		if (c[index+2] == '(')
		{
			string idToFind = "";
			int k = index+3;
			while (c[k] != ')')
			{
				idToFind += c[k];
				k++;
			}

			for (list<ArithmeticOperation*>::iterator i = queued.begin(); i != queued.end(); i++)
			{
				if ((*i)->getId() == stoi(idToFind))
				{
					(*i)->setReceiver(temp); //dont go right
				}
			}
		}
		else
		{
			if (isdigit(c[index+2]))
			{
				int k = index+2;
				string val = "";
				while (c[k] != ' ')
				{
					val += c[k];
					k++;
				}
				temp->addLeftOperand(val);
			}
			else
			{
				string val = "";
				val += c[index+2];
				if (temp->getUID() != '=')
				{
					try
					{
						double x = Memory::getInstance()->get(val);
						temp->addLeftOperand(to_string(x));
					}
					catch (VariableNotFoundException& e)
					{
						Memory::getInstance()->addToReadingQueue(temp, val);
						Memory::getInstance()->flushLeft();
					}
				}
				else
				{
					temp->addLeftOperand(val);
				}
			}
		}

		//now we are checking the right operand so goright gets true when we get the prev op, its goright gets true!

		if (c[c.length() - 1] == ')')
		{
			string idToFind = "";
			int k = c.length()-2;
			while (c[k] != '(')
			{
				idToFind += c[k];
				k--;
			}
			idToFind = swapchars(idToFind);
			for (list<ArithmeticOperation*>::iterator i = queued.begin(); i != queued.end(); i++)
			{
				if ((*i)->getId() == stoi(idToFind))
				{
					(*i)->isRightOperand();
					(*i)->setReceiver(temp); //dont go right
				}
			}
		}
		else
		{
			if (isdigit(c[c.length()-1]))
			{
				int k = c.length() - 1;
				string val = "";
				while (c[k] != ' ')
				{
					val += c[k];
					k--;
				}
				val = swapchars(val);
				temp->addRightOperand(val);
				if (temp->getUID() == '=')
				{
					string tmpname = temp->getLeftOperand();
					double tmpval = stod(temp->getRightOperand());
					Memory::getInstance()->set(tmpname,tmpval);
				}
			}
			else
			{
				string val = "";
				val += c[c.length()-1];
				try
				{
					double x = Memory::getInstance()->get(val);
					temp->addRightOperand(to_string(x));
				}
				catch (VariableNotFoundException &e)
				{
					Memory::getInstance()->addToReadingQueue(temp, val);
					Memory::getInstance()->flushRight();
				}
			}
		}
		//so far so good, i guess
		queued.push_back(temp);
	}
	//'tree' created, lets test it
	cout << endl << endl;
	for (list<ArithmeticOperation*>::iterator i = queued.begin(); i != queued.end(); i++)
	{
		cout << "Operacija: " << (*i)->getId() << endl;
		cout << "Da li se rezultat salje dalje: " << ((*i)->getNext() != nullptr) << endl;
		cout << "Broj punih input portova: " << (*i)->getUsedPorts();
		cout << endl << endl;
	}

	vector<ArithmeticOperation*> listatemp = Memory::getInstance()->getWhowants();
	vector<string> imenatemp = Memory::getInstance()->getNamesToRead();

	for (int i = 0; i < imenatemp.size(); i++)
	{
		cout << "Operacija br " << listatemp[i]->getId() << " zeli da procita promenljivu " << imenatemp[i];
		cout << endl << endl;
	}

	list<ArithmeticOperation*>::iterator i = queued.begin();
	while (i != queued.end())
	{
		if ((*i)->getUsedPorts() == 2)
		{
			ongoing.push_back(*i);
			queued.erase(i++);
		}
		else i++;
	}
	
	string logname = "";
	for (int i = 0; i < name1.length() - 4; i++)
	{
		logname += name[i];
	}
	logname += ".log";
	
	Writer::getInstance()->setFileName(logname);
	

	int sol = 0;


	while (ongoing.size() || queued.size()) //we doing something
	{
		cout << endl << "ULAZIMO" << endl;
		list<ArithmeticOperation*>::iterator i = ongoing.begin();
		while (i != ongoing.end())
		{
			char temporary = (*i)->getUID();
			if (temporary == '+')
			{
				Event::create(*i, Sum::getT(), 1);
				i++;
			}
			else if (temporary == '*')
			{
				Event::create(*i, Mul::getT(), 1);
				i++;
			}
			else if (temporary == '^')
			{
				Event::create(*i, Exp::getT(), 1);
				i++;
			}
			else
			{
				if (Memory::getNw() > 0)
				{
					Event::create(*i, Memory::getTw()); // maybe -- the nw then if its > 0 create the event?
					i++;
					Memory::setNw(Memory::getNw() - 1);
				}
				else
				{
					queued.push_back(*i);
					i = ongoing.erase(i);
				}
			}
		}

		sol++;
		bool p = true;
		cout << "Trenutno vreme: " << Scheduler::Instance()->getCurTime() << endl;


		Scheduler::Instance()->processNow();

		for (list<ArithmeticOperation*>::iterator ii = finished.begin(); ii != finished.end(); ii++)
		{
			list<ArithmeticOperation*>::iterator jj = ongoing.begin();
			while (jj != ongoing.end())
			{
				if ((*ii) = (*jj))
				{
					jj = ongoing.erase(jj);
				}
				else jj++;
			}
		}
		list<ArithmeticOperation*>::iterator jj = ongoing.begin();
		while (jj != ongoing.end())
		{
			if ((*jj)->getUID()=='=')
			{
				jj = ongoing.erase(jj);
			}
			else jj++;
		}
		

		cout << "Vreme nakon " << sol <<". prolaska: " << Scheduler::Instance()->getCurTime() << endl;


		/*for (list<ArithmeticOperation*>::iterator ii = ongoing.begin(); ii != ongoing.end(); ii++)
		{
			finished.push_back(*ii);
		}
		ongoing.clear();*/ //REMOVE 1 BY 1 IN NOTIFY!

		i = queued.begin();

		while (i != queued.end())
		{
			if ((*i)->getUsedPorts() == 2)
			{
				ongoing.push_back(*i);
				queued.erase(i++);
				//if (!queued.size()) break;
			}
			else i++;
		}

	}


	Writer::getInstance()->go();

}
