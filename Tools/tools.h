#pragma once

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "sha256.h"
#include "base64.h"

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
    /**
     * @brief send a message (terminated with \\n) to a socket
     * 
     * @param socket socket file descriptor to send to
     * @param msg message to send (must end with \\n)
     */
    void sendMessage(int socket, const std::string& msg);
    /**
     * @brief receive a message terminated with a \\n
     * 
     * @param socket socket file descriptor to receive from
     * @return string containing the message (if string == "" -> connection closed)
     */
    std::string receiveMessage(int socket);
};