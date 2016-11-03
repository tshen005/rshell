//
//  analyse.h
//  rshell
//
//  Created by shen on 16/11/2.
//  Copyright © 2016年 shen. All rights reserved.
//

#ifndef analyse_h
#define analyse_h

#include <iostream>
#include <vector>
#include <string>
#include <cstring>


using namespace std;

class Analyse {
public:
    string s;
    vector<string> cmdVec;
    
public:
    Analyse() {};
    Analyse(string input) {
        this->s = input;
    }
    
    // remove the comments
    string splitTag(string cmd) {
        int index = (int)cmd.find("#");
        string str;
        
        if(index >= 0) {
            str = cmd.substr(0,index);
            str = this->removeSpace(str);
            return str;
        }
        
        return cmd; // return the cmd without comments
    }
    
    // split with semi
    void splitSemi(string cmd) {
        int index = (int)cmd.find(";");
        string tmp;
        
        while (index > 0) {
            tmp = cmd.substr(0,index);
            
            tmp = this->removeSpace(tmp);
            this->cmdVec.push_back(tmp);
            
            cmd = cmd.substr(index+1);
            index = (int)cmd.find(";");
        }
        
        tmp = this->removeSpace(tmp);
        this->cmdVec.push_back(tmp);
    }
    
    
    // remove the spaces in the begin and end of the string
    string removeSpace(string a) {
        for(int i = 0;i < a.size();++ i) {
            if(a[i] != ' ') {
                a = a.substr(i);
                break;
            }
        }
        for(int i = (int)a.size()-1; i >= 0; --i) {
            if(a[i] != ' ') {
                a = a.substr(0,i+1);
                break;
            }
        }
        return a;
    }
    
    // split the string with the given pattern
    vector<string> split(string cmd, string pattern) {
        vector<string> value;
        int index = (int)cmd.find(pattern);
        string tmp;
        while(index > 0) {
            tmp = cmd.substr(0,index);
            tmp = this->removeSpace(tmp);
            value.push_back(tmp);
            cmd = cmd.substr(index+pattern.size());
            index = (int)cmd.find(pattern);
        }
        
        if(cmd.size() > 0) {
            cmd = this->removeSpace(cmd);
            value.push_back(cmd);
        }
        return value;
    }
    
};


#endif /* analyse_h */
