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
        DATA = 'd',
        ERROR = 'e'
    };
    /**
     * @brief parses a message and splits the arguments, the syntax for a message is
     * ADD: "a<username> <password>"
     * QUERY: "q<username>"
     * DATA: "d{<data[0]> <data[1]> ...}"
     * ERROR: "e<error>"
     */
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