//
//  command.h
//  rshell
//
//  Created by shen on 16/11/2.
//  Copyright © 2016年 shen. All rights reserved.
//

#ifndef command_h
#define command_h

#include "analyse.h"
class Command {
private:
    vector<string> argu;
    string exectuable;
    
public:
    Command() {}
    
    // split the cmd into exe and argumnets
    Command(string cmd) {
        Analyse* a = new Analyse();
        string tmp = a->splitTag(cmd);
        vector<string> tmpp = a->split(tmp," ");
        
        this->exectuable = tmpp[0];
        
        for(int i = 1;i < (int)tmpp.size();++ i) {
            this->argu.push_back(tmpp[i]);
        }
    }
    
    string getExec() {
        return this->exectuable;
    }
    
    vector<string> getArguVec() {
        return this->argu;
    }
    
    
    // return tht arguments in a line in char*
    char* getArguVec_c_str() {
        string s;
        for(int i = 0;i < (int)this->argu.size();++ i) {
            if(i != 0) s += " ";
            s += argu[i];
        }
        
        return const_cast<char*>(s.c_str());
    }
    
    // return tht arguments in a line in string
    string getArguVec_str() {
        string s;
        for(int i = 0;i < (int)this->argu.size();++ i) {
            if(i != 0) s += " ";
            s += argu[i];
        }
        
        return s;
    }
    
    friend ostream& operator <<(ostream& os, Command c) {
        os << c.getExec();
        if((int)c.getArguVec().size() > 0) {
            for(int i = 0;i < (int)c.getArguVec().size();++ i) {
                os << " " << c.getArguVec()[i];
            }
            
        }
        
        return os;
    }
    
};


#endif /* command_h */
