//
//  connector.h
//  rshell
//
//  Created by shen on 16/11/2.
//  Copyright © 2016年 shen. All rights reserved.
//

#ifndef connector_h
#define connector_h

#include "analyse.h"
#include <iostream>
#include <string>


class Connector{
private:
    string type;
    
public:
    Connector() {}
    
    Connector(string conn) {
        this->type = conn;
    }
    
    // return which the connector is
    int whichConnector() {
        if(type == "||") return 1;
        
        if(type == "&&") return 2;
        
        if(type == ";") return 3;
        
        return -1;
    }
    
    string getType() {
        return type;
    }
    
    friend ostream& operator <<(ostream& os, Connector c) {
        os << c.getType();
        
        return os;
    }
    
};


#endif /* connector_h */
