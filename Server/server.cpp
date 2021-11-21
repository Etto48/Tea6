#include "server.h"

namespace Server
{
    // Connection
    void *Connection::run(void *me)
    {
        Connection *t = reinterpret_cast<Connection *>(me);
        int nbytes;
        char buf[1024];

        auto ipv6Str = Tools::ipv6ToString(t->clientAddr.sin6_addr);

        Log::event << Log::time() << " " << ipv6Str << "(" << t->id << ") Connection opened.\n";
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
                        buf[i] = 0;
                        break;
                    }
                }
                msg += std::string(buf);
            } while (nbytes > 0 && !endmsg);
            if (nbytes > 0)
            {
                Log::debug << Log::time() << " " << ipv6Str << "(" << t->id << ") Received: " << msg << "\n";
                // parse the message
                Parser::ParsedMessage pm{msg};
                std::string response = "";
                switch (pm.command)
                {
                case Parser::ADD:
                    if (!(Index::registeredUsers += Index::UserData(pm.args[0], t->clientAddr.sin6_addr)))
                    { // error
                        response = Parser::errorMessages::AlreadyPresent;
                    }
                    else
                    { // success
                        response = msg + "\n";
                    }
                    break;
                case Parser::QUERY:
                {
                    auto ud = Index::registeredUsers.find(pm.args[0]);
                    if (!ud)
                    {
                        response = Parser::errorMessages::NotPresent;
                    }
                    else
                    {
                        // add address to response
                        response = std::string("q") + ud->username + " " + Tools::ipv6ToString(ud->addr) + "\n";
                    }
                }
                break;
                case Parser::ERROR:
                    response = Parser::errorMessages::Protocol;
                    break;
                }
                Log::debug << Log::time() << " " << ipv6Str << "(" << t->id << ") Sent: " << response;
                send(t->clientSocket, response.c_str(), response.length(), 0);
            }
        } while (nbytes > 0);
        Log::event << Log::time() << " " << ipv6Str << "(" << t->id << ") Connection closed.\n";
        return nullptr;
    }
    void Connection::die()
    {
        shutdown(clientSocket, SHUT_RDWR);
        ::close(clientSocket);
    }

    // Server
    Server::Server(uint16_t port)
    {
        serverSocket = socket(AF_INET6, SOCK_STREAM, 0);
        if (serverSocket < 0)
        {
            perror("Error creating socket");
            pthread_exit(nullptr);
        }
        int yes = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
        {
            perror("Erro setting socket reuseaddr to true");
            pthread_exit(nullptr);
        }
        bzero(&serverAddr, sizeof(sockaddr_in6));
        serverAddr.sin6_family = AF_INET6;
        serverAddr.sin6_port = htons(port);
        serverAddr.sin6_flowinfo = 0;
        serverAddr.sin6_scope_id = 0x20; // link - local
        serverAddr.sin6_addr = {0};
        if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(sockaddr_in6)) < 0)
        {
            perror("Error binding socket");
            pthread_exit(nullptr);
        }
        if (listen(serverSocket, 100) < 0)
        {
            perror("Error listening on socket");
            pthread_exit(nullptr);
        }
        if (pthread_create(&id, nullptr, run, (void *)this) < 0)
        {
            perror("Error starting thread");
            pthread_exit(nullptr);
        }
    }
    void *Server::run(void *me)
    {
        Server *t = reinterpret_cast<Server *>(me);
        Log::event << Log::time() << " (" << t->id << ") Server started.\n";
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
                    {
                        perror("Error during poll");
                        pthread_exit(nullptr);
                    }
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
        for (auto &c : connList)
        { // close every open connection
            c.close();
            c.join();
        }
        shutdown(serverSocket, SHUT_RDWR);
        ::close(serverSocket);
        Log::event << Log::time() << " (" << id << ") Server closed.\n";
        pthread_exit(nullptr);
    }

};