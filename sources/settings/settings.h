#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace std;

class Settings {
    public:
    Settings(int argc, char **argv) :
    _argc(argc),
    _argv(argv) {
        // Default setting
        seed = static_cast<unsigned long int>(time(0));

        // User setting
        po::options_description desc("Allowed options");
        desc.add_options()
        ("seed", po::value<unsigned long int>(&seed), "seed (default: time(0))")
        ;

        po::store(po::parse_command_line(_argc, _argv, desc), vm);
        po::notify(vm);

        // Show help
        if (vm.count("help")) {
                cout<<desc<<endl;
                exit(EXIT_SUCCESS);
        }

        if (vm.count("version")) {
                std::cout<<"*************************************"<<std::endl;
                std::cout<<"[+] *** Master-Slave MATSim ***"<<std::endl;
                std::cout<<"[+] Day compilation : "<<__DATE__<<" "<<__TIME__<<std::endl;
                std::cout<<"[+] Name : MATSim MasterSlave MPI"<<endl;
                std::cout<<"[+] Version : 1"<<std::endl;
                std::cout<<"*************************************"<<std::endl;
                exit(EXIT_SUCCESS);
        }
    }

    virtual ~Settings() {

    }


    
    unsigned long int seed;

    protected:
    po::variables_map vm;
    int _argc;
    char **_argv;
};


#endif