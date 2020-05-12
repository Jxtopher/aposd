#ifndef JXTOPHER_MASTERSYNCHRONOUS_H
#define JXTOPHER_MASTERSYNCHRONOUS_H

#include <memory>
#include <vector>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

#include "../../launcher/launcher.hpp"
#include "../../parameterSelection/parameterSelection.hpp"
#include "../../rewardComputation/rewardComputation.hpp"
#include "../../selection/solutionSelection.hpp"
#include "../calculationModel.hpp"

#include "../mpiConfig.hpp"

#include "master.hpp"

template<class SOL> class MasterSynchronous : public Master {
  public:
	MasterSynchronous(std::unique_ptr<ParameterSelection> parameterSelection,
					  std::unique_ptr<RewardComputation<SOL>> rewardComputation,
					  std::unique_ptr<SolutionSelection<SOL>> selection);

	virtual ~MasterSynchronous();

	virtual void operator()();

  protected:
	int sizeOfmessage;
	MPI_Request request;
	MPI_Status status;

	int mpi_globals_number_of_nodes;
	int mpi_globals_rank;
	int mpi_globals_namelen;
	char mpi_globals_name[MPI_MAX_PROCESSOR_NAME];

	std::unique_ptr<ParameterSelection> _parameterSelection;
	std::unique_ptr<RewardComputation<SOL>> _rewardComputation;
	std::unique_ptr<SolutionSelection<SOL>> _selection;
};

#endif
