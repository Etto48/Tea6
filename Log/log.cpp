#include "log.h"

#define HOME std::string(getenv("HOME"))

namespace Log
{
    Log log{HOME+"/.Tea6/event.log",false};
    Log error{HOME+"/.Tea6/error.log",false};
    Log debug{HOME+"/.Tea6/debug.log",true};
};