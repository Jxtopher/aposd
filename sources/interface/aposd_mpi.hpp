#ifndef JXTOPHER_APOSD_MPI_H
#define JXTOPHER_APOSD_MPI_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>

// -> MPI ---------------------
#include <mpi.h>

int mpi_globals_number_of_nodes;
int mpi_globals_rank;
int mpi_globals_namelen;
char mpi_globals_name[MPI_MAX_PROCESSOR_NAME];

#define MPI_MASTER 0
#define MPI_TAG 0
// <- MPI ---------------------

#include "../calculationModel/islandModel/islandModel.hpp"
#include "../calculationModel/islandModel/sharedParameter.hpp"
#include "../calculationModel/masterWorkers/master.hpp"
#include "../calculationModel/masterWorkers/masterSynchronous.hpp"
#include "../calculationModel/masterWorkers/masterWorkersSynchronous.hpp"

#include "../launcher/launcher.hpp"

#include "../parameterSelection/parameterSelection.hpp"
#include "../rewardComputation/rewardComputation.hpp"
#include "../selection/selection_maximization.hpp"
#include "../selection/solutionSelection.hpp"
#include "../solution/solution.hpp"
#include "classBuilder.hpp"

void Interface_MPI(int argc, char** argv, const Json::Value& configuration);

void Interface_MPI(int argc, char** argv, const Json::Value& configuration) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " CREATE Interface_MPI";
	std::shared_ptr<std::mt19937> mt_rand = std::make_shared<std::mt19937>();

	if(!configuration["seed"].empty())
		mt_rand->seed(configuration["seed"].asInt());
	else
		mt_rand->seed(static_cast<std::mt19937::result_type>(time(0)));

	ClassBuilder classBuilder(mt_rand);

	std::unique_ptr<Launcher> launcher =
		classBuilder.launcher(configuration["CalculationModel"]["Launcher"]);
	std::unique_ptr<ParameterSelection> parameterSelection =
		classBuilder.parameterSelection(configuration["CalculationModel"]["ParameterSelection"]);
	std::unique_ptr<RewardComputation<Solution<unsigned int>>> rewardComputation =
		classBuilder.rewardComputation<Solution<unsigned int>>(
			configuration["CalculationModel"]["RewardComputation"]);
	std::unique_ptr<SolutionSelection<Solution<unsigned int>>> selection =
		std::make_unique<Selection_maximization<Solution<unsigned int>>>();

	if(CalculationModel::MASTER_WORKER_MODEL ==
	   configuration["CalculationModel"]["className"].asString()) {
		MasterWorkersSynchronous<Solution<unsigned int>> calculationmodel(
			argc, argv, std::move(launcher), std::move(parameterSelection),
			std::move(rewardComputation), std::move(selection));

		calculationmodel();
	} else if(CalculationModel::ISLAND_MODEL ==
			  configuration["CalculationModel"]["className"].asString()) {
		std::unique_ptr<Topologies> topologies =
			classBuilder.topologies(configuration["CalculationModel"]["Topologies"]);

		SharedParameter<Solution<unsigned int>> calculationmodel(
			argc, argv, std::move(topologies), std::move(launcher), std::move(parameterSelection),
			std::move(rewardComputation), std::move(selection));

		calculationmodel();

	} else {
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
								 " [-] The calculation model is not defined");
	}
}

#endif