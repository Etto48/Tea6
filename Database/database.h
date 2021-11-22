#pragma once

#include <string>
#include <list>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <pthread.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>

#include "../Log/log.h"
#include "../Tools/tools.h"
#include "../globalDefine.h"

namespace Database
{
    template <class T>
    struct DataContainer
    {
        std::string key;
        T value;
        DataContainer(const std::string &key, const T &value) : key(key), value(value) {}
        bool operator==(const DataContainer<T> &dc) { return key == dc.key; }
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
        std::string path = "";
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
        /**
         * @brief identical to operator+= but doesn't lock the mutex, used only inside another mutex
         *
         * @param data new entry
         * @return true if success, false if error (key already present)
         */
        bool addNoLock(const DataContainer<T> &data);
        /**
         * @brief identical to operator-= but doesn't lock the mutex, used only inside another mutex
         *
         * @param key key to remove
         * @return true if success, false if error (key not found)
         */
        bool removeNoLock(const std::string &key);
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
         * @brief load a database from a file and link it to that
         *
         * @param path path to the file
         * @param hashTableSize the bitter the faster (uses more ram)
         * @return database linked to a file (saved automatically)
         */
        Database<T> static fromFile(const std::string &path, const std::string &dbName = "", size_t hashTableSize = 256);
        /**
         * @brief load a database from a file and link it to that, if operation failed the data will be restored to a previous state
         * if the operation succeeded the old data will be lost
         *
         * @param path path to the file
         * @return true if operation succeeded, false if failed
         */
        bool load(const std::string &path);
        /**
         * @brief update the file the database is linked to or change the linking, this function is also called in the destructor
         *
         * @param path if left out just update the file, if set relink the database
         * @return true if operation succeeded, false if not
         */
        bool save(const std::string &path = "");
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
        /**
         * @brief get the path where the db is linked
         * 
         * @return path string, if it's "" then the database is not linked
         */
        const std::string& getPath(){return this->path;}
        /**
         * @brief Destroy the Database object and save it if it's linked to a file
         *
         */
        ~Database() { if(this->path!="")save(); }
    };

    namespace Tables
    {
        extern Database<std::string> passwords;
        extern Database<in6_addr> online;
    };
};

#include "databaseTemplateImplementation.h"