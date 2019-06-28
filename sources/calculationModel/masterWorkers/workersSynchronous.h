#ifndef WORKERS_SYNCHRONOUS_H
#define WORKERS_SYNCHRONOUS_H

#include <mpi.h>
#include <regex>

#include "workers.h"

class WorkersSynchronous : public Workers {
   public:
    WorkersSynchronous(Launcher &launcher) : _launcher(launcher) {}
    virtual ~WorkersSynchronous() {}

    void operator()() {
        DEBUG_TRACE("Workers")

        int continued = true;
        int order;

        while (continued) {
            MPI_Recv(&order, 1, MPI_INT, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
            switch (order) {
                case MPI_Order::INIT_SOLUTION: {
                    string s = _launcher.initSolution();
                    MPI_Isend((char *)s.c_str(), s.size(), MPI_CHAR, MPI_MASTER, MPI_TAG, MPI_COMM_WORLD, &request);
                } break;
                case MPI_Order::COMPUTE_FITNESS: {
                    // Recv
                    int sizeOfmessage;  // Get size of string solution
                    int parameter;  // Get parameter

                    MPI_Probe(MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);
                    MPI_Get_count(&status, MPI_CHAR, &sizeOfmessage);

                    char *msg = new char[sizeOfmessage + 1];  // Get solution
                    MPI_Recv(msg, sizeOfmessage, MPI_CHAR, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);

                    MPI_Recv(&parameter, 1, MPI_INT, MPI_ANY_SOURCE, MPI_TAG, MPI_COMM_WORLD, &status);

                    // Call solver
                    string s = _launcher.solve(msg, parameter);
                    delete msg;

                    // Send message
                    MPI_Isend((char *)s.c_str(), s.size(), MPI_CHAR, MPI_MASTER, MPI_TAG, MPI_COMM_WORLD, &request);
                } break;
                case MPI_Order::FINISH:
                    continued = false;
                    break;
                default:
                    break;
            }
        }
    }

   protected:
    Launcher &_launcher;
    MPI_Status status;
    MPI_Request request;
};

#endif