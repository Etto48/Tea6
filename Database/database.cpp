#include "database.h"

namespace Database
{
    namespace Tables
    {
        Database<std::string> passwords("passwords");
        Database<in6_addr> online("online");
    };
};