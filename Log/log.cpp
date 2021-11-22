#include "log.h"

#ifdef _DEBUG
constexpr bool doEcho = true;
#else
constexpr bool doEcho = false;
#endif

namespace Log
{
    Log event{TEA6ROOT + "event.log", doEcho, 32};
    Log error{TEA6ROOT + "error.log", doEcho, 31};
    #ifdef _DEBUG
    Log debug{TEA6ROOT + "debug.log", doEcho, 33};
    #else
    Log debug{"/dev/null", doEcho, 33};
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