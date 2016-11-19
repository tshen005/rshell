
#ifndef COMMANDNODE_H
#define COMMANDNODE_H

//std included
#include <iostream>
//files included
#include "connector.h"
#include "command.h"
using namespace std;

class CommandNode: public Command, public Connector
{
	private:
		Command c;		//for command
		Connector con; 	//for connector
	
	public:
		//default constructors
		CommandNode(){};	
		CommandNode(const string &command, const string &connector)
		{
			c = Command(command);
			con = Connector(connector);
		}

		Command getCommand() const		//get command
		{ return c; }

		Connector getConnector() const	//get connector
		{ return con; }

		friend ostream& operator <<(ostream& os, const CommandNode cn);
};

ostream& operator <<(ostream& os, const CommandNode cn)
{
	os << cn.getCommand() << " " << cn.getConnector();
	return os;
}


#endif
