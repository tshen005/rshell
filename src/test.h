//
//  test.h
//  rshell
//
//  Created by shen on 16/11/2.
//  Copyright © 2016年 shen. All rights reserved.
//

#ifndef test_h
#define test_h

#include <utility>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "commandline.h"

class Test : public CommandLine{
private:
    string currentFlag;
    
public:
    Test() {this->currentFlag = "";}
    
    string getFlag() {
        return this->currentFlag;
    }
    
    // test if the flags are valid
    bool ifFlagValid(string flag) {
        if(flag == "-e" or flag == "-f" or flag == "-d") {
            return true;
        }
        return false;
    }
    
    int runTest(string exec, string cn) {
        Analyse* a = new Analyse();
        bool status;
        
        string add, cnTrim;
        string arg;
        if(exec != "test") {
            add = exec + " " + cn;
            cnTrim = a->removeSpace(add);
        }
        else {
            cnTrim = a->removeSpace(cn);
        }
        vector<string> tmp = a->split(cnTrim, " ");
        
        if(cnTrim[(int)cnTrim.size()-1] != ' ') {
            if(exec == "test" && cnTrim.empty()) return 1;
            if(tmp.size() <= 1 and cn[0] == '-') return 0;
        }
        
        if((int)tmp.size() > 1) {
            currentFlag = tmp[0];
            arg = tmp[1];
        }
        if((int)tmp.size() == 1 and cnTrim[0] == '-') return 0;
        
        else {
            currentFlag = "-e";
            arg = tmp[0];
        }
        
        if(currentFlag == arg) {
            return 0;
        }
        
        status = this->ifFlagValid(currentFlag);
        
        if(status) {
            struct stat info;
            bool exists = false;
            
            if(stat(arg.c_str(),&info) != 0) {
                exists = false;
            }
            else {
                exists = true;
            }
            
            if(currentFlag == "-e") {
                if(exists) {
                    return 0;
                }
                else {
                    return 1;
                }
            }
            
            if(currentFlag == "-f") {
                if(S_ISREG(info.st_mode) && exists) {
                    return 0;
                }
                else {
                    return 1;
                }
            }
            
            if(currentFlag == "-d") {
                if(S_ISREG(info.st_mode) && exists) {
                    return 0;
                }
                else {
                    return 1;
                }
            }
            return 0;
        }
        
        return -1;
    }
    
};

#endif /* test_h */
