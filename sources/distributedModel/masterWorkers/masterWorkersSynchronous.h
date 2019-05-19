#ifndef JXTOPHER_MASTERWORKERSSYNCHRONOUS_H
#define JXTOPHER_MASTERWORKERSSYNCHRONOUS_H

#include <mpi.h>

#include "masterSynchronous.h"
#include "workersSynchronous.h"

template <class SOL>
class MasterWorkersSynchronous {
   public:
    MasterWorkersSynchronous(int argc, char** argv, 
                            Launcher& launcher, 
                            ParameterSelection& parameterSelection,
                             RewardComputation<SOL>& rewardComputation,
                             Selection<SOL>& selection)
        : _launcher(launcher), 
        _parameterSelection(parameterSelection), 
        _rewardComputation(rewardComputation),
        _selection(selection) {
        // Variable MPI init
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &mpi_globals_nbnodes);
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_globals_rank);
        MPI_Get_processor_name(mpi_globals_name, &mpi_globals_namelen);
    }
    virtual ~MasterWorkersSynchronous() { MPI_Finalize(); }

    void operator()() {
        if (mpi_globals_nbnodes == 1) {
            // Not distributed
        } else if (mpi_globals_rank == MPI_MASTER) {
            // MASTER
            MasterSynchronous<SOL> mSynchro(_parameterSelection, _rewardComputation, _selection);
            mSynchro();
        } else {
            // WORKER
            WorkersSynchronous wSynchro(_launcher);
            wSynchro();
        }
    }

   private:
    Launcher& _launcher;
    ParameterSelection& _parameterSelection;
    RewardComputation<SOL>& _rewardComputation;
    Selection<SOL>& _selection;
};

#endif