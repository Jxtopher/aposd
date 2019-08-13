#ifndef JXTOPHER_MASTERSYNCHRONOUS_H
#define JXTOPHER_MASTERSYNCHRONOUS_H

#include <memory>

#include "master.h"
#include "../../selection/selection.h"

using namespace std;

template <class SOL>
class MasterSynchronous : public Master {
   public:
    MasterSynchronous(unique_ptr<ParameterSelection> parameterSelection, 
                        unique_ptr<RewardComputation<SOL>> rewardComputation,
                        unique_ptr<Selection<SOL>> selection)
        : Master(), 
        _parameterSelection(std::move(parameterSelection)), 
        _rewardComputation(std::move(rewardComputation)),
        _selection(std::move(selection)) {}

    virtual ~MasterSynchronous() {}

    virtual void operator()() {
        DEBUG_TRACE("MASTER")

        _parameterSelection->reset();
        vector<SOL> solutions(mpi_globals_nbnodes - 1);
        vector<SOL> solutionsAfterMutation(mpi_globals_nbnodes - 1);

        int order = MPI_Order::INIT_SOLUTION;
        for (int i = 1; i < mpi_globals_nbnodes; i++)
            MPI_Isend(&order, 1, MPI_INT, i, MPI_TAG, MPI_COMM_WORLD, &request);

        for (int i = 1; i < mpi_globals_nbnodes; i++) {
            int sizeOfmessage;
            MPI_Probe(i, MPI_TAG, MPI_COMM_WORLD, &status);     // It will block the caller until a message is ready
            MPI_Get_count(&status, MPI_CHAR, &sizeOfmessage);   // Obtain message size
            char *msg = new char[sizeOfmessage + 1];
            MPI_Recv(msg, sizeOfmessage, MPI_CHAR, i, MPI_TAG, MPI_COMM_WORLD, &status); // Finally, receive the message with a correctly sized buffe
            solutions[i - 1] = SOL(string(msg));
            cout << "[MASTER] " << msg << endl;
            delete msg;
        }

        do {
                // Selection parameter a execute
                vector<unsigned int> parameter = _parameterSelection->getParameter(mpi_globals_nbnodes - 1);
                SOL bestSolution = _selection->operator()(solutions);

                // ------------------------------------------
                order = MPI_Order::COMPUTE_FITNESS;
                for (int i = 1; i < mpi_globals_nbnodes; i++)
                    MPI_Isend(&order, 1, MPI_INT, i, MPI_TAG, MPI_COMM_WORLD, &request);

                for (int i = 1; i < mpi_globals_nbnodes; i++) {
                        string soluString = bestSolution.str();
                        MPI_Isend((char *)soluString.c_str(), soluString.size(), MPI_CHAR, i, MPI_TAG, MPI_COMM_WORLD, &request);
                        MPI_Isend(&(parameter[i-1]), 1, MPI_INT, i, MPI_TAG, MPI_COMM_WORLD, &request);
                }

                for (int i = 1; i < mpi_globals_nbnodes; i++) {
                        int sizeOfmessage;
                        MPI_Probe(i, MPI_TAG, MPI_COMM_WORLD, &status);
                        MPI_Get_count(&status, MPI_CHAR, &sizeOfmessage);
                        char *msg = new char[sizeOfmessage + 1];
                        MPI_Recv(msg, sizeOfmessage, MPI_CHAR, i, MPI_TAG, MPI_COMM_WORLD, &status);
                        cout << "[MASTER] " << msg << endl;
                        solutionsAfterMutation[i - 1] = SOL(string(msg));
                        delete msg;
                }

                // Compute reward
                vector<pair<double, unsigned int>> rewardOp = _rewardComputation->operator()(solutions, solutionsAfterMutation, parameter);
                _parameterSelection->update(rewardOp);
                solutions = solutionsAfterMutation;
        } while(solutions[0].getFitness() < 50);
        // ------------------------------------------
        order = MPI_Order::FINISH;
        for (int i = 1; i < mpi_globals_nbnodes; i++)
            MPI_Isend(&order, 1, MPI_INT, i, MPI_TAG, MPI_COMM_WORLD, &request);
    }

   protected:
    int sizeOfmessage;
    MPI_Request request;
    MPI_Status status;

    unique_ptr<ParameterSelection> _parameterSelection;
    unique_ptr<RewardComputation<SOL>> _rewardComputation;
    unique_ptr<Selection<SOL>> _selection;
};

#endif
