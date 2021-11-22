#include "peer.h"

#define PEER_BACKLOG 100

namespace Peer
{
    void *Peer::run(void *me)
    {
        Peer* t = (Peer *)me;
        while(!t->shutdownRequested);
        
        return nullptr;
    }
    void Peer::die()
    {
        for(auto& x:connectedPeers)
        {
            shutdown(x.first,SHUT_RDWR);
            ::close(x.first);
        }
        shutdown(server.first,SHUT_RDWR);
        ::close(server.first);
        shutdown(peerServer.first,SHUT_RDWR);
        ::close(peerServer.first);
        shutdownRequested = true;
    }
    Peer::Peer(const std::string &username, const std::string &password, const std::string &indexIp, uint16_t serverPort, uint16_t peerPort) : userInfo(username, Tools::sha256(password)), id(0)
    {
        server.second.sin6_family = AF_INET6;
        server.second.sin6_addr = Tools::stringToIpv6(indexIp);
        server.second.sin6_flowinfo = 0;
        server.second.sin6_scope_id = htons(serverPort);
        server.second.sin6_scope_id = 0x20;

        peerServer.second.sin6_family = AF_INET6;
        peerServer.second.sin6_addr = {0};
        peerServer.second.sin6_flowinfo = 0;
        peerServer.second.sin6_scope_id = htons(peerPort);
        peerServer.second.sin6_scope_id = 0x20;

        server.first = socket(AF_INET6, SOCK_STREAM, 0);
        if (server.first < 0)
            perror("Error creating socket");

        peerServer.first = socket(AF_INET6, SOCK_STREAM, 0);
        if (peerServer.first < 0)
            perror("Error creating socket");
        int yes = 1;
        if (setsockopt(peerServer.first, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, 1))
            perror("Error in setsockopt");

        if (connect(server.first, (sockaddr *)&server.second, sizeof(server.second)))
            perror("Error connecting to server");

        if (bind(peerServer.first, (sockaddr *)&server.second, sizeof(server.second)))
            perror("Error binding peerServer socket");

        if (listen(peerServer.first, PEER_BACKLOG))
            perror("Error listening on socket");

        if (pthread_create(&id, nullptr, run, (void *)this))
            perror("Could not create thread");
    }
};