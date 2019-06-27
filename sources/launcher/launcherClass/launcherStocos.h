#ifndef LAUNCHERSTOCOS_H
#define LAUNCHERSTOCOS_H

#include <iostream>

#include "../launcher.h"
#include "stocos/sources/solution/solution.h"

using namespace std;

class LauncherStocos : public Launcher {
    public:
    LauncherStocos() {
        std::mt19937 mt_rand;
    }

    virtual ~LauncherStocos() {

    }


    string initSolution() {
        return "";
    }

	string solve(string solution, unsigned int numParameter) {
        return "";
    }

    private:


};


#endif