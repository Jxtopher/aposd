#ifndef JXTOPHER_APOSD_MPI_H
#define JXTOPHER_APOSD_MPI_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>

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
#include "../calculationModel/mpiConfig.hpp"
#include "classBuilder.hpp"

void Interface_MPI(int argc, char** argv, const Json::Value& configuration);

#endif