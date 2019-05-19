#ifndef LAUNCHERSTOCOS_H
#define LAUNCHERSTOCOS_H

#include "../launcher.h"

#include "iostream"

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