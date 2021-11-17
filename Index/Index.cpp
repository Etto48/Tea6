#include "Index.h"

namespace Index
{
    //UserData
    bool UserData::operator==(const UserData& x)
    {
        return this->username==x.username;
    }

    //_IndexList
    bool _IndexList::add(const UserData& entry)
    {
        if(this->find(entry.username))
        {
            ls.emplace_back(entry);   
            return true;
        }
        else return false;
    }
    bool _IndexList::remove(const std::string& username)
    {
        auto p = this->find(username);
        if(!p)
        {
            ls.remove(*p);
            return true;
        }
        else return false;
    }
    UserData* _IndexList::find(const std::string& username)
    {
        for(auto& x: ls)
        {
            if(x.username==username)
                return &x;
        }
        return nullptr;
    }

    //Index
    Index::Index()
    {
        this->indexMutex = PTHREAD_MUTEX_INITIALIZER;
    }
    uint8_t Index::hashIndex(const std::string& username)
    {
        uint8_t ret = 0;
        for(auto& c: username)
            ret+=reinterpret_cast<const uint8_t&>(c);
        return ret;
    }
    bool Index::operator+=(const UserData& entry)
    {
        pthread_mutex_lock(&indexMutex);
        auto ret = hashTable[hashIndex(entry.username)].add(entry);
        pthread_mutex_unlock(&indexMutex);
        return ret;
    }
    bool Index::operator-=(const std::string& username)
    {
        pthread_mutex_lock(&indexMutex);
        auto ret = hashTable[hashIndex(username)].remove(username);
        pthread_mutex_unlock(&indexMutex);
        return ret;
    }
    UserData* Index::find(const std::string& username)
    {
        pthread_mutex_lock(&indexMutex);
        auto ret = hashTable[hashIndex(username)].find(username);
        pthread_mutex_unlock(&indexMutex);
        return ret;
    }
    Index::~Index()
    {
        pthread_mutex_destroy(&indexMutex);
    }
};