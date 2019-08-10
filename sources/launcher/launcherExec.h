#ifndef JXTOPHER_LAUNCHEREXEC_H
#define JXTOPHER_LAUNCHEREXEC_H

#include <iostream>
#include <sstream>
#include <unistd.h>

#include "launcher.h"
#include "../solution/solution.h"

using namespace std;

class LauncherExec : public Launcher {
	public:
		LauncherExec(const string &pathExecutable) : 
			Launcher(),
			_pathExecutable(pathExecutable),
			_args(""),
			bufferLength(1024) {
			assert(exists_file(_pathExecutable));
			buffer = new char[bufferLength];
		}

		LauncherExec(const string &pathExecutable, const string &args) : 
			Launcher(),
			_pathExecutable(pathExecutable),
			_args(args),
			bufferLength(1024) {
			assert(exists_file(_pathExecutable));
			buffer = new char[bufferLength];
		}
		
		virtual ~LauncherExec() {
			delete[] buffer;
		}

		string initSolution() {
			stringstream ss;
			ss<<"--initSolution";

			return launchExecutable(ss.str());
		}

		string solve(string solution, unsigned int numParameter) {
			stringstream ss;
			ss<<"--solution \""<<solution<<"\" ";
			ss<<"--parameter "<<numParameter;

			return launchExecutable(ss.str());
		}

		string launchExecutable(const string &cmdParameter) {
			stringstream ss;
			
			string cmd = _pathExecutable + " " + _args + " " + cmdParameter;
			//cout<<__LINE__<<" : "<<cmd<<endl;
			FILE* pipe = popen(cmd.c_str(), "r");
			try {
				while (!feof(pipe)) {
					if (fgets(buffer, static_cast<int>(bufferLength), pipe) != NULL) {
						ss << buffer;
					}
				}
			} catch (...) {
				pclose(pipe);
				throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " POPEN");
			}
			pclose(pipe);
			
			return ss.str();
		}

		bool exists_file (const  std::string & name) {
			return ( access( name.c_str(), F_OK ) != -1 );
		}

	private:
	const string _pathExecutable;
	const string _args;
	const unsigned int bufferLength;
	char *buffer;
};

#endif