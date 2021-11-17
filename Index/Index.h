#pragma once
#include <list>
#include <string>
#include <algorithm>
#include <arpa/inet.h>

namespace Index
{
    constexpr unsigned int HASH_TABLE_SIZE = 256;
    struct UserData
    {
        std::string username;
        in6_addr addr;
        
        UserData(const std::string& username, in6_addr addr):username(username),addr(addr){};
        /**
         * @brief checks if two UserData structs are equal
         * 
         * @param x the other UserData
         * @return true if they have the same username, false otherwise
         */
        bool operator==(const UserData& x);
    };

    class _IndexList
    {
    protected:
        std::list<UserData> ls;
    public:
        /**
         * @brief adds a new user to the list
         *
         * @param entry struct containing the new user's informations
         * @return true if an entry with the same username was not present and the operation was successful, false otherwise
         */
        bool add(const UserData& entry);
        /**
         * @brief removes a user from the list
         * 
         * @param username the user's name
         * @return true if the username is found and the entry removed, false otherwise
         */
        bool remove(const std::string& username);
        /**
         * @brief finds a user in the list
         * 
         * @param username the user's name
         * @return nullptr if the username was not found, a pointer to the UserData struct in the index otherways
         */
        UserData* find(const std::string& username);
    };

    class Index
    {
    protected:
        _IndexList hashTable[HASH_TABLE_SIZE];
        pthread_mutex_t indexMutex;
        /**
         * @brief the return is calculated summing up every char in the username mod 256
         * 
         * @param username the user's name
         * @return index to use in hashTable
         */
        uint8_t hashIndex(const std::string& username);

    public:
        /**
         * @brief Construct a new Index object and initialize mutex
         * 
         */
        Index();
        /**
         * @brief adds a new user to the index
         * 
         * @param entry struct containing the new user's informations
         * @return true if an entry with the same username was not present and the operation was successful, false otherwise
         */
        bool operator+=(const UserData& entry);
        /**
         * @brief removes a user from the index
         * 
         * @param username the user's name
         * @return true if the username is found and the entry removed, false otherwise
         */
        bool operator-=(const std::string& username);
        /**
         * @brief finds a user in the index
         * 
         * @param username the user's name
         * @return nullptr if the username was not found, a pointer to the UserData struct in the index otherways
         */
        UserData* find(const std::string& username);
        /**
         * @brief Destroy the Index object and mutex
         * 
         */
        ~Index();
    };
};