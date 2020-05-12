#include "calculationModel/masterWorkers/masterWorkersSynchronous.hpp"

template<class SOL>
MasterWorkersSynchronous<SOL>::MasterWorkersSynchronous(int argc, char** argv, std::unique_ptr<Launcher> launcher,
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
		mSynchro = std::make_unique<MasterSynchronous<SOL>>(std::move(parameterSelection), std::move(rewardComputation), std::move(selection));
	} else {
		// WORKER
		wSynchro = std::make_unique<WorkersSynchronous>(std::move(launcher));
	}
}

template<class SOL>
MasterWorkersSynchronous<SOL>::~MasterWorkersSynchronous() { MPI_Finalize(); }

template<class SOL>
void MasterWorkersSynchronous<SOL>::operator()() {
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