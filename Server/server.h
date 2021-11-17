#pragma once
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>

namespace Server
{
    class Connection
    {
    protected:
        pid_t pid;
        int clientSocket;
        sockaddr_in6 clientAddr;
        int run();
        void die(int signum);
    public:
        /**
         * @brief Construct a new Connection object
         *
         * @param clientSocket socket fd of the client
         * @param clientAddr sockaddr struct of the client
         */
        Connection(int clientSocket,const sockaddr_in6& clientAddr):clientSocket(clientSocket),clientAddr(clientAddr),pid(0){}
        /**
         * @brief starts the connection in a different process
         * 
         */
        void start();
        /**
         * @brief forces the termination of the connection
         * 
         */
        void close();
        /**
         * @brief Destroy the Connection object and wait for the process to close
         * 
         */
        ~Connection();
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