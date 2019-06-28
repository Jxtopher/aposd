///
/// \file main.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2019-03
/// \brief 
///

//----------------------
#include <mpi.h>

int mpi_globals_nbnodes;
int mpi_globals_rank;
int mpi_globals_namelen;
char mpi_globals_name[MPI_MAX_PROCESSOR_NAME];

#define MPI_MASTER		0
#define MPI_TAG			0
//----------------------

#include <iostream>
#include <cstdlib>
#include <random>

#include "rewardComputation/rewardComputation.h"

#include "macro.h"

#include "configurations.h"

#include "launcher/launcher.h"
#include "launcher/launcherClass/launcherStocos.h"
#include "launcher/launcherExec.h"
//#include "launcher/launcherFork.h"
//#include "launcher/launcherClass/launcherClassOneMax.h"
#include "calculationModel/sequentialModel/sequentialModel.h"
#include "calculationModel/masterWorkers/masterWorkersSynchronous.h"
#include "calculationModel/masterWorkers/master.h"
#include "calculationModel/masterWorkers/masterSynchronous.h"
#include "parameterSelection/parameterSelection.h"
#include "parameterSelection/psConstant.h"
#include "parameterSelection/psRandom.h"
#include "settings/settings.h"
#include "solution/solution.h"
#include "selection/selection.h"
#include "selection/selection_maximization.h"

#include "calculationModel/islandModel/topologies/topologies.h"
#include "calculationModel/islandModel/topologies/circle.h"
#include "calculationModel/islandModel/topologies/complete.h"
#include "calculationModel/islandModel/topologies/star.h"
#include "calculationModel/islandModel/topologies/randomEdge.h"
#include "calculationModel/islandModel/islandModel.h"
#include "calculationModel/islandModel/sharedParameter.h"

using namespace std;

void version(string name_software, string num_version);

void version(string name_software, string num_version) {
	std::cout<<"*************************************"<<std::endl;
	std::cout<<"[+] *** "<<name_software<<" ***"<<std::endl;
	std::cout<<"[+] Day compilation : "<<__DATE__<<" "<<__TIME__<<std::endl;
	std::cout<<"[+] Version : "<<num_version<<std::endl;
	std::cout<<"*************************************"<<std::endl;
}

int main(int argc, char **argv) {
	unsigned long int seed1 = static_cast<unsigned long int>(time(0));
	std::mt19937 mt_rand1;
	mt_rand1.seed(seed1);

	Topologies *t = new Complete(5);
	t->print();

	IslandModel *im = new SharedParameter(argc, argv, *t);
	im->~IslandModel();

	exit(0);
	DEBUG_TRACE("Start of the program")

	Settings settings(argc, argv);

	//
	unsigned long int seed = static_cast<unsigned long int>(time(0));
	std::mt19937 mt_rand;
	mt_rand.seed(seed);

	Launcher *launcher = new LauncherExec("../stocos/build/stocos", "--budget=2 --problem=0 --instance=../stocos/instances/OneMax/onemax-50.json");
	//ParameterSelection *parameterSelection = new PsConstant(3,0);
	ParameterSelection *parameterSelection = new PsRandom(mt_rand, 3);
	RewardComputation<Solution<unsigned int>> *rewardComputation = new RewardComputation<Solution<unsigned int>>;
	SequentialModel<Solution<unsigned int>> sequentialModel(
		*launcher, 
		*parameterSelection, 
		*rewardComputation);

	// sequentialModel();
	Selection<Solution<unsigned int>> *selection = new Selection_maximization<Solution<unsigned int>>;

	MasterWorkersSynchronous<Solution<unsigned int>> mwSynchronous(argc, argv,
		*launcher, 
		*parameterSelection, 
		*rewardComputation,
		*selection);

	mwSynchronous();

	// Problème à résoudre
	//switch() {
	//	default:
	//	break;
	//}
	//LauncherClass *launcherClassOneMax = new LauncherClassOneMax;

	// Méthode de réglage de paramètre
	//switch() {
	//	default:
	//	break;
	//}
	
	//ParameterSelection *ssUniform = new SsUniform(4,0);


	// Calcul de la récompense
	//switch() {
	//	default:
	//	break;
	//}
	//RewardComputation *rewardComputation = new RewardComputation<SolutionArray <unsigned int, bool> >;


	//SequentialModel< SolutionArray <unsigned int, bool> > sequentialModel(launcherClassOneMax, ssUniform, rewardComputation);
	/*switch(MASTER_WORKER_MODEL) {
		case MASTER_WORKER_MODEL:
		break;
		case ISLAND_MODEL:
		break;
		case SEQUENTIAL_MODEL:
			
		break;
		default:
			THROW("The distributed model is not defined");
		break;
	}*/
	/*version("Hello World !", "1");
	
	for (int i =  0 ; i < 5 ; i++) {
		DEBUG_VAR(i)
		cout<<"Hello World!"<<endl;
	}

	if (0 == -1) {
		THROW("Error of variable i")
	}*/

	//LauncherFork lfork("kkkkk");
	//LauncherClass lClass;


	//lClass(NULL, 1);

	DEBUG_TRACE("Stop program")
	return EXIT_SUCCESS;
}