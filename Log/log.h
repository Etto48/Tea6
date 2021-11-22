#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <pthread.h>
#include <stdarg.h>
#include <string.h>
#include <sstream>
#include "../globalDefine.h"

namespace Log
{
    /**
     * @brief use it to get the current time in a printable format
     *
     * @param fmt format to use, %Y is year %m is month %d is day %H is hours %M is minutes %S is seconds
     * @return string containing time in required format
     */
    std::string time(std::string fmt = "%Y-%m-%d %H:%M:%S");

    class Log
    {
    protected:
        std::fstream out;
        bool doEcho;
        pthread_mutex_t logMutex = PTHREAD_MUTEX_INITIALIZER;
        unsigned int color;
    public:
        Log(const std::string &logfile, bool doEcho, unsigned int color = 0) : out(logfile, std::ios::app), doEcho(doEcho), color(color) {}
        /**
         * @brief use this to log an object to a logfile, you should use it like this: LOG << Log::time() << ... << '\n';
         *
         * @tparam T type of object to log
         * @param obj object to log
         * @return reference to the logger
         */
        template <class T>
        Log &operator<<(T obj)

        {
            pthread_mutex_lock(&logMutex);
            out << obj;
            if (doEcho)
                std::cout << "\e[" << color << "m" << obj << "\e[0m";
            pthread_mutex_unlock(&logMutex);
            return *this;
        }
    };

    /**
     * @brief logs common events
     *
     */
    extern Log event;
    /**
     * @brief logs errors
     *
     */
    extern Log error;
    /**
     * @brief logs debug info
     *
     */
    extern Log debug;
};