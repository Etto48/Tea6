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
namespace Parser{

    class Parser{
    
        private: 
            std::list<Message> _parsedMsgs;
            bool store(std::string& s);
            bool store(Message& m);
        public: 
            Parser();
            std::string parseMessage(std::string& s);
            std::string parseMessage(Message& m);
            void printMsgHistory();
    };
}