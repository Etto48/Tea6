#include "parser.h"

namespace Parser
{
    ParsedMessage::ParsedMessage(std::string message)
    {
        if(message.length()<1)
            this->command = ERROR;
        else
        {
            switch(message[0])
            {
                case ADD:
                    this->command = ADD;
                    break;
                case QUERY:
                    this->command = QUERY;
                    break;
                default:
                    this->command = ERROR;
                    return;
            }
            message.erase(0,1);
            std::istringstream iss{message};
            std::vector<std::string> results{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>()};
            this->args = results;
        } 
    }
std::string ParsedMessage::str()
        {
            std::string ret = "";
            ret+=command;
            for(size_t i = 0; i<args.size();i++)
            {
                ret+=i!=0?" ":"";
                ret+=args[i];
            }
            ret+='\n';
            return ret;
        }

    namespace errorMessages
    {
        std::string AlreadyPresent{"eAlreadyPresent\n"};
        std::string NotPresent{"eNotPresent\n"};
        std::string Protocol{"eProtocol\n"};
    }

}