///
/// @file main.cpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2019-03
/// @brief 
///



#include <boost/program_options.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <signal.h>
#include <sstream> //for std::stringstream
#include <stdexcept>
#include <string>  //for std::string

#include "interface/interface.h"


#if MODULE_MPI
	// -> MPI ---------------------
	#include <mpi.h>

	int mpi_globals_nbnodes;
	int mpi_globals_rank;
	int mpi_globals_namelen;
	char mpi_globals_name[MPI_MAX_PROCESSOR_NAME];

	#define MPI_MASTER		0
	#define MPI_TAG			0
	// <- MPI ---------------------

	#include "interface/aposd_mpi.h"
#endif

#if MODULE_SAAS
	#include <jsoncpp/json/json.h>

	#include "interface/aposd_webApps.h"
#endif

#if MODULE_SEQ
	#include "interface/aposd_sequential.h"
#endif

using namespace std;

void version(string name_software, string num_version);

void version(string name_software, string num_version) {
	std::cout<<"*************************************"<<std::endl;
	std::cout<<"[+] *** "<<name_software<<" ***"<<std::endl;
	std::cout<<"[+] Day compilation : "<<__DATE__<<" "<<__TIME__<<std::endl;
	std::cout<<"[+] Version : "<<num_version<<std::endl;
	std::cout<<"[+] MODULE MPI : "<<MODULE_MPI<<std::endl;
	std::cout<<"[+] MODULE_SAAS : "<<MODULE_SAAS<<std::endl;
	std::cout<<"[+] MODULE_SEQ : "<<MODULE_SEQ<<std::endl;
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

	if (configuration["aposd"]["Interface"] == Interface::MPI) {
		#if MODULE_MPI
			Interface_MPI(argc, argv, configuration["aposd"]);
		#else
			throw runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " [-] The MPI module is not include of the binary. Please turn true of MODULE_MPI in complilation.");
		#endif
	} else if (configuration["aposd"]["Interface"] == Interface::WEBAPPLICATION) {
		#if MODULE_SAAS
		Interface_webApps(argc, argv, configuration["aposd"]);
		#else
			throw runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " [-] The SaaS module is not include of the binary. Please turn true of MODULE_SAAS in complilation.");
		#endif
	} else if (configuration["aposd"]["Interface"] == Interface::SEQUENTIAL) {
		#if MODULE_SEQ
			Interface_sequential(argc, argv, configuration["aposd"]);
		#else
			throw runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " [-] The sequential module is not include of the binary. Please turn true of MODULE_SEQ in complilation.");
		#endif
	} else {
		throw runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " [-] Communication model "+ configuration["aposd"]["Interface"].asString() +" does not exist.");
	}

	return EXIT_SUCCESS;
}