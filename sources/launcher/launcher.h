#ifndef JXTOPHER_LAUNCHER_H
#define JXTOPHER_LAUNCHER_H

#include <string>

#include "../solution/solution.h"

using namespace std;

class Launcher {
	public:
		Launcher() {

		}

		virtual ~Launcher() {

		}

		virtual string initSolution() = 0;
		virtual string solve(string solution, unsigned int numParameter) = 0;
		

		static constexpr const char* LAUNCHEREXEC = "LauncherExec";
        static constexpr const char* LAUNCHERFORK = "LauncherFork";
	protected:


};

#endif
