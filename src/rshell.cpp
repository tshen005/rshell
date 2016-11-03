//
//  main.cpp
//  rshell
//
//  Created by shen on 16/11/1.
//  Copyright © 2016年 shen. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <utility>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "shell.h"

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
}


        
