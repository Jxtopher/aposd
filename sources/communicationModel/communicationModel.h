
#ifndef COMMUNICATIONMODEL_H
#define	COMMUNICATIONMODEL_H

#include <string>

using namespace std;

class CommunicationModel {
    public:
        static const int MPI = 0;
        static const int WEBAPPLICATION = 1;
        static const int SEQUENTIAL = 2;

        static constexpr const char* STR_MPI = "MPI";
        static constexpr const char* STR_WEBAPPLICATION = "WEBAPPLICATION";
        static constexpr const char* STR_SEQUENTIAL = "SEQUENTIAL";
};

#endif