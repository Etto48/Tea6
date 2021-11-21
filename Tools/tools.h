#pragma once

#include <string>
#include <arpa/inet.h>
#include <string.h>

namespace Tools
{
    /**
     * @brief converts an ipv6 in a readable format
     *
     * @param ip ipv6
     * @return string with readable ipv6
     */
    std::string ipv6ToString(in6_addr ip);
    /**
     * @brief converts readable ipv6 in a struct
     *
     * @param ip string with ipv6
     * @return struct used for connection and data storage
     */
    in6_addr stringToIpv6(const std::string &ip);
};