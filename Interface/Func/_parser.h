#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string>
#include <arpa/inet.h>
#include <list>
#include <iostream>

//Può ricevere soltanto messaggi inizialmente
//q <username>
//a <username>

struct Message{
    std::string s;
    in6_addr addr;
};

enum printCodes { _allMessages, _parsedMessages};

class Parser{
    private: 
            std::list<Message> parsedMessages;
            std::list<Message> allMessages;
    public : 
        Parser();
        bool operator >> (Message& m);
        bool operator << (printCodes &p);
        
};