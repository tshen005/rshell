#ifndef COMMAND_H
#define COMMAND_H

//standard library included
#include <iostream>
#include <string>
#include <vector>
//files included
#include "parse.h"
using namespace std;

class Command
{
	private:
		vector<string> argList;		//argument list
		string executable;			//executable of the command (ls, echo, etc.)
	
	public:
		Command(){};				//default constructor

		Command(string command)
		{
			Parse p;
			command = p.ltrim(command," ");
			vector<string> tmp = p.split(command," ");	//parse found executable into vector
			executable = p.ltrim(tmp[0]," ");
			//cout << executable << endl;

			if(tmp.size() > 1)		//if vector > 1 add strings to argList vector
			{
				for(unsigned int i = 1; i < tmp.size(); i++)
				{
					string argVal = tmp[i];
					argList.push_back(p.ltrim(argVal," "));
				}	
			}
		}

		//get executable
		string getExec() const	
		{ return executable; }

		//get c_str of executable
		char* getExec_c_str() const
		{ return const_cast<char*>(executable.c_str()); }

		//get argList
		vector<string> getArgList()
		{ return argList; }	
		
		//get ArgList strings
		string getArgListStr() const
		{
			string newString = "";

			for(unsigned int i = 0; i < argList.size(); i++)
			{
				if( i != 0)			//add spaces cept for beginning
				{ newString += " "; }

				newString += argList[i];
			}
			return newString;
		}

		//get c_str of ArgList (same as previous function)
		char* getArgList_c_str()
		{
			string newString = "";

			for(unsigned int i = 0; i < argList.size(); i++)
			{
				if( i != 0)
				{ newString += " "; }

				newString += argList[i];
			}
			return const_cast<char*>(newString.c_str());
		}

		string getCommand_c_str()
		{
			string commandString = "";
			return commandString;
		}

		friend ostream& operator <<(ostream& os, const Command c);
	};

	ostream& operator <<(ostream& os, const Command c)
	{
		if(c.getArgListStr().length() > 0)
		{ os << c.getExec() << " " << c.getArgListStr(); }
		else
		{ os << c.getExec(); }
	
		return os;
	}

#endif
