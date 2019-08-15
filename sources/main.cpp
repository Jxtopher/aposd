///
/// @file main.cpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2019-03
/// @brief 
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

#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <map>
#include <sstream> //for std::stringstream 
#include <string>  //for std::string
#include <jsoncpp/json/json.h>
#include <fstream>

#include <boost/program_options.hpp>

#include "communicationModel/communicationModel.h"
#include "communicationModel/aposd_mpi.h"
#include "communicationModel/aposd_webApps.h"
#include "communicationModel/aposd_sequential.h"

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
    string configFile; // Chemin du ficher de configuration json

	boost::program_options::variables_map vm;
	boost::program_options::options_description argements("[*] main option");
	argements.add_options()
						("help,h", "help message")
						("config,c", boost::program_options::value<string>(&configFile), "File configuration json (default : null)");
	try {
    	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, argements), vm);
		boost::program_options::notify(vm);
	} catch (const boost::program_options::error &ex) {
        throw runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " [-] error program_options");
  	}

	if (vm.count("version")) {
			version("Adaptive Portfolio Selection Distributed", "1");
			exit(EXIT_SUCCESS);
	}

	if (vm.count("help")) {
			cout<<argements<<endl;
			exit(EXIT_SUCCESS);
	}

    if (configFile.empty()) {
        cerr<<"./aposd -c config.json"<<endl;
        exit(EXIT_FAILURE);
    }

    // Read json file
    Json::Value configuration;
    Json::Reader reader;
    std::ifstream test(configFile, std::ifstream::binary);
    bool parsingSuccessful = reader.parse(test, configuration, false);

    if (!parsingSuccessful)
        throw runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " " +reader.getFormattedErrorMessages());

    std::string encoding = configuration.get("encoding", "UTF-8").asString();

	if (configuration["aposd"]["CommunicationModel"] == CommunicationModel::MPI) {
		CommunicationModel_MPI(argc, argv, configuration["aposd"]);
	} else if (configuration["aposd"]["CommunicationModel"] == CommunicationModel::WEBAPPLICATION) {
		CommunicationModel_webApps(argc, argv, configuration["aposd"]);
	} else if (configuration["aposd"]["CommunicationModel"] == CommunicationModel::SEQUENTIAL) {
		CommunicationModel_sequential(argc, argv, configuration["aposd"]);
	} else {
		throw runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " [-] Communication model "+ configuration["aposd"]["CommunicationModel"].asString() +" does not exist.");
	}

	return EXIT_SUCCESS;
}