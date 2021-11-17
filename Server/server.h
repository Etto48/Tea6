#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
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
         * @param csock clientSocket
         * @param caddr clientAddr
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
         * @brief Destroy the Connection object and join thread
         * 
         */
        ~Connection()
        {
            void* ret;
            pthread_join(id,&ret);
        }
    };
    class Server
    {
    protected:
        int serverSocket;
        sockaddr_in6 serverAddr;
    public:
        Server(uint16_t port);
        
    };
};