#ifndef SHELL_H
#define SHELL_H

//std included
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <utility>
//syscalls
#include <unistd.h> //execvp fork cd ..
#include <sys/wait.h> //waitpid
#include <errno.h>
//files included
#include "parse.h"
#include "commandnode.h"
#include "test.h"

using namespace std;

class Shell
{
 private:
  bool status;		//check status of whether shell cont. running or not
    char* pwd;
    char* oldpwd;
 public:
    
  Shell(){
      oldpwd=getenv("HOME");
      pwd=getenv("HOME");
  };			//default constructor
  
  bool getStatus()	//get current status of shell
  { return status; }
  
  void start()		//start shell program
  { status = true; }
  
  void finish()		//shell is done
  { status = false; }
  
  pair<bool,bool> execute(const CommandNode & cn) {
      
      //initialize variables and values
      bool returnStatus = false;
      bool child = false;
      string exe = cn.getCommand().getExec();
      char beg = exe.at(0);					//get front [
     char end;
     //char end = NULL;
      if( cn.getCommand().getArgListStr().size() > 0 ){
	end = cn.getCommand().getArgListStr().at(cn.getCommand().getArgListStr().size()-1);		//get back ]
      }
      if(exe.length() > 1)			//if exe has [] w/out space
	{ 
	  if(exe.at(1) == ']' || exe.at(exe.size()-1) == ']')
	    { end = ']'; }
	}
      if(exe == "Test")
	{exe = "test";}
      //cout << "exe: " << exe << endl; 
      //cout << cn << endl;
      //cout << "end: " << end << endl;
      
      //Check to see if command is cd
      if(exe == "cd")
	{
	  //cout << "cd might happend:" << endl;
	  int argSize = cn.getCommand().getArgList().size();
	  errno = 0;
	  
	  if(argSize > 0){
	     if(cn.getCommand().getArgListStr() == "~"){
             oldpwd=getenv("PWD");
            pwd=getenv("HOME");
            chdir(pwd);
            setenv("PWD",pwd,1);
          }
          else if(cn.getCommand().getArgListStr() == "-") {
              //cout << oldpwd << "^^^" << endl;
              pwd=getenv("PWD");
              chdir(oldpwd);
              setenv("PWD",oldpwd,1);
              oldpwd = pwd;
          }
	      else if(cn.getCommand().getArgListStr() != "-"){
              pwd=getenv("PWD");
              oldpwd=pwd;
              string a = oldpwd;
              
              //cout << oldpwd << endl;
              chdir(cn.getCommand().getArgList_c_str());
              char* c1=strcat(pwd,"/");
              char* c=strcat(c1,cn.getCommand().getArgList_c_str());
              //cout << c << "**" << endl;
              setenv("PWD",c,1);
              cout << getenv("PWD") << endl;
              oldpwd=const_cast<char*>(a.c_str());
              cout << oldpwd << endl;
          }
	      //cout << "error:" << errno << endl;
	    }
	  else
	    {
	      //cd to home directory
            oldpwd = getenv("PWD");
            
            pwd=getenv("HOME");
            chdir(pwd);
            setenv("PWD",pwd,1);
	      //cout << "error:" << errno << endl;
	    }
	  if(errno != 0)
	    {
	      cout << "rshell: " << exe << ": " +  cn.getCommand().getArgListStr() << ": " << strerror(errno) << endl;
	      child = false;
	      return make_pair(false,child);
	    }
	  else
	    {
	      child = false;
	      return make_pair(true,child);
	    }
	}
      else if(exe == "test" || beg == '[')	//check to see if command is test
	{
	  if((end != ']' || end == '\0') && exe != "test")	//if end bracket not found
	    {
	      cerr << "rshell: [: missing `]'" << endl; 
	      child = false;
	      return make_pair(false, child); 
	    }
	  
	  Test tStat;							//make test object
	  int condition = tStat.runTest(exe, cn.getCommand().getArgListStr());		//pass argument of command to test class		
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
	}
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
	      
	      for(unsigned int i = 0; i < cn.getCommand().getArgList().size(); i++)
		{
		  //add c string to arguement list
		  //need to use const cast to remove error: invalid conversion from ‘const char*’ to ‘char*’ [-fpermissive] error
		  arguementList[i+1] = const_cast<char*>(cn.getCommand().getArgList().at(i).c_str());
		  //cout << arguementList[i] << endl;
		  arguementList[i+2] = 0;
		}
	      
	      //cout << command.c_str() << " ";
	      //for(unsigned int i = 0; i <  cn.getCommand().getArgList().size() && i < 33 ; i++)
	      //{ cout << arguementList[i] << " "; }
	      //cout << endl;
	      //cout << "execvp" << endl;
	      
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
		  cout << "rshell: " << exe << ": " +  cn.getCommand().getArgListStr() << ": " << strerror(errno) << endl;
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
    Parse p;
    ///split on parentheses
    //vector<string> parenParts = p.parenSplit(commands);
    
    //Split on ';'
    vector<string> commandParts = p.split(commands,";");
    
    //Now go through command parts and create List of commandNode
    string command = "";
    vector<CommandNode> commandList;
    
    for(unsigned int i = 0; i < commandParts.size(); i++)
      {
	//cout << commandParts[i] << endl;
	//Parse now on || and &&
	command = commandParts[i];
	string tmp = "";
	int orIndex = command.find("||");
	int andIndex = command.find("&&");
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
	    commandList.push_back(CommandNode(tmp,connector));
	    command = command.substr(first+connector.length());
	    orIndex = command.find("||");
	    andIndex = command.find("&&");
	  }
	
	if(command.length() > 0)
	  { commandList.push_back(CommandNode(command,";")); }
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
	CommandNode currentNode = commandList[i];
	Connector con = currentNode.getConnector();
	bool run = false;
	string cmdstr = p.ltrim(currentNode.getCommand().getExec(), " ");
	
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
	    if(preCon.isOR())
	      { run = false; }
	    else if(preCon.isAND())
	      { run = true; }
	    else
	      { run = true; }
	  }
	else
	  {
	    if(preCon.isOR())
	      {
		//cout << "has ||" << endl;
		run = true;
		chainStatus = true;
	      }
	    else if(preCon.isAND())
	      {
		//cout << "has &&" << endl;
		run = false;
	      }
	  }
	
	if(con.isSEMI())
	  {
	    //cout << "has ;" << endl;
	    chainStatus = true;
	  }
	
	if(run)
	  {
	    pair<bool,bool> ret;
	    
	    //check if command is exit
	    string exitcmd = p.ltrimr(commandList[i].getCommand().getExec(), " ");
	    if(exitcmd == "exit")
	      {
		finish();
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
  void initialize()	
  {
    start();	
    bool child = false;
    
    //display current logged in user & hostname then start shell program
    while(status && !child)		
      {
	string input; 
	
	//Output prompt
	char username[1024] ={0};
	int getUsername_resp = getlogin_r(username,sizeof(username)-1);
	
	
	char hostname [1024] = {0};
	int getHostname_resp = gethostname(hostname,sizeof(hostname)-1);
	//cout << getUsername_resp << " " << getHostname_resp<< endl;
	
	if((getHostname_resp == 0) && (getUsername_resp == 0))
	  {
	    //string username_str = "";
	    //string hostname_str = "";
	    //cout << username_str <<"@"<<hostname_str;
	    printf("[%s@%s %s]", username,hostname,getenv("PWD"));
	  }
	cout <<"$ ";	//start getting inputs for shell program
	
	getline(cin, input);	//get line input
	
				//Parse input
	Parse p;				//parse object
	input = p.ltrimr(input," ");
	input = p.ltrimr(input, "\t");
	
	if(input == "")			//if empty then continue regardless
	  { continue; }
	//cout << input << endl;
	
	if(input != "exit")
	  {
	    //Split inputs on # and ignore everything afterwards
	    string commands = p.split(input,"#")[0];
	    
	    int open = p.findParen(commands, "(");		//find # of '('
	    int close = p.findParen(commands, ")");		//find # of ')'
	    int rpn_count = 0;
	  char c ;
	  //char c = NULL;
	    for(unsigned int rpn_i = 0; rpn_i < commands.size(); rpn_i++ ){
	      c = commands.at(rpn_i);
	      if(c == '('){
		rpn_count++;
	      }
	      else if(c == ')'){
		if(rpn_count == 0){
		  rpn_count -= 2;
		}
		else{
		  rpn_count--;
		}
	      }
	    }
	    
	    if(open != close || rpn_count != 0)			//if # of '(' != ')' then exit
	      {
		cout << "rshell: syntax error near unexpected token \')\'" << endl;
		break;
	      }
	    commands = p.ltrimr(commands," ");
	    bool commandChainStatus = true;
	    Connector commandChainPrevCon;
	    bool child = false;
	    while(commands.length() > 0)
	      {
		string commandtmp = "";
		bool run = false;
		//cout <<"Before:" << commands << endl;
		//bool foundParen = false;
		int parenIndex = commands.find("(");
		if(parenIndex != int(string::npos)){
		  int findEndParen = commands.find(")");
		  if(parenIndex == 0){
		    if(int(findEndParen) == int(commands.length()-1)){
		      commandtmp = "";
		    }
		    else{
		      commandtmp = commands.substr(findEndParen+1);
		    }
		    
		    commands = p.ltrimr(commands,"(");
		    commands = commands.substr(0,findEndParen-1);
		  }
		  else{
		    //cout << "else" << endl;
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
			commandtmp = commands.substr(parenIndex);
		      }
		    
		    
		    commands = commands.substr(0,parenIndex-1);
		   
		  }
		  //cout << "command =" << commands << endl;
		  //cout << "tmp=" << commandtmp << endl;
		  //check chain
		  if(commandChainStatus){
		    if(commandChainPrevCon.isOR())
		      {
			run = false;
			//cout << "run false" << endl;
		      }
		    else if(commandChainPrevCon.isAND()){
		      run = true;
		      //cout << "run true" << endl;
		    }
		    else{
		      //cout << "run else" << endl;
		      run = true;
		    }
		  }
		  else{
		    if(commandChainPrevCon.isOR())
		      {
			run = true;
			commandChainStatus = true;
		      }
		    else if(commandChainPrevCon.isAND()){
		      run = false;
		    }
		  }
		  
		  commandtmp = p.ltrimr(commandtmp," ");
		  int orFind = commandtmp.find("||");
		  int andFind = commandtmp.find("&&");
		  if(orFind == 0){
		    //cout << "found or" << endl;
		    commandChainPrevCon = Connector("||");
		    commandtmp = commandtmp.substr(orFind+2);
		  }
		  else if (andFind == 0){
		    //cout << "found and" << endl;
		    commandChainPrevCon = Connector("&&");
		    commandtmp = commandtmp.substr(andFind+2);
		  }
		  else{
		    //cout << "found else" << endl;
		    commandChainPrevCon = Connector(";");
		  }
		  commandtmp = p.ltrimr(commandtmp,";");
		  commandtmp = p.ltrimr(commandtmp," ");
		  
		  //cout <<"BAfter:" << commandtmp << endl;
		}
		else{
		  if(commandChainStatus){
		    if(commandChainPrevCon.isOR())
		      {
			run = false;
		      }
		    else if(commandChainPrevCon.isAND()){
		      run = true;
		    }
		    else{
		      run = true;
		    }
		  }
		  else{
		    if(commandChainPrevCon.isOR())
		      {
			run = true;
			commandChainStatus = true;
		      }
		    else if(commandChainPrevCon.isAND()){
		      run = false;
		    }
		
		  }
		  //check for ; ?
		  commandChainPrevCon = Connector(";");
		  commandtmp = "";
		}
		if(run){
		  //cout << "run command:" << commands << endl;
		  pair<bool,bool> runCommands =  runChain(commands);
		  commandChainStatus = runCommands.first;
		  child = runCommands.second;
		  /*
		  if(commandChainStatus)
		    cout << "TRUES" << endl;
		  else
		    cout << "FALZES" << endl;
		  */
		}
		commands = commandtmp;
		//cout <<"After:" << commands << endl;
		if(child){
		  //cout << "child" << endl;
		  break;
		}
		else{
		  //cout << "Parent" << endl;
		}
	      }
	    if(child){
	      break;
	    }
	  }
	else
	  { break; }	//if user input is exit then break loop and exit
      }
    finish();
  }
};

#endif
