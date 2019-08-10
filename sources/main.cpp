///
/// \file main.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2019-03
/// \brief 
///

// -> MPI ---------------------
#include <mpi.h>

int mpi_globals_nbnodes;
int mpi_globals_rank;
int mpi_globals_namelen;
char mpi_globals_name[MPI_MAX_PROCESSOR_NAME];

#define MPI_MASTER		0
#define MPI_TAG			0
// <- MPI ---------------------

#include "communicationModel/communicationModel.h"
#include "communicationModel/aposd_mpi.h"
#include "communicationModel/aposd_webApps.h"

#include <sstream> //for std::stringstream 
#include <string>  //for std::string


using namespace std;


int main(int argc, char **argv) {
	DEBUG_TRACE("Start of the program")

	switch (CommunicationModel::WEBAPPLICATION) {
	case CommunicationModel::MPI :
		CommunicationModel_MPI(argc, argv);
		break;
	case CommunicationModel::WEBAPPLICATION :
		CommunicationModel_webApps(argc, argv);
		break;
	default:
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The communication model is not defined");
		break;
	}
	

	DEBUG_TRACE("Stop program")
	return EXIT_SUCCESS;
}