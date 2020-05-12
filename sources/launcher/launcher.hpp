#ifndef JXTOPHER_LAUNCHER_H
#define JXTOPHER_LAUNCHER_H

#include <string>

#include "../solution/solution.hpp"

class Launcher {
  public:
	Launcher() {
	}

	virtual ~Launcher() {
	}

	virtual std::string initSolution() = 0;
	virtual std::string solve(std::string solution, unsigned int parameter_id) = 0;

	static constexpr const char* LAUNCHEREXEC = "LauncherExec";
	static constexpr const char* LAUNCHERFORK = "LauncherFork";

  protected:
};

#endif
