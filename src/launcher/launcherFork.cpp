#include "launcher/launcherFork.hpp"

LauncherFork::LauncherFork(std::string subExecName) { // : Launcher()
	char* argv[3] = {NULL};

	/* create the pipe */
	if(pipe(read_pipe) == -1) {
		printf("pipe failed\n");
		exit(1);
	}
	if(pipe(write_pipe) == -1) {
		printf("pipe failed\n");
		exit(1);
	}

	pid = fork();

	switch(pid) {
		case -1: perror("fork"); exit(1);
		case 0: /* le fils */
			std::cout << "processus fils" << std::endl;
			close(PARENT_READ);
			close(PARENT_WRITE);

			dup2(CHILD_READ, STDIN_FILENO);
			dup2(CHILD_WRITE, STDOUT_FILENO);
			close(CHILD_READ);
			close(CHILD_WRITE);
			/* execute the process (ls command) */
			execvp("./echoecho", argv);
			exit(2);
		default: /* le pere */
			close(CHILD_READ);
			close(CHILD_WRITE);

			std::cout << "pere: a cree processus " << pid << std::endl;
	}
}

LauncherFork::~LauncherFork() {
	wait(&status);
	if(WIFEXITED(status))
		std::cout << "fils termine normalement: status = " << WEXITSTATUS(status) << std::endl;
	else
		std::cout << "fils termine anormalement" << std::endl;
}

std::string LauncherFork::operator()(std::string solution, double fitness, unsigned int parameter_id) {
	unsigned int size_msg = 2;
	unsigned char buf[400];
	buf[0] = 'A';
	buf[1] = 'C';

	write(PARENT_WRITE, &size_msg, sizeof(unsigned int));
	write(PARENT_WRITE, buf, size_msg);

	unsigned int size_msg_bis = 0;
	unsigned char buf_bis[400];
	read(PARENT_READ, &size_msg_bis, sizeof(unsigned int));
	read(PARENT_READ, buf_bis, size_msg_bis);

	std::cout << "****" << buf_bis << "*****" << std::endl;
	return "";
}
