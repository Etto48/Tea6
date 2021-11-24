#pragma once
#include <stdlib.h>
#include <string>

#define HOME std::string(getenv("HOME"))
#define TEA6ROOT (std::string(getenv("HOME"))+"/.Tea6/")

#define INDEX_PORT 4823

#ifndef VERSION
    #define VERSION "v0.0.0"
#endif