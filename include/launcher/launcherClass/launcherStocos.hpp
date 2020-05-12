#ifndef LAUNCHERSTOCOS_H
#define LAUNCHERSTOCOS_H

#include <iostream>
#include <random>

#include "../launcher.hpp"

class LauncherStocos : public Launcher {
  public:
	LauncherStocos();

	virtual ~LauncherStocos();

	std::string initSolution();

	std::string solve(std::string solution, unsigned int parameter_id);

  private:
};

#endif