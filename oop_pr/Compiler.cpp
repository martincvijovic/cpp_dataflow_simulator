#include "Compiler.h"
#include <fstream>
#include <iostream>
#include <list>
#include <stack>
#include <string>

using namespace std;

Compiler* Compiler::Instance;

Compiler* Compiler::getInstance() 
{
	if (Instance == nullptr)
	{
		Instance = new Compiler();
	}
	return Instance;
}


typedef struct Node
{
	int qnumber;
	string value;
	Node* left;
	Node* right;
}TreeNode;

bool isOperator(string c)
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

int precedence(string c)
{
	switch (c[0])
	{
	case '=':return 0;
	case '+':return 1;
	case '*':return 2;
	case '^':return 3;
	}
}

void printPreorder(TreeNode* root)
{
	if (root == nullptr) return;
	if (root->qnumber) cout << "(" << root->qnumber << ")";
	cout << root->value << " ";
	printPreorder(root->left);
	printPreorder(root->right);
}

void printPostorder(TreeNode* root)
{
	if (root == nullptr)
	{
		return;
	}
	printPostorder(root->left);
	printPostorder(root->right);
	
	cout << root->value << " ";
}



void Compiler::Compile(ifstream& program, string name) //works like charm, i think
{


	ofstream out;
	string outname = "";
	for (int i = 0; i < name.length() - 4; i++)
	{
		outname += name[i];
	}
	outname += ".imf";
	out.open(outname);


	int countop = 1;
	list<TreeNode*> root; //i need a list of these nodes, run all of them (leaves first) at the same moment?
	string infix;
	list<string> convertable;
	list<string> postfix;
	int order = 1;
	while (getline(program, infix))
	{
		convertable.clear();
		postfix.clear();
		string temp;
		for (int i = 0; i < infix.length(); i++)
		{
			if ((int)infix[i] >= 48 && (int)infix[i] <= 57)
			{
				temp += infix[i];
			}
			else
			{
				if(temp.compare("") != 0) convertable.push_back(temp);
				temp = "";//infix[i];
				temp += infix[i];
				convertable.push_back(temp);
				temp = "";
			}
		} //debugged and works
		convertable.push_back(temp);
		//for (list<string>::iterator i = convertable.begin(); i != convertable.end(); i++) cout << *i << " ";
		//cout << endl;
		stack<string> q;
		int EQFLAG = 0;
		//now create a postfix list of strings, then make a tree out of it
		for (list<string>::iterator i = convertable.begin(); i != convertable.end(); i++)
		{
			if ((*i).compare("=") == 0)
			{
				EQFLAG = 1;
			}
			if (!isOperator(*i))
			{
				postfix.push_back(*i);
			}
			else
			{
				//string ope = q.top();
				if (!q.size())
				{
					q.push(*i);
				}
				else if (precedence(*i) > precedence(q.top())) q.push(*i);
				else
				{
					while (q.size() && precedence(*i) <= precedence(q.top())) //might throw an error
					{
						postfix.push_back(q.top());
						q.pop();
					}
					q.push(*i);
				}
			}
		} 
		
		while (q.size()) { postfix.push_back(q.top()); q.pop(); }
		if(EQFLAG)for (list<string>::iterator i = postfix.begin(); i != postfix.end(); i++)
		{
			if ((*i).compare("=") == 0)
			{
				EQFLAG = 0;
			}
		}
		if(EQFLAG)postfix.push_back("=");
		stack<TreeNode*> qq;//we got our postfix, lets make a tree and ***push its root into the root list***

		

		cout << "POSTFIX: ";  for (list<string>::iterator i = postfix.begin(); i != postfix.end(); i++) cout << *i << " ";

		TreeNode* t = new TreeNode;
		TreeNode* t1 = new TreeNode;
		TreeNode* t2 = new TreeNode;
		t->left = nullptr;
		t->right = nullptr;
		t1->left = nullptr;
		t1->right = nullptr;
		t2->left = nullptr;
		t2->right = nullptr;
		TreeNode* roottemp;
		
		for (list<string>::iterator i = postfix.begin(); i != postfix.end(); i++)
		{
			/*if ((*i).compare("=") == 0)
			{
				EQFLAG = 1;
			}*/
			if (!isOperator(*i))
			{
				TreeNode* temp = new TreeNode;
				temp->left = nullptr;
				temp->right = nullptr;
				temp->value = *i;
				temp->qnumber = 0;
				qq.push(temp);
			}
			else
			{
				TreeNode* temp = new TreeNode;
				temp->right = qq.top();
				qq.pop();
				temp->left = qq.top();
				qq.pop();
				temp->value = *i;
				temp->qnumber = order++;
				qq.push(temp);
			}
			roottemp = qq.top();
		}
		/*if (EQFLAG)
		{
			TreeNode* rootnew = new TreeNode();
			rootnew->value = "=";
			TreeNode* leftnew = new TreeNode();
			leftnew->value = *(postfix.begin());
			rootnew->left = leftnew;
			rootnew->right = roottemp;
			roottemp = rootnew;
		}*/
		//lets test our tree
		//cout << endl << endl;
		//printPostorder(roottemp->right);
		
		/*THIS IS OUR INTERMEDIATE FORM*/
		
		//cout << endl << endl;

	



		stack<TreeNode*> o;
		for (list<string>::iterator i = postfix.begin(); i != postfix.end(); i++)
		{
			if (!isOperator(*i))
			{
				TreeNode* temp = new TreeNode();
				temp->left = nullptr;
				temp->right = nullptr;
				temp->value = *i;
				o.push(temp);
			}
			else
			{
				TreeNode* temp = new TreeNode();
				temp->value = *i;
				temp->right = o.top();
				o.pop();
				temp->qnumber = countop++;
				temp->left = o.top();
				o.pop();
				o.push(temp);
				out << "(" << temp->qnumber << ") " << temp->value << " ";
				if (!isOperator(temp->left->value)) out << temp->left->value; else out << "(" << temp->left->qnumber << ")"; out << " ";
				if (!isOperator(temp->right->value)) out << temp->right->value; else out << "(" << temp->right->qnumber << ")";
				out << endl;
			}
		}


		//printPreorder(roottemp);
		//cout << endl << endl;
		//Tree Succesfully Created (debugged and works)
		root.push_back(roottemp);
		cout << endl << "POSTORDER: ";
		printPostorder(roottemp);
		cout << endl << endl;
	}
}
