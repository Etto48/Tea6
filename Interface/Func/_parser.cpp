#include "_parser.h"

Parser::Parser(){};

bool Parser::operator<<(printCodes& p){
    switch(p){
        case _allMessages: 
            while(!allMessages.empty()) std::cout<<allMessages.<<std::endl;
             break;
        case _parsedMessages:  std::cout << allMessages break;
    }
}

