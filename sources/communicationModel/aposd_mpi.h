#ifndef JXTOPHER_APOSD_MPI_H
#define JXTOPHER_APOSD_MPI_H

#include <cstdlib>
#include <iostream>
#include <random>

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
#include "../settings/settings.h"
#include "../solution/solution.h"
#include "classBuilder.h"

void CommunicationModel_MPI(int argc, char **argv, const Json::Value &configuration);

void CommunicationModel_MPI(int argc, char **argv, const Json::Value &configuration) {
    std::mt19937 mt_rand;
    ClassBuilder classBuilder(mt_rand);

    if (!configuration["seed"].empty())
        mt_rand.seed(configuration["seed"].isInt());
    else
        mt_rand.seed(static_cast<mt19937::result_type>(time(0)));
    
    unique_ptr<Launcher> launcher = classBuilder.launcher(configuration["Launcher"]);
    unique_ptr<ParameterSelection> parameterSelection = classBuilder.parameterSelection(configuration["ParameterSelection"]);
    unique_ptr<RewardComputation<Solution<unsigned int>>> rewardComputation = classBuilder.rewardComputation<Solution<unsigned int>>(configuration["RewardComputation"]);
	unique_ptr<Selection<Solution<unsigned int>>> selection = make_unique<Selection_maximization<Solution<unsigned int>>>();
	

	if (CalculationModel::MASTER_WORKER_MODEL == configuration["className"].asString()) {
		MasterWorkersSynchronous<Solution<unsigned int>> calculationmodel(
			argc, 
			argv,
			std::move(launcher), 
			std::move(parameterSelection), 
			std::move(rewardComputation),
			std::move(selection));
		
		calculationmodel();
	} else if (CalculationModel::ISLAND_MODEL == configuration["className"].asString()) {
		unique_ptr<Topologies> topologies = classBuilder.topologies(configuration["Topologies"]);
		
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