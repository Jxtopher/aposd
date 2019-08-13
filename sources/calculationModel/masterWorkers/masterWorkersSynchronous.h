#ifndef JXTOPHER_MASTERWORKERSSYNCHRONOUS_H
#define JXTOPHER_MASTERWORKERSSYNCHRONOUS_H

#include <memory>
#include <mpi.h>

#include "../calculationModel.h"
#include "masterSynchronous.h"
#include "workersSynchronous.h"

using namespace std;

template <class SOL>
class MasterWorkersSynchronous : public CalculationModel {
   public:
    MasterWorkersSynchronous(int argc, char** argv, 
                            unique_ptr<Launcher> launcher, 
                            unique_ptr<ParameterSelection> parameterSelection,
                             unique_ptr<RewardComputation<SOL>> rewardComputation,
                             unique_ptr<Selection<SOL>> selection) {
        // Variable MPI init
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &mpi_globals_nbnodes);
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_globals_rank);
        MPI_Get_processor_name(mpi_globals_name, &mpi_globals_namelen);

        if (mpi_globals_nbnodes == 1) {
            // Not distributed
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] Error we have only one node !");
        } else if (mpi_globals_rank == MPI_MASTER) {
            // MASTER
            mSynchro = make_unique<MasterSynchronous<SOL>>(std::move(parameterSelection), std::move(rewardComputation), std::move(selection));
        } else {
            // WORKER
            wSynchro = make_unique<WorkersSynchronous>(std::move(launcher));
        }
    }
    virtual ~MasterWorkersSynchronous() { MPI_Finalize(); }

    void operator()() {
        if (mpi_globals_nbnodes == 1) {
            // Not distributed
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] Error we have only one node !");
        } else if (mpi_globals_rank == MPI_MASTER) {
            // MASTER
            mSynchro->operator()();
        } else {
            // WORKER
            wSynchro->operator()();
        }
    }

   private:
        unique_ptr<MasterSynchronous<SOL>> mSynchro;
        unique_ptr<WorkersSynchronous> wSynchro;
};

#endif