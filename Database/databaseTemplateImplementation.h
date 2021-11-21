#pragma once

#include "database.h"

namespace Database
{
    template <class T>
    size_t Database<T>::hash(const std::string &key)
    {
        size_t ret = 0;
        for (auto &c : key)
        {
            ret += reinterpret_cast<const uint8_t &>(c);
            ret %= hashTable.size();
        }
        return ret;
    }
    template <class T>
    const DataContainer<T> *Database<T>::findNoLock(const std::string key)
    {
        auto &l = hashTable[hash(key)];
        for (auto &e : l)
        {
            if (e.key == key)
            {
                return &e;
            }
        }
        return nullptr;
    }

    template <class T>
    bool Database<T>::operator+=(const DataContainer<T> &data)
    {
        pthread_mutex_lock(&accessMutex);
        auto &l = hashTable[hash(data.key)];
        auto p = this->findNoLock(data.key);
        if (!p)
            l.push_back(data);
        pthread_mutex_unlock(&accessMutex);
        Log::debug << Log::time() << " "
                   << "Db(" << name << "): ADD " << data.key;
        if (!p)
        {
            Log::debug << "\n";
        }
        else
        {
            Log::debug << " (FAIL)\n";
        }
        return !p;
    }

    template <class T>
    bool Database<T>::operator-=(const std::string &key)
    {
        pthread_mutex_lock(&accessMutex);
        auto &l = hashTable[hash(key)];
        auto p = this->findNoLock(key);
        if (p)
            l.remove(*p);
        pthread_mutex_unlock(&accessMutex);
        Log::debug << Log::time() << " "
                   << "Db(" << name << "): REMOVE " << key;
        if (p)
        {
            Log::debug << "\n";
        }
        else
        {
            Log::debug << " (FAIL)\n";
        }
        return p;
    }

    template <class T>
    const DataContainer<T> *Database<T>::find(const std::string &key)
    {
        pthread_mutex_lock(&accessMutex);
        auto f = this->findNoLock(key);
        pthread_mutex_unlock(&accessMutex);
        Log::debug << Log::time() << " "
                   << "Index: FIND " << key;
        if (f)
        {
            Log::debug << "\n";
        }
        else
        {
            Log::debug << " (FAIL)\n";
        }
        return f;
    }
};