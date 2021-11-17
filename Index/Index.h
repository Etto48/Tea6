#pragma once
#include <list>
#include <string>
#include <arpa/inet.h>

namespace Index
{
    constexpr unsigned int HASH_TABLE_SIZE = 256;
    class Index
    {
    protected:
        _IndexList hashTable[HASH_TABLE_SIZE];
    public:
        bool operator+=(const UserData& entry);
        bool operator-=(const std::string& username);
        UserData* find(const std::string& username);
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