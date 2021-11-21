#include "log.h"

#define HOME std::string(getenv("HOME"))

#ifdef _DEBUG
constexpr bool doEcho = true;
#else
constexpr bool doEcho = false;
#endif

namespace Log
{
    Log event{HOME + "/.Tea6/event.log", doEcho, 32};
    Log error{HOME + "/.Tea6/error.log", doEcho, 31};
    #ifdef _DEBUG
    Log debug{HOME + "/.Tea6/debug.log", doEcho};
    #else
    Log debug{"/dev/null", doEcho};
    #endif

    std::string time(std::string fmt)
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        auto time = std::put_time(std::localtime(&in_time_t), fmt.c_str());
        std::stringstream ss;
        ss << time;
        return ss.str();
    }
};