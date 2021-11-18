#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>

namespace Log
{
    class Log
    {
    protected:
        std::fstream out;
        bool doEcho;

    public:
        Log(const std::string &logfile, bool doEcho) : out(logfile, std::ios::app), doEcho(doEcho) {}

        template <class T>
        Log &operator<<(T obj)
        {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            auto time = std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

            out << time << "\t" << obj;
            if (doEcho)
                std::cout << time << "\t" << obj;

            return *this;
        }
    };

    extern Log log;
    extern Log error;
    extern Log debug;
};