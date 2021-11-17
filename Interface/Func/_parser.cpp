#include "_parser.h"

namespace Parser{

    bool Parser::store(Message& m){
        Message p = m;
        Parser::_parsedMsgs.push_back(p);
    }

    bool Parser::store(std::string& s){
        Message m;
        m.s = s;
        m.addr.__u6_addr.__u6_addr32[0] = 0;
        m.addr.__u6_addr.__u6_addr32[1] = 0;
        m.addr.__u6_addr.__u6_addr32[2] = 0;
        m.addr.__u6_addr.__u6_addr32[3] = 0;
        Parser::_parsedMsgs.push_back(m);
    }

    void Parser::printMsgHistory(){
        std::list<Message> l;
        l = Parser::_parsedMsgs;
        while(!Parser::_parsedMsgs.empty()){ std::cout<<l.front().s; l.pop_front(); }
    }

    std::string Parser::parseMessage(Message& m){
        if(m.s.empty())return NULL;
        store(m);
        switch(m.s[0]){
            case 'q':
                //TODO CALLS ROBA DI ETTORE CHE NON SI SA COSA FACCIA
                break;
            case 'a':
                //TODO CALLS ROBA DI ETTORE CHE NON SI SA COSA FACCIA
                break;
            default: return NULL;
        }
    }

    std::string Parser::parseMessage(std::string& s){
        if(s.empty())
        store(s);
        switch(s[0]){
            case 'q':
                //TODO CALLS ROBA DI ETTORE CHE NON SI SA COSA FACCIA
                break;
            case 'a':
                //TODO CALLS ROBA DI ETTORE CHE NON SI SA COSA FACCIA
                break;
            default: return NULL;
        }
    }
}