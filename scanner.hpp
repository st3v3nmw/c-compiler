#ifndef SCANNER_H
#define SCANNER_H
#ifdef _WIN32
    #include "windows.h"
    #define DELAY Sleep
#else
    #include "unistd.h"
    #define DELAY usleep
#endif

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

#endif