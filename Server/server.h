#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <list>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

namespace Server
{
    class Connection
    {
    protected:
        int clientSocket;
        sockaddr_in6 clientAddr;
        pthread_t id;
        /**
         * @brief thread code
         * 
         * @param me pointer to class
         * @return thread return
         */
        static void* run(void* me);
    public:
        /**
         * @brief Construct a new Connection object
         *
         * @param clientSocket socket fd of the client
         * @param clientAddr sockaddr struct of the client
         */
        Connection(int clientSocket,const sockaddr_in6& clientAddr):clientSocket(clientSocket),clientAddr(clientAddr),id(0){
            if(pthread_create(&id,nullptr,run,(void*)this)<0)
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
    };
    class Server
    {
    protected:
        pthread_t id;
        int serverSocket;
        sockaddr_in6 serverAddr;
        std::list<Connection> connList;
        /**
         * @brief thread code
         * 
         * @param me pointer to class
         * @return thread return
         */
        static void* run(void* me);
    public:
        /**
         * @brief Construct a new Server object
         * 
         * @param port server tcp port
         */
        Server(uint16_t port);
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
    };
};