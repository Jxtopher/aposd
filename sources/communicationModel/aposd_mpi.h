#ifndef JXTOPHER_APOSD_MPI_H
#define JXTOPHER_APOSD_MPI_H

#include <cstdlib>
#include <iostream>
#include <random>
#include <memory>

#include "../calculationModel/islandModel/islandModel.h"
#include "../calculationModel/islandModel/sharedParameter.h"
#include "../calculationModel/masterWorkers/master.h"
#include "../calculationModel/masterWorkers/masterSynchronous.h"
#include "../calculationModel/masterWorkers/masterWorkersSynchronous.h"

#include "../launcher/launcher.h"

#include "../macro.h"

#include "../parameterSelection/parameterSelection.h"
#include "../rewardComputation/rewardComputation.h"
#include "../selection/selection.h"
#include "../selection/selection_maximization.h"
#include "../solution/solution.h"
#include "classBuilder.h"

void CommunicationModel_MPI(int argc, char **argv, const Json::Value &configuration);

void CommunicationModel_MPI(int argc, char **argv, const Json::Value &configuration) {
	DEBUG_TRACE("CREATE CommunicationModel_MPI")
    std::shared_ptr<std::mt19937> mt_rand = make_shared<std::mt19937>();

    if (!configuration["seed"].empty())
        mt_rand->seed(configuration["seed"].isInt());
    else
        mt_rand->seed(static_cast<mt19937::result_type>(time(0)));
	
    ClassBuilder classBuilder(mt_rand);
    
    std::unique_ptr<Launcher> launcher = classBuilder.launcher(configuration["CalculationModel"]["Launcher"]);
    std::unique_ptr<ParameterSelection> parameterSelection = classBuilder.parameterSelection(configuration["CalculationModel"]["ParameterSelection"]);
    std::unique_ptr<RewardComputation<Solution<unsigned int>>> rewardComputation = classBuilder.rewardComputation<Solution<unsigned int>>(configuration["CalculationModel"]["RewardComputation"]);
	std::unique_ptr<Selection<Solution<unsigned int>>> selection = make_unique<Selection_maximization<Solution<unsigned int>>>();
	

	if (CalculationModel::MASTER_WORKER_MODEL == configuration["CalculationModel"]["className"].asString()) {
		MasterWorkersSynchronous<Solution<unsigned int>> calculationmodel(
			argc, 
			argv,
			std::move(launcher), 
			std::move(parameterSelection), 
			std::move(rewardComputation),
			std::move(selection));
		
		calculationmodel();
	} else if (CalculationModel::ISLAND_MODEL == configuration["CalculationModel"]["className"].asString()) {
		std::unique_ptr<Topologies> topologies = classBuilder.topologies(configuration["CalculationModel"]["Topologies"]);
		
		SharedParameter<Solution<unsigned int>> calculationmodel(
			argc, 
			argv,
			std::move(topologies),
			std::move(launcher), 
			std::move(parameterSelection),
			std::move(rewardComputation),
			std::move(selection));

		calculationmodel();

	} else {
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The calculation model is not defined");
	}
}

#endif