#pragma once
#include <list>
#include <string>
#include <arpa/inet.h>

namespace Index
{
    class Index
    {
    protected:
    public:
    };

    struct UserData
    {
        std::string username;
        in6_addr addr;
        
        UserData(const std::string& username, in6_addr addr);
    };

    class _IndexList
    {
    protected:
        std::list<UserData> ls;
    public:

    };
};