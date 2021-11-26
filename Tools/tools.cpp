#include "tools.h"

namespace Tools
{
    // tools
    std::string ipv6ToString(in6_addr ip)
    {
        char addrbuf[1024];
        bzero(addrbuf, 1024);
        inet_ntop(AF_INET6, (void *)&ip, addrbuf, 1024);
        std::string ret{addrbuf};
        return ret;
    }
    in6_addr stringToIpv6(const std::string &ip)
    {
        in6_addr ret;
        inet_pton(AF_INET6, ip.c_str(), &ret);
        return ret;
    }

    void sendMessage(int socket, const std::string &msg)
    {
        bool done = false;
        int lastSent = 0;
        int totalSent = 0;
        while (!done)
        {
            lastSent = send(socket, msg.c_str() + totalSent, msg.length() - totalSent, 0);
            totalSent += lastSent;
            if (lastSent < 0)
                perror("Error in send");
            else if (totalSent == msg.length())
                done = true;
        }
    }
    std::string receiveMessage(int socket)
    {
        std::string ret = "";
        char buf[1024];
        size_t sizeRead = 0;
        bool endmsg = false;
        do
        {
            bzero(buf, 1024);
            sizeRead = recv(socket, buf, 1023, 0);
            for (size_t i = 0; i < sizeRead; i++)
            {
                if (buf[i] == '\n')
                {
                    buf[i] = 0;
                    endmsg = true;
                    break;
                }
            }
            ret += std::string(buf);
        } while (sizeRead > 0 && !endmsg);
        if (sizeRead > 0)
            ret += '\n';
        else
            ret = "";
        return ret;
    }
};