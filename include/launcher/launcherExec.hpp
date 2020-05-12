#ifndef JXTOPHER_LAUNCHEREXEC_H
#define JXTOPHER_LAUNCHEREXEC_H

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string>

#include "../solution/solution.hpp"
#include "launcher.hpp"

class LauncherExec : public Launcher {
  public:
	LauncherExec(const std::string& pathExecutable);
	LauncherExec(const std::string& pathExecutable, const std::string& args);
	virtual ~LauncherExec();

	std::string initSolution();

	std::string solve(std::string solution, unsigned int parameter_id);

	std::string launchExecutable(const std::string& cmdParameter);

	bool exists_file(const std::string& name);

  private:
	const std::string _pathExecutable;
	const std::string _args;
	const unsigned int bufferLength;
	char* buffer;
};

#endif