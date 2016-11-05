//
//  shell.h
//  rshell
//
//  Created by shen on 16/11/2.
//  Copyright © 2016年 shen. All rights reserved.
//

#ifndef shell_h
#define shell_h

#include <stdio.h>
#include <utility>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "commandline.h"
//#include "test.h"



class Shell{
private:
    bool status;
    
public:
    Shell() {}
    
    bool getStatus() {
        return status;
    }
    
    void start() {
        status = true;
    }
    
    void stop() {
        status = false;
    }
    pair<bool,bool> execute(CommandLine cn)
    {
        
        //initialize variables and values
        bool returnStatus = false;
        bool child = false;
        string exe = cn.getCmd().getExec();
    //    char beg = exe.at(0);					//get front [
        char end;
        if( cn.getCmd().getArguVec().size() > 0 ){
            end = cn.getCmd().getArguVec_str().at(cn.getCmd().getArguVec_str().size()-1);		//get back ]
        }
        if(exe.length() > 1)			//if exe has [] w/out space
        {
            if(exe.at(1) == ']' || exe.at(exe.size()-1) == ']')
            { end = ']'; }
        }
        
        
        if(exe == "cd")
        {
            //cout << "cd might happend:" << endl;
            int argSize = (int)cn.getCmd().getArguVec().size();
            errno = 0;
            
            if(argSize > 0)
            {
                if(cn.getCmd().getArguVec_str() == "~")
                { chdir(getenv("HOME")); }
                else
                { chdir(cn.getCmd().getArguVec_c_str()); }
                //cout << "error:" << errno << endl;
            }
            else
            {
                //cd to home directory
                chdir(getenv("HOME"));
                //cout << "error:" << errno << endl;
            }
            if(errno != 0)
            {
                cout << "rshell: " << exe << ": " +  cn.getCmd().getArguVec_str() << ": " << strerror(errno) << endl;
                child = false;
                return make_pair(false,child);
            }
            else
            {
                child = false;
                return make_pair(true,child);
            }
        }
       /* else if(exe == "test" || beg == '[')	//check to see if command is test
        {
            if((end != ']' || end == '\0') && exe != "test")	//if end bracket not found
            {
                cerr << "rshell: [: missing `]'" << endl;
                child = false;
                return make_pair(false, child);
            }
            
            Test tStat;							//make test object
            int condition = tStat.runTest(exe, cn.getCmd().getArguVec_str());		//pass argument of command to test class
            string f = tStat.getFlag();
            
            //cout << "status: " << condition << endl;
            if(condition == -1)
            {
                cout << "rshell: " << exe << ": " + f << ": "<< strerror(errno) << endl;
                child = false;
                return make_pair(false, child);
            }
            else if(condition == 1)
            {
                cout << "(False)" << endl;
                child = false;
                return make_pair(false, child);
            }
            else
            {
                cout << "(True)" << endl;
                child = false;
                return make_pair(true, child);
            }
        }*/
        else			//for shell commands
        {
            pid_t pid = fork();
            
            if(pid > 0)
            {
                //I am the parent process
                //add later check for background check
                int status = errno;
                waitpid(pid,&status,0);
                //cout << "Parent end" << endl;
                if(errno)
                {
                    returnStatus = false;
                    child = false;
                    //cout << errno << endl;
                }
                else
                {
                    returnStatus = true;
                    child = false;
                }
                
                if(status == 0)
                {
                    //cout << "command true w/ status" << status << endl;
                    returnStatus = true;
                    child = false;
                    //cout << "TRUE" << endl;
                }
                else
                {
                    returnStatus = false;
                    child = false;
                    //cout << "FALSE" << endl;
                }
            }
            else if(pid == 0)
            {
                // I am the child process
                child = true;
                //cout << "child start" << endl;
                //execute command
                //What about ~ in command?
                string command = exe;
                //construct arguement list c string array
                //int size = 32;
                char *arguementList[1024];		//standard amount
                memset(arguementList,0,sizeof(arguementList));
                
                //add command
                arguementList[0] =  const_cast<char*>(command.c_str());
                
                for(unsigned int i = 0; i < cn.getCmd().getArguVec().size(); i++)
                {
                    
                    arguementList[i+1] = const_cast<char*>(cn.getCmd().getArguVec().at(i).c_str());
                    //cout << arguementList[i] << endl;
                    arguementList[i+2] = 0;
                }
                
                
                
                int status = 0;
                child = true;
                status = execvp(command.c_str(),arguementList);
                
                //cout << "status::"<< status << endl;
                //cout << "execvp end" << endl;
                if(status >= 0)
                {
                    returnStatus = true;
                    child = true;
                }
                else
                {
                    cout << "rshell: " << exe << ": " +  cn.getCmd().getArguVec_str() << ": " << strerror(errno) << endl;
                    returnStatus = false;
                    child = true;
                }
            }
            else
            {
                //an error occurred
                cerr << "Fork failed" << endl;
                returnStatus = false;
                child = true;
            }
        }
        //cout << "child end" << endl;
        //check status
        return make_pair(returnStatus,child);
    }
    
    pair<bool,bool> runChain(const string &commands){
        bool child = false;
        bool s = false;
        Analyse p;
        ///split on parentheses
        //vector<string> parenParts = p.parenSplit(commands);
        
        //Split on ';'
        vector<string> commandParts = p.split(commands,";");
        
        //Now go through command parts and create List of commandNode
        string command = "";
        vector<CommandLine> commandList;
        
        for(unsigned int i = 0; i < commandParts.size(); i++)
        {
            //cout << commandParts[i] << endl;
            //Parse now on || and &&
            command = commandParts[i];
            string tmp = "";
            int orIndex = (int)command.find("||");
            int andIndex = (int)command.find("&&");
            string connector ="";
            
            while( (int(orIndex) != int(string::npos)) or (int(andIndex) != int(string::npos)) )
            {
                //cout << command << endl;
                int first = 0;
                
                if( (int(orIndex) != int(string::npos)) && (int(andIndex) != int(string::npos)) )
                {
                    //first = (orIndex < andIndex) ? orIndex : andIndex;
                    if(orIndex < andIndex)
                    {
                        first = orIndex;
                        connector = "||";
                    }
                    else if(orIndex > andIndex)
                    {
                        first = andIndex;
                        connector = "&&";
                    }
                }
                else if( (int(orIndex) != int(string::npos)) && (int(andIndex) == int(string::npos)) )
                {
                    first = orIndex;
                    connector = "||";
                }
                else if ( (int(orIndex) == int(string::npos)) && (int(andIndex) != int(string::npos)) )
                {
                    first = andIndex;
                    connector = "&&";
                }
                
                tmp = command.substr(0,first);
                commandList.push_back(CommandLine(tmp,connector));
                command = command.substr(first+connector.length());
                orIndex = (int)command.find("||");
                andIndex = (int)command.find("&&");
            }
            
            if(command.length() > 0)
            { commandList.push_back(CommandLine(command,";")); }
        }
        //cout << commandList.size() << endl;
        //Now for through each command in commandList and exectue them if possible
        //https://en.wikipedia.org/wiki/Bash_(Unix_shell)
        bool chainStatus = true;
        Connector preCon;
        bool currentReturnStatus = false;
        
        //runs every command in command list in proper order
        for(unsigned int i = 0; i < commandList.size(); i++)
        {
            //Execute command node
            CommandLine currentNode = commandList[i];
            Connector con = currentNode.getConn();
            bool run = false;
            string cmdstr = p.removeSpace(currentNode.getCmd().getExec());
            
            //if input starts w/ connectors
            if(cmdstr == "||")
            {
                cerr << "rshell: syntax error near unexpected token \'||\'" << endl;
                break;
            }
            else if(cmdstr == "&&")
            {
                cerr << "rshell: syntax error near unexpected token \'&&\'" << endl;
                break;
            }
            
            //skip logic
            if(chainStatus)
            {
                if(preCon.whichConnector() == 1)
                { run = false; }
                else if(preCon.whichConnector() == 2)
                { run = true; }
                else
                { run = true; }
            }
            else
            {
                if(preCon.whichConnector() == 1)
                {
                    //cout << "has ||" << endl;
                    run = true;
                    chainStatus = true;
                }
                else if(preCon.whichConnector() == 2)
                {
                    //cout << "has &&" << endl;
                    run = false;
                }
            }
            
            if(con.whichConnector() == 3)
            {
                //cout << "has ;" << endl;
                chainStatus = true;
            }
            
            if(run)
            {
                pair<bool,bool> ret;
                
                //check if command is exit
                string exitcmd = p.removeSpace(commandList[i].getCmd().getExec());
                if(exitcmd == "exit")
                {
                    stop();
                    child = true;
                    s = chainStatus;
                    return make_pair(s,child);
                }
                
                //cout << "current cmd: " << commandList[i] << endl;
                ret = execute(commandList[i]);			//execvp() on commands
                currentReturnStatus = ret.first;
                child = ret.second;
                
                if(child)
                {
                    break;
                }
                
                //assign new status
                chainStatus = currentReturnStatus && chainStatus;
            }
            else
            { /*cout << "no run" << endl;*/ }
            
            //assign new status
            //chainStatus = currentReturnStatus && chainStatus;
            preCon = con;
            
            if(child)
            { break; }
        }
        s = chainStatus;
        return make_pair(s,child);
    }
    
    //start shell
    void initialize(){
        start();
        
        //display current logged in user & hostname then start shell program
        while(status)
        {
            string input;
            
            //Output prompt
            char username[1024] ={0};
            int getUsername_resp = getlogin_r(username,sizeof(username)-1);
            
            /*
             //printf("%s\n",username);
             char* home = getlogin();
             if(home != NULL)
             printf("%s\n",home);
             */
            char hostname [1024] = {0};
            int getHostname_resp = gethostname(hostname,sizeof(hostname)-1);
            //cout << getUsername_resp << " " << getHostname_resp<< endl;
            
            if((getHostname_resp == 0) && (getUsername_resp == 0))
            {
                
                cout << username <<"@"<<hostname;
                //printf("%s@%s", username,hostname);
            }
            cout <<"$ ";	//start getting inputs for shell program
            
            getline(cin, input);	//get line input
            
            //Parse input
            Analyse p;				//parse object
            input = p.removeSpace(input);
            //input = p.ltrimr(input, "\t");
            
            if(input == "")			//if empty then continue regardless
            { continue; }
            //cout << input << endl;
            if(input == "exit") {
                break;
            }
            if(input != "exit")
            {
                
                string commands = p.split(input,"#")[0];
                
                commands = p.removeSpace(commands);
                bool commandChainStatus = true;
                Connector commandChainPrevCon;
                bool child = false;
                while(commands.length() > 0)
                {
                    string commandtmp;
                    bool run = false;
                    
                    int fOR  = int(commands.find("||"));
                    int fAND = int(commands.find("&&"));
                    if(fOR != int(string::npos) && fAND != int(string::npos)){
                        
                        if(fOR < fAND){
                            commandtmp = commands.substr(fOR);
                        }
                        else if(fAND < fOR){
                            commandtmp = commands.substr(fAND);
                        }
                    }
                    else if(fOR != int(string::npos) && fAND == int(string::npos) )
                    {
                        commandtmp = commands.substr(fOR);
                    }
                    else if(fOR == int(string::npos)  && fAND != int(string::npos) )
                    {
                        commandtmp = commands.substr(fAND);
                    }
                    else if( fOR == int(string::npos)  && fAND == int(string::npos) )
                    {
                        //                      commandtmp = commands.substr(parenIndex);
                    }
                    
                    if(commandChainStatus){
                        if(commandChainPrevCon.whichConnector() == 1)
                        {
                            run = false;
                        }
                        else if(commandChainPrevCon.whichConnector() == 2){
                            run = true;
                            //cout << "run true" << endl;
                        }
                        else{
                            //cout << "run else" << endl;
                            run = true;
                        }
                    }
                    else{
                        if(commandChainPrevCon.whichConnector() == 1)
                        {
                            run = true;
                            commandChainStatus = true;
                        }
                        else if(commandChainPrevCon.whichConnector() == 2){
                            run = false;
                        }
                    }
                    
                    commandtmp = p.removeSpace(commandtmp);
                    int orFind = (int)commandtmp.find("||");
                    int andFind = (int)commandtmp.find("&&");
                    
                    if(orFind == 0){
                        
                        commandChainPrevCon = Connector("||");
                        commandtmp = commandtmp.substr(orFind+2);
                    }
                    else if (andFind == 0){
                        
                        commandChainPrevCon = Connector("&&");
                        commandtmp = commandtmp.substr(andFind+2);
                    }
                    else{
                        
                        commandChainPrevCon = Connector(";");
                    }
                    p.splitSemi(commandtmp);
                    commandtmp = p.removeSpace(commandtmp);;
                    
                    if(commandChainStatus){
                        if(commandChainPrevCon.whichConnector() == 1)
                        {
                            run = false;
                        }
                        else if(commandChainPrevCon.whichConnector() == 2){
                            run = true;
                        }
                        else{
                            run = true;
                        }
                    }
                    else{
                        if(commandChainPrevCon.whichConnector() == 1)
                        {
                            run = true;
                            commandChainStatus = true;
                        }
                        else if(commandChainPrevCon.whichConnector() == 2){
                            run = false;
                        }
                        
                    }
                    
                    commandChainPrevCon = Connector(";");
                    commandtmp = "";
                    
                    if(run){
                        //cout << "run command:" << commands << endl;
                        pair<bool,bool> runCommands =  runChain(commands);
                        commandChainStatus = runCommands.first;
                        child = runCommands.second;
                    }
                    commands = commandtmp;
                }
            }
        }
        stop();
    }
};


#endif /* shell_h */
