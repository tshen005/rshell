#ifndef CONNECTOR_H
#define CONNECTOR_H

//std included
#include <iostream>
#include <string>
//files included
#include "parse.h"
using namespace std;

class Connector
{
	private:
		string connector_type;	//gets connector type (&&, ||, etc.)

	public:
		//default constructors
		Connector()
		{ connector_type = ""; }

		Connector(const string &connector)
		{ connector_type = connector; }
		
		//get connector type
		string getType() const;
		
		//bool functions
		bool isOR() const 
		{ return (connector_type == "||")?true:false; }
		
		bool isAND() const 
		{ return (connector_type == "&&")?true:false; }
		
		bool isSEMI() const 
		{ return (connector_type == ";")?true:false; }
		
		friend ostream& operator <<(ostream& os, const Connector c);
};

ostream& operator <<(ostream& os, const Connector c)
{
	os << c.getType();
	return os;
}

string Connector::getType() const	//returns type of the connector
{ return connector_type; }

#endif
