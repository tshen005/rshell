//
//  commandline.h
//  rshell
//
//  Created by shen on 16/11/2.
//  Copyright © 2016年 shen. All rights reserved.
//

#ifndef commandline_h
#define commandline_h

#include "command.h"
#include "connector.h"

class  CommandLine : public Command, public Connector {
private:
    Command* cmd;
    Connector* conn;
    
public:
    CommandLine() {}
    
    CommandLine(string command, string connector) {
        this->cmd = new Command(command);
        this->conn = new Connector(connector);
    }
    
    Connector getConn() {
        return *this->conn;
    }
    
    Command getCmd() {
        return *this->cmd;
    }
    
    friend ostream& operator <<(ostream& os, CommandLine* cn) {
        os << cn->getCmd() << " " << cn->getConn();
        return os;
    }
};

#endif /* commandline_h */
