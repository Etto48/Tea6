#include "database.h"

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const in6_addr& ipv6)
{
    os<<Tools::ipv6ToString(ipv6);
    return os;
}
std::basic_istream<char>& operator>>(std::basic_istream<char>& is, in6_addr& ipv6)
{
    std::string tmp;
    is>>tmp;
    ipv6 = Tools::stringToIpv6(tmp);
    return is;
}

namespace Database
{
    namespace Tables
    {
        Database<std::string> passwords("passwords");
        Database<in6_addr> online("online");
    };
};