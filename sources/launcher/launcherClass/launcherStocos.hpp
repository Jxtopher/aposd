#ifndef LAUNCHERSTOCOS_H
#define LAUNCHERSTOCOS_H

#include <iostream>
#include <random>

#include "../launcher.hpp"

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

    std::string solve(std::string solution, unsigned int parameter_id) {
        return "";
    }

  private:
};

#endif