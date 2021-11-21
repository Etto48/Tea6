#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

namespace Parser
{
    enum MessageType
    {
        ADD = 'a',
        QUERY = 'q',
        ERROR = 'e'
    };
    struct ParsedMessage
    {
        MessageType command;
        std::vector<std::string> args;
        ParsedMessage(std::string message);
        std::string str();
    };

    namespace errorMessages
    {
        extern std::string AlreadyPresent;
        extern std::string NotPresent;
        extern std::string Protocol;
    };

};