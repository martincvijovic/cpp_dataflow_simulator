#include "customexceptions.h"
#include "Machine.h"
#include "MemorySingleton.h"
#include <iostream>

using namespace std;

vector<ArithmeticOperation*> Memory::WhoWantsToRead;
vector<string> Memory::NamesToWrite; 
vector<string> Memory::NamesToRead;
vector<double> Memory::ValuesToWrite; 
vector<string> Memory::names; 
vector<double> Memory::values;
vector<int> Memory::left;
vector<int> Memory::right;

int Memory::Tw = 0;
int Memory::Nw = 0;
Memory* Memory::Instance;

Memory * Memory::getInstance()
{
	if (Instance == nullptr)
	{
		Instance = new Memory();
	}
	return Instance;
}

void Memory::set(string varName, double val) //push to q, care for order!
{
	NamesToWrite.push_back(varName);
	ValuesToWrite.push_back(val);
}

void Memory::flushLeft()
{
	left.push_back(1);
	right.push_back(0);
}

void Memory::flushRight()
{
	left.push_back(0);
	right.push_back(1);
}

double Memory::get(string varName) //getter will be called only when the operation is notified that needed variable is written!
{
	try
	{
		for (unsigned int i = 0; i < names.size(); i++)
		{
			if (varName.compare(names[i]) == 0)
			{
				return values[i];
			}
		}
		throw(VariableNotFoundException("Trazena promenljiva ne postoji u memoriji!"));
	}
	catch (VariableNotFoundException& e)
	{
		throw(VariableNotFoundException("Trazena promenljiva ne postoji u memoriji!"));
		//return 0.0;
	}		
}

void Memory::setNw(int nw)
{
	Nw = nw;
}

int Memory::getNw()
{
	return Nw;
}

void Memory::setTw(int tw)
{
	Tw = tw;
}

int Memory::getTw()
{
	return Tw;
}

void Memory::notify(ID id) // memory gets notified when a write is finished, if anyone wants the new variable, notify it that it's ready! 
//INCREASE NW AND CHECK THE Q FOR ANY VARIABLES THAT NEED TO BE WRITTEN!
{
	Nw++; // I should Nw-- in the Machine
	if (NamesToWrite.size())
	{
		names.push_back(NamesToWrite[0]);
		values.push_back(ValuesToWrite[0]);
	}
	


	
	for (unsigned int i = 0; i < NamesToRead.size(); i++)
	{
		if (NamesToRead[i].compare(NamesToWrite[0]) == 0)
		{
			if (left[i] == 1)
			{
				//cout << "NOTIFY-UJEM OPERACIJU BR " << WhoWantsToRead[i]->getId() << " da je " << NamesToRead[i] << " spreman" << endl;
				for (list<ArithmeticOperation*>::iterator j = Machine::queued.begin(); j != Machine::queued.end(); j++)
				{
					if ((*j) == WhoWantsToRead[i])
					{
						//cout << "NOTIFY-UJEM OPERACIJU BR " << (*j)->getId() << " da je " << NamesToRead[i] << " spreman" << endl;
						(*j)->notify(2);
						
					}
				}
				//WhoWantsToRead[i]->notify(2); 
			}
			else
			{
				//cout << "NOTIFY-UJEM OPERACIJU BR " << WhoWantsToRead[i]->getId() << " da je " << NamesToRead[i] << " spreman" << endl;
				for (list<ArithmeticOperation*>::iterator j = Machine::queued.begin(); j != Machine::queued.end(); j++)
				{
					if ((*j) == WhoWantsToRead[i])
					{
						//cout << "NOTIFY-UJEM OPERACIJU BR " << (*j)->getId() << " da je " << NamesToRead[i] << " spreman" << endl;
						(*j)->notify(3);
						
					}
				}
				//WhoWantsToRead[i]->notify(3); //read to right op
			}
			//cout << "NOTIFY-UJEM OPERACIJU BR " << WhoWantsToRead[i]->getId() << " da je " << NamesToRead[i] << " spreman" << endl;
		}
	}
	//cout << "AHAAAAAAAS AAAAAAAAAAAAAAAAAA " << endl;

	

	if (NamesToRead.size() == 1)
	{
		NamesToRead.clear();
		WhoWantsToRead.clear();
	}
	int i = 0;

	vector<string>NamesToReadNew;
	vector<ArithmeticOperation*>WhoWantsToReadNew;

	for (int i = 0; i < NamesToRead.size(); i++)
	{
		if ( (NamesToRead[i].compare(NamesToWrite[0]) != 0))
		{
			NamesToReadNew.push_back(NamesToRead[i]);
			WhoWantsToReadNew.push_back(WhoWantsToRead[i]);
		}
	}
	NamesToRead.clear();
	WhoWantsToRead.clear();

	for (int i = 0; i < NamesToReadNew.size(); i++)
	{
		NamesToRead.push_back(NamesToReadNew[i]);
		WhoWantsToRead.push_back(WhoWantsToReadNew[i]);
	}
	
	if (NamesToWrite.size())
	{
		NamesToWrite.erase(NamesToWrite.begin() + 0);
		ValuesToWrite.erase(ValuesToWrite.begin() + 0);
	}



	
	/*vector<string> NamesToWritenew;
	vector<double> ValuesToWritenew;
	for (int i = 1; i < NamesToWrite.size(); i++)
	{
		ValuesToWritenew[i-1] = ValuesToWrite[i];
		NamesToWritenew[i - 1] = NamesToWrite[i];
	}

	NamesToWrite = NamesToWritenew;
	ValuesToWrite = ValuesToWritenew;*/
	/*cout << "NAMES TO WRITE PRE I POSLE" << endl;
	for (int i = 0; i < NamesToWrite.size(); i++)
	{
		cout << NamesToWrite[i] << " ";
	}
	cout << endl;*/

	

	/*for (int i = 0; i < NamesToWrite.size(); i++)
	{
		cout << NamesToWrite[i] << " ";
	}
	cout << endl;
	*/
	
}


