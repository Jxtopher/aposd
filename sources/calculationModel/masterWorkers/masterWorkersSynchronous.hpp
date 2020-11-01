#ifndef JXTOPHER_MASTERWORKERSSYNCHRONOUS_H
#define JXTOPHER_MASTERWORKERSSYNCHRONOUS_H

#include <memory>
#include <mpi.h>

#include "../calculationModel.hpp"
#include "masterSynchronous.hpp"
#include "workersSynchronous.hpp"

template<class SOL>
class MasterWorkersSynchronous : public CalculationModel {
  public:
    MasterWorkersSynchronous(int argc, char** argv, std::unique_ptr<Launcher> launcher,
                             std::unique_ptr<ParameterSelection> parameterSelection,
                             std::unique_ptr<RewardComputation<SOL>> rewardComputation,
                             std::unique_ptr<SolutionSelection<SOL>> selection) {
        // Variable MPI init
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &mpi_globals_number_of_nodes);
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_globals_rank);
        MPI_Get_processor_name(mpi_globals_name, &mpi_globals_namelen);

        if(mpi_globals_number_of_nodes == 1) {
            // Not distributed
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
                                     " [-] Error we have only one node !");
        } else if(mpi_globals_rank == MPI_MASTER) {
            // MASTER
            mSynchro = std::make_unique<MasterSynchronous<SOL>>(
                std::move(parameterSelection), std::move(rewardComputation), std::move(selection));
        } else {
            // WORKER
            wSynchro = std::make_unique<WorkersSynchronous>(std::move(launcher));
        }
    }

    virtual ~MasterWorkersSynchronous() {
        MPI_Finalize();
    }

    void operator()() {
        if(mpi_globals_number_of_nodes == 1) {
            // Not distributed
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
                                     " [-] Error we have only one node !");
        } else if(mpi_globals_rank == MPI_MASTER) {
            // MASTER
            mSynchro->operator()();
        } else {
            // WORKER
            wSynchro->operator()();
        }
    }

  private:
    std::unique_ptr<MasterSynchronous<SOL>> mSynchro;
    std::unique_ptr<WorkersSynchronous> wSynchro;

    int mpi_globals_number_of_nodes;
    int mpi_globals_rank;
    int mpi_globals_namelen;
    char mpi_globals_name[MPI_MAX_PROCESSOR_NAME];
};

#endif