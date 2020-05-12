#include "launcher/launcherExec.hpp"

LauncherExec::LauncherExec(const std::string& pathExecutable)
	: Launcher(), _pathExecutable(pathExecutable), _args(""), bufferLength(1024) {
	if(!exists_file(_pathExecutable))
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
									"The path \"" + _pathExecutable + "\" does not exsit");
	buffer = new char[bufferLength];
}

LauncherExec::LauncherExec(const std::string& pathExecutable, const std::string& args)
	: Launcher(), _pathExecutable(pathExecutable), _args(args), bufferLength(1024) {
	if(!exists_file(_pathExecutable))
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
									"The path \"" + _pathExecutable + "\" does not exsit");
	buffer = new char[bufferLength];
}

LauncherExec::~LauncherExec() { delete[] buffer; }

std::string LauncherExec::initSolution() {
	std::stringstream ss;
	ss << "--initSolution";

	return launchExecutable(ss.str());
}

std::string LauncherExec::solve(std::string solution, unsigned int parameter_id) {
	std::stringstream ss;
	ss << "--solution \"" << solution << "\" ";
	ss << "--parameter " << parameter_id;

	return launchExecutable(ss.str());
}

std::string LauncherExec::launchExecutable(const std::string& cmdParameter) {
	std::stringstream ss;

	std::string cmd = _pathExecutable + " " + _args + " " + cmdParameter;
	// std::cout<<__LINE__<<" : "<<cmd<<std::endl;
	FILE* pipe = popen(cmd.c_str(), "r");
	try {
		while(!feof(pipe)) {
			if(fgets(buffer, static_cast<int>(bufferLength), pipe) != NULL) { ss << buffer; }
		}
	} catch(...) {
		pclose(pipe);
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
									" POPEN");
	}
	pclose(pipe);

	return ss.str();
}

bool LauncherExec::exists_file(const std::string& name) { return (access(name.c_str(), F_OK) != -1); }