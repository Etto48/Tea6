#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <pthread.h>

#include "../Database/database.h"
#include "../Tools/tools.h"

namespace Peer
{
    class Peer
    {
    protected:
        pthread_t id;
        Database::DataContainer<std::string> userInfo;
        /**
         * @brief thread code
         * 
         * @param me pointer to class
         * @return thread return
         */
        static void* run(void* me);
        /**
         * @brief close the connection, should never be called
         * 
         */
        void die();
    public:
        /**
         * @brief Construct a new Peer object
         * 
         * @param username key to register in the index
         * @param password password for autentication
         * @param indexIp IPv6 of the index server
         * @param port server TCP port
         */
        Peer(const std::string& username, const std::string& password, const std::string& indexIp, uint16_t port):
            userInfo(username,Tools::sha256(password)),id(0)
        {
            if(pthread_create(&id,nullptr,run,(void*)this))
                perror("Could not create thread");
        }
        /**
         * @brief join thread
         * 
         * @return thread return
         */
        void* join()
        {
            void* ret;
            pthread_join(id,&ret);
            return ret;
        }
        /**
         * @brief function to kill the connection from another thread
         * 
         */
        void close(){die();}
    };
};
