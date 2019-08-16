
#ifndef INTERFACE_H
#define	INTERFACE_H

#include <string>

using namespace std;

class Interface {
    public:
        static constexpr const char* MPI = "MPI";
        static constexpr const char* WEBAPPLICATION = "WEBAPPLICATION";
        static constexpr const char* SEQUENTIAL = "SEQUENTIAL";
};

#endif