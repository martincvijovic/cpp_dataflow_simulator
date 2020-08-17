#include "Compiler.h"
#include "Machine.h"
#include "MemorySingleton.h"
#include "Operation.h"
#include "Types.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;


int main() //maybe char*[] argv?
{
	string conf, prog;
	cout << "Unesite ime config i program fajla, razdvojte ih novim redom" << endl << endl;
	getline(cin, conf);
	getline(cin, prog);


	ifstream config;
	config.open(conf);

	ifstream program;
	program.open(prog);

	Compiler::getInstance()->Compile(program,prog); // now you should get an IMF, test it
	
	const char T = 'T';
	const char a = 'a';
	const char m = 'm';
	const char e = 'e';
	const char w = 'w';
	const char N = 'N';
	
	string c;
	while (getline(config,c)) // actually works
	{	
		int x = 0;
		for (unsigned int i = 3; i < c.length() - 1; i++)
		{
			x *= 10;
			x += c[i] - 48;
		}
		
		if (c[0] == T)
		{
			if (c[1] == a)
			{
				Sum::setT(x);
			}
			if (c[1] == m)
			{
				Mul::setT(x);
			}
			if (c[1] == e)
			{
				Exp::setT(x);
			}
			if (c[1] == w)
			{
				Memory::setTw(x);
			}
		}
		else if(c[0] == N)
		{
			Memory::setNw(x);
		}
	}

	cout << "Ta: " << Sum::getT() << endl << "Tm: " << Mul::getT() << endl << "Te: " << Exp::getT() << endl << "Tw: " << Memory::getTw() << endl << "Nw: " << Memory::getNw() << endl;
	cout << "Machine ready...";
	system("PAUSE");

	Machine::getInstance()->exec(prog);
	system("PAUSE");

	config.close();
	program.close();
	

	
	return 0;
}