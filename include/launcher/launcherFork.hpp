#ifndef LAUNCHERFORK_H
#define LAUNCHERFORK_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "launcher.hpp"

#define PARENT_READ read_pipe[0]
#define PARENT_WRITE write_pipe[1]
#define CHILD_WRITE read_pipe[1]
#define CHILD_READ write_pipe[0]

class LauncherFork { //: public Launcher {
  public:
	LauncherFork(std::string subExecName);

	virtual ~LauncherFork();

	std::string operator()(std::string solution, double fitness, unsigned int parameter_id);

  protected:
	int status;
	pid_t pid;
	int read_pipe[2];
	int write_pipe[2];
};

#endif
