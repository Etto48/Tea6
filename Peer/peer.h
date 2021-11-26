#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include <tuple>
#include <signal.h>

#include "../Database/database.h"
#include "../Parser/parser.h"
#include "../Tools/tools.h"

namespace Peer
{
    /**
     * @brief NEVER initialize this class in the stack or the "this" pointer will be lost
     * 
     */
    class Peer
    {
    protected:
        pthread_t id;
        Database::DataContainer<std::string> userInfo;
        std::pair<int,sockaddr_in6> server;
        std::pair<int,sockaddr_in6> peerServer;
        std::vector<std::pair<int,sockaddr_in6>> connectedPeers;
        bool shutdownRequested = false;
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
         * @param serverPort server TCP port
         * @param peerPort peer TCP port
         */
        Peer(const std::string& username, const std::string& password, const std::string& indexIp, uint16_t serverPort, uint16_t peerPort);
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
