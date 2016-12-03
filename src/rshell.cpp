#include <iostream>
#include <stdlib.h>
//files included
#include "shell.h"
#include "parse.h"
using namespace std;

int main()
{
	Shell sh;				//initialize shell object
	bool status;			//check to cont. shell or not

	sh.initialize();		//start shell program
	status = sh.getStatus();
	
	if(status)
	{
		//cont. looping shell program
		cerr << "The shell did not exit properly :(" << endl;
		return 1;
	}
	else 
	{ return 0; }	//end and exit program	
}
