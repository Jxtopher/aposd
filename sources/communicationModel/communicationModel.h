
#ifndef COMMUNICATIONMODEL_H
#define	COMMUNICATIONMODEL_H

#include <string>

using namespace std;

class CommunicationModel {
    public:
        static constexpr const char* MPI = "MPI";
        static constexpr const char* WEBAPPLICATION = "WEBAPPLICATION";
        static constexpr const char* SEQUENTIAL = "SEQUENTIAL";
};

#endif