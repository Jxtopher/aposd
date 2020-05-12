#ifndef JXTOPHER_MASTERWORKERSSYNCHRONOUS_H
#define JXTOPHER_MASTERWORKERSSYNCHRONOUS_H

#include <memory>
#include <mpi.h>

#include "../../launcher/launcher.hpp"
#include "../calculationModel.hpp"
#include "masterSynchronous.hpp"
#include "workersSynchronous.hpp"

template<class SOL> class MasterWorkersSynchronous : public CalculationModel {
  public:
	MasterWorkersSynchronous(int argc, char** argv, std::unique_ptr<Launcher> launcher,
							 std::unique_ptr<ParameterSelection> parameterSelection,
							 std::unique_ptr<RewardComputation<SOL>> rewardComputation,
							 std::unique_ptr<SolutionSelection<SOL>> selection);
	virtual ~MasterWorkersSynchronous();

	void operator()();
	
  private:
	int mpi_globals_number_of_nodes;
	int mpi_globals_rank;
	int mpi_globals_namelen;
	char mpi_globals_name[MPI_MAX_PROCESSOR_NAME];

	std::unique_ptr<MasterSynchronous<SOL>> mSynchro;
	std::unique_ptr<WorkersSynchronous> wSynchro;
};

#endif