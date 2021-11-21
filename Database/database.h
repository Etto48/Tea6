#pragma once

#include <string>
#include <list>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <pthread.h>
#include <arpa/inet.h>

#include "../Log/log.h"

namespace Database
{
    template <class T>
    struct DataContainer
    {
        std::string key;
        T value;
        DataContainer(const std::string &key, const T &value) : key(key), value(value) {}
    };

    /**
     * @brief store/search a key and a value of type T
     *
     * @tparam T type of the value
     */
    template <class T>
    class Database
    {
    protected:
        std::vector<std::list<DataContainer<T>>> hashTable;
        pthread_mutex_t accessMutex = PTHREAD_MUTEX_INITIALIZER;
        std::string name;
        /**
         * @brief calculate the index based on the key
         *
         * @param key key from which we derive the index
         * @return index for the hashTable
         */
        size_t hash(const std::string &key);
        /**
         * @brief identical to find(const std::string& key) but doesn't lock the mutex, used only inside another mutex
         *
         * @param key key to find
         * @return const pointer to the data, nullptr if not found
         */
        const DataContainer<T> *findNoLock(const std::string key);

    public:
        /**
         * @brief Construct a new Database object
         *
         * @param dbName name for database debug
         * @param hashTableSize the bigger the faster (uses more ram)
         */
        Database(const std::string &dbName = "database", size_t hashTableSize = 256) : name(dbName)
        {
            hashTable.resize(hashTableSize);
        }

        /**
         * @brief add a new entry to the database
         *
         * @param data new entry
         * @return true if success, false if error (key already present)
         */
        bool operator+=(const DataContainer<T> &data);
        /**
         * @brief remove an item from the database
         *
         * @param key key to remove
         * @return true if success, false if error (key not found)
         */
        bool operator-=(const std::string &key);
        /**
         * @brief finds an entry in the database
         *
         * @param key key to find
         * @return const pointer to the data, nullptr if not found
         */
        const DataContainer<T> *find(const std::string &key);
    };

    namespace Tables
    {
        extern Database<std::string> passwords;
        extern Database<in6_addr> online;
    };
};

#include "databaseTemplateImplementation.h"