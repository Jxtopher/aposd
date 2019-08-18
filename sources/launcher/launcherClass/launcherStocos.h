#ifndef LAUNCHERSTOCOS_H
#define LAUNCHERSTOCOS_H

#include <iostream>

#include "../launcher.h"
#include "stocos/sources/solution/solution.h"



class LauncherStocos : public Launcher {
    public:
    LauncherStocos() {
        std::mt19937 mt_rand;
    }

    virtual ~LauncherStocos() {

    }


    std::string initSolution() {
        return "";
    }

	std::string solve(std::string solution, unsigned int numParameter) {
        return "";
    }

    private:


};


#endif