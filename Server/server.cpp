#include "server.h"

namespace Server
{
    // Connection
    void *Connection::run(void *me)
    {
        Connection *t = reinterpret_cast<Connection *>(me);
        int nbytes;
        char buf[1024];
        char addrbuf[1024];
        inet_ntop(AF_INET6, (void *)&t->clientAddr.sin6_addr, addrbuf, 1024);
        do
        {
            std::string msg = "";
            bool endmsg = false;
            do
            {
                bzero(buf, 1024);
                nbytes = recv(t->clientSocket, buf, 1023, 0);
                for (int i = 0; i < nbytes; i++)
                {
                    if (buf[i] == '\n') // endmsg
                    {
                        endmsg = true;
                        break;
                    }
                }
                msg += std::string(buf);
            } while (nbytes > 0 && !endmsg);
            if (nbytes > 0)
            {
                std::cout << std::string(addrbuf) << "(" << t->id << ") Received: " << msg;
                // to @Etto48 here we should execute a parser to run a command and respond corresponding to the message
            }
        } while (nbytes > 0);
        if(!t->shutdownRequested)
            std::cout << std::string(addrbuf) << "(" << t->id << ") Connection closed.\n";
        return nullptr;
    }
    void Connection::die()
    {
        shutdownRequested = true;
        shutdown(clientSocket, SHUT_RDWR);
        ::close(clientSocket);
    }

    // Server
    Server::Server(uint16_t port)
    {
        serverSocket = socket(AF_INET6, SOCK_STREAM, 0);
        bzero(&serverAddr, sizeof(sockaddr_in6));
        serverAddr.sin6_family = AF_INET6;
        serverAddr.sin6_port = htons(port);
        serverAddr.sin6_flowinfo = 0;
        serverAddr.sin6_scope_id = 0x20; // link - local
        serverAddr.sin6_addr = {0};
        if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(sockaddr_in6)) < 0)
            perror("Error binding socket");
        if (listen(serverSocket, 100) < 0)
            perror("Error listening on socket");
        if (pthread_create(&id, nullptr, run, (void *)this) < 0)
            perror("Error starting thread");
    }
    void *Server::run(void *me)
    {
        Server *t = reinterpret_cast<Server *>(me);
        while (true)
        {
            sockaddr_in6 clientAddr;
            bzero(&clientAddr, sizeof(sockaddr_in6));
            socklen_t caSize = sizeof(sockaddr_in6);
            bool clientAvailable = false;
            do
            {
                if (t->shutdownRequested)
                    t->die();
                else
                {
                    pollfd pfd;
                    pfd.events = POLLIN;
                    pfd.revents = 0;
                    pfd.fd = t->serverSocket;
                    auto polled = poll(&pfd, 1, 100);
                    if (polled < 0)
                        perror("Error during poll");
                    else if (polled > 0)
                        clientAvailable = true;
                }
            } while (!clientAvailable);
            int clientSocket = accept(t->serverSocket, (sockaddr *)&clientAddr, &caSize);
            t->connList.push_back(Connection(clientSocket, clientAddr));
        }
        while (!t->connList.empty())
        {
            t->connList.begin()->join();
            t->connList.pop_front();
        }
    }
    void Server::die()
    {
        for (auto &c: connList)
        { // close every open connection
            c.close();
        }
        shutdown(serverSocket, SHUT_RDWR);
        ::close(serverSocket);
        for (auto &c: connList)
        { // close every open connection
            c.join();
        }
        std::cout << "(" << id << ") Server closed.\n";
        pthread_exit(nullptr);
    }
};