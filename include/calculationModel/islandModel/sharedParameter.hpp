#ifndef JXTOPHER_SHAREDPARAMETER_H
#define JXTOPHER_SHAREDPARAMETER_H

#include <mutex>
#include <thread>
#include <tuple>

#include "../../launcher/launcher.hpp"
#include "../../parameterSelection/parameterSelection.hpp"
#include "../../rewardComputation/rewardComputation.hpp"
#include "../../selection/solutionSelection.hpp"
#include "../mpiConfig.hpp"
#include "islandModel.hpp"
#include "topologies/topologies.hpp"

template<class SOL> class SharedParameter : public IslandModel<SOL> {
  public:
	SharedParameter(int argc, char** argv, std::unique_ptr<Topologies> topologies,
					std::unique_ptr<Launcher> launcher,
					std::unique_ptr<ParameterSelection> parameterSelection,
					std::unique_ptr<RewardComputation<SOL>> rewardComputation,
					std::unique_ptr<SolutionSelection<SOL>> selection);
	virtual ~SharedParameter();

	void operator()();

  private:
	std::unique_ptr<Launcher> _launcher;
	std::unique_ptr<ParameterSelection> _parameterSelection;
	std::unique_ptr<RewardComputation<SOL>> _rewardComputation;
	std::unique_ptr<SolutionSelection<SOL>> _selection;

	std::vector<SOL> solutions; // liste des solutions candidat

	int continued; // loop
	int order;	 // order to send

	MPI_Status status;
	MPI_Request request;

	double recv_reward;		// Get reward by recv
	int recv_parameter;		// Get parameter by recv
	int recv_sizeOfmessage; // Get size of string solution by recv
	char* recv_msg;			// Get string solution by recv
	int recv_msg_size;		// size of recv_msg
};

#endif