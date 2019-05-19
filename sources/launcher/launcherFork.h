#ifndef LAUNCHERFORK_H
#define LAUNCHERFORK_H

#include <string>
#include <cstring>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

#include "launcher.h"


#define PARENT_READ read_pipe[0]
#define PARENT_WRITE write_pipe[1]
#define CHILD_WRITE read_pipe[1]
#define CHILD_READ  write_pipe[0]

using namespace std;

class LauncherFork  {//: public Launcher {
	public:
		LauncherFork(string subExecName) { // : Launcher()
			 char*  argv[3] = {NULL};

			/* create the pipe */
			if (pipe(read_pipe) == -1) {
				printf("pipe failed\n");
				exit(1) ;
			}
			if (pipe(write_pipe) == -1) {
				printf("pipe failed\n");
				exit(1) ;
			}

			pid = fork () ;
			
			switch (pid) {
				case -1 :
					perror ("fork") ; 
					exit (1) ;
				case 0 : /* le fils */
					cout<<"processus fils"<<endl;
					close(PARENT_READ);
					close(PARENT_WRITE);

					dup2(CHILD_READ, STDIN_FILENO);
					dup2(CHILD_WRITE, STDOUT_FILENO);
					close(CHILD_READ);
					close(CHILD_WRITE);
       			/* execute the process (ls command) */
					execvp( "./echoecho", argv);
					exit (2) ;
				default : /* le pere */
					close(CHILD_READ);
					close(CHILD_WRITE);

					
					cout<<"pere: a cree processus "<< pid<<endl;

			}
		}

		virtual ~LauncherFork() {
			wait (&status);
			if (WIFEXITED (status))
				cout<<"fils termine normalement: status = "<<WEXITSTATUS(status)<<endl;
			else
				cout<<"fils termine anormalement"<<endl;
		}

		string operator()(string solution, double fitness, unsigned int numParameter)  {
			unsigned int size_msg = 2;
			unsigned char buf[400];
			buf[0] = 'A';
			buf[1] = 'C';
			
			
			write(PARENT_WRITE,  &size_msg, sizeof(unsigned int));
			write(PARENT_WRITE,  buf, size_msg);




			unsigned int size_msg_bis = 0;
			unsigned char buf_bis[400];
			read(PARENT_READ, &size_msg_bis, sizeof(unsigned int));
			read(PARENT_READ, buf_bis, size_msg_bis);

			cout<<"****"<<buf_bis<<"*****"<<endl;
			return "";
		}


		protected:
			int status;
			pid_t pid;
			int read_pipe[2];
			int write_pipe[2];
};

#endif
