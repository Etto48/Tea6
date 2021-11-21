#include "tools.h"

namespace Tools
{
    // tools
    std::string ipv6ToString(in6_addr ip)
    {
        char addrbuf[1024];
        bzero(addrbuf,1024);
        inet_ntop(AF_INET6, (void *)&ip, addrbuf, 1024);
        std::string ret{addrbuf};
        return ret;
    }
    in6_addr stringToIpv6(const std::string &ip)
    {
        in6_addr ret;
        inet_pton(AF_INET6,ip.c_str(),&ret);
        return ret;
    }
};