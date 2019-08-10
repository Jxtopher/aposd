#ifndef JXTOPHER_APOSD_MPI_H
#define JXTOPHER_APOSD_MPI_H

#include <iostream>
#include <cstdlib>
#include <random>

#include "../rewardComputation/rewardComputation.h"

#include "../macro.h"

#include "../launcher/launcher.h"
#include "../launcher/launcherClass/launcherStocos.h"
#include "../launcher/launcherExec.h"
//#include "launcher/launcherFork.h"
//#include "launcher/launcherClass/launcherClassOneMax.h"
#include "../calculationModel/sequentialModel/sequentialModel.h"
#include "../calculationModel/masterWorkers/masterWorkersSynchronous.h"
#include "../calculationModel/masterWorkers/master.h"
#include "../calculationModel/masterWorkers/masterSynchronous.h"
#include "../parameterSelection/parameterSelection.h"
#include "../parameterSelection/psConstant.h"
#include "../parameterSelection/psRandom.h"
#include "../parameterSelection/psAdaptivePursuit.h"
#include "../parameterSelection/psUCBW.h"
#include "../parameterSelection/psSelectBestMutate.h"
#include "../parameterSelection/psEpsilonGreedyW.h"
#include "../settings/settings.h"
#include "../solution/solution.h"
#include "../selection/selection.h"
#include "../selection/selection_maximization.h"

#include "../calculationModel/islandModel/topologies/topologies.h"
#include "../calculationModel/islandModel/topologies/circle.h"
#include "../calculationModel/islandModel/topologies/complete.h"
#include "../calculationModel/islandModel/topologies/star.h"
#include "../calculationModel/islandModel/topologies/randomEdge.h"
#include "../calculationModel/islandModel/islandModel.h"
#include "../calculationModel/islandModel/sharedParameter.h"




void version(string name_software, string num_version);
void CommunicationModel_MPI(int argc, char **argv);


void version(string name_software, string num_version) {
	std::cout<<"*************************************"<<std::endl;
	std::cout<<"[+] *** "<<name_software<<" ***"<<std::endl;
	std::cout<<"[+] Day compilation : "<<__DATE__<<" "<<__TIME__<<std::endl;
	std::cout<<"[+] Version : "<<num_version<<std::endl;
	std::cout<<"*************************************"<<std::endl;
}


void CommunicationModel_MPI(int argc, char **argv) {
	Settings settings(argc, argv);


	//
	unsigned long int seed = static_cast<unsigned long int>(time(0));
	std::mt19937 mt_rand;
	mt_rand.seed(seed);



	Topologies *topologies;
	if (CalculationModel::ISLAND_MODEL) {
		topologies = new Complete(4);
		topologies->print();
	}


	// Problème à résoudre
	Launcher *launcher = new LauncherExec("../stocos/build/stocos", "--budget=2 --problem=0 --instance=../stocos/instances/OneMax/onemax-50.json");
	//switch() {
	//	default:
	//	break;
	//}
	//LauncherClass *launcherClassOneMax = new LauncherClassOneMax;

	// Méthode de réglage de paramètre
	unsigned int nbParameter = 3;
	ParameterSelection *parameterSelection;
	switch(ParameterSelection::RANDOM) {
        case ParameterSelection::ADAPTIVEPURSUIT :
			parameterSelection = new PsAdaptivePursuit(mt_rand,nbParameter);
			break;
        case ParameterSelection::CONSTANT :
		 	parameterSelection = new PsConstant(nbParameter,0);
			break;
        case ParameterSelection::EPSILONGREEDY :
			parameterSelection = new PsEspsilonGreedy(mt_rand, nbParameter);
			break;
        case ParameterSelection::RANDOM :
			parameterSelection = new PsRandom(mt_rand, nbParameter);
			break;
        case ParameterSelection::SELECTBESTMUTATE :
			//parameterSelection = new PsSelectBestMutate(mt_rand, nbParameter);
			break;
        case ParameterSelection::UCBW :
			parameterSelection = new PsUCBW(mt_rand, nbParameter);
			break;
		default:
			assert("The calculation model is not defined" && false);
		break;
	}


	// Calcul de la récompense
	RewardComputation<Solution<unsigned int>> *rewardComputation = new RewardComputation<Solution<unsigned int>>;
	// switch() {
	// }
	//RewardComputation *rewardComputation = new RewardComputation<SolutionArray <unsigned int, bool> >;


	// Selection de la meilleurs solution
	Selection<Solution<unsigned int>> *selection = new Selection_maximization<Solution<unsigned int>>;


	// Modèle de calcule
	CalculationModel *calculationmodel;
	switch(CalculationModel::MASTER_WORKER_WEBSOCKET) {
		case CalculationModel::MASTER_WORKER_MODEL:
			calculationmodel = new MasterWorkersSynchronous<Solution<unsigned int>>(argc, argv,
				*launcher, 
				*parameterSelection,
				*rewardComputation,
				*selection);
		break;
		case CalculationModel::ISLAND_MODEL:
			calculationmodel = new SharedParameter<Solution<unsigned int>>(argc, argv, 
			*topologies, 
			*launcher,
			*parameterSelection, 
			*rewardComputation,
			*selection);
		break;
		case CalculationModel::SEQUENTIAL_MODEL:
			calculationmodel = new SequentialModel<Solution<unsigned int>>(
				*launcher, 
				*parameterSelection, 
				*rewardComputation);
		break;
		default:
			assert("The calculation model is not defined" && false);
		break;
	}
	
	calculationmodel->operator()();
	calculationmodel->~CalculationModel();
}

#endif