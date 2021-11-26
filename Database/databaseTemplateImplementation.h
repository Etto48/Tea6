#pragma once

#include "database.h"

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const in6_addr& ipv6);
std::basic_istream<char>& operator>>(std::basic_istream<char>& is, in6_addr& ipv6);

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
    bool Database<T>::addNoLock(const DataContainer<T> &data)
    {
        auto &l = hashTable[hash(data.key)];
        auto p = this->findNoLock(data.key);
        if (!p)
            l.push_back(data);
        return !p;
    }
    template <class T>
    bool Database<T>::removeNoLock(const std::string &key)
    {
        auto &l = hashTable[hash(key)];
        auto p = this->findNoLock(key);
        if (p)
            l.remove(*p);
        return p;
    }
    template <class T>
    Database<T> Database<T>::fromFile(const std::string &path, const std::string &dbName, size_t hashTableSize)
    {
        Database<T> ret("fromFile",hashTableSize);
        if (dbName == "")
            ret.name = path;
        else
            ret.name = dbName;
        ret.load(path);
        return ret;
    }
    template <class T>
    bool Database<T>::load(const std::string &path)
    {
        pthread_mutex_lock(&accessMutex);
        bool fail = false;
        std::vector<std::list<DataContainer<T>>> oldData;
        {//create the file if missing
            std::fstream fcreate(path, std::ios::out | std::ios::app);
        }
        std::fstream reader(path, std::ios::in);
        if (reader.fail())
        {
            fail = true;
        }
        else
        {
            oldData = this->hashTable;
            hashTable = std::vector<std::list<DataContainer<T>>>();
            hashTable.resize(oldData.size());
        }
        size_t count = 0;

        std::string line;

        while (std::getline(reader, line)  && !fail)
        {
            std::stringstream ss(line);
            std::string newKey;
            T newValue;

            
            ss >> newKey;
            if (ss.bad())
            {
                fail = true;
                break;
            }
            ss >> newValue;
            if (ss.bad())
            {
                fail = true;
                break;
            }

            this->addNoLock(DataContainer<T>{newKey, newValue});
            count++;
        }
        if (fail)
            hashTable = oldData;
        else
            this->path = path;
        reader.close();
        pthread_mutex_unlock(&accessMutex);
        Log::debug << Log::time() << " Db(" << name << ") LOAD from " << path;
        if (fail)
        {
            Log::debug << " (FAIL)\n";
            return false;
        }
        else
        {
            Log::debug << ", " << count << " entries loaded\n";
            return true;
        }
    }
    template <class T>
    bool Database<T>::save(const std::string &path)
    {
        pthread_mutex_lock(&accessMutex);
        bool fail = false;
        std::fstream writer;
        if (path == "" && this->path == "")
            fail = true;
        else
        {
            writer.open(path == "" ? this->path : path, std::ios::out);
            if (writer.fail())
                fail = true;
        }
        size_t count = 0;
        if (!fail)
        {
            for (auto &l : hashTable)
            {
                for (auto &e : l)
                {
                    writer << e.key << " " << e.value << "\n";
                    count++;
                }
            }
        }

        if (path != "" && !fail)
            this->path = path;
        writer.close();
        pthread_mutex_unlock(&accessMutex);

        Log::debug << Log::time() << " Db(" << name << ") SAVE to " << this->path;
        if (fail)
        {
            Log::debug << " (FAIL)\n";
            return false;
        }
        else
        {
            Log::debug << ", " << count << " entries saved\n";
            return true;
        }
    }

    template <class T>
    bool Database<T>::operator+=(const DataContainer<T> &data)
    {
        pthread_mutex_lock(&accessMutex);
        auto ret = this->addNoLock(data);
        pthread_mutex_unlock(&accessMutex);
        Log::debug << Log::time() << " "
                   << "Db(" << name << "): ADD \"" << data.key << "\" : \"" << data.value << "\"";
        if (ret)
        {
            Log::debug << "\n";
        }
        else
        {
            Log::debug << " (FAIL)\n";
        }
        return ret;
    }

    template <class T>
    bool Database<T>::operator-=(const std::string &key)
    {
        pthread_mutex_lock(&accessMutex);
        auto ret = this->removeNoLock(key);
        pthread_mutex_unlock(&accessMutex);
        Log::debug << Log::time() << " "
                   << "Db(" << name << "): REMOVE \"" << key << "\"";
        if (ret)
        {
            Log::debug << "\n";
        }
        else
        {
            Log::debug << " (FAIL)\n";
        }
        return ret;
    }

    template <class T>
    const DataContainer<T> *Database<T>::find(const std::string &key)
    {
        pthread_mutex_lock(&accessMutex);
        auto f = this->findNoLock(key);
        pthread_mutex_unlock(&accessMutex);
        Log::debug << Log::time() << " "
                   << "Db(" << name << "): FIND \"" << key << "\"";
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