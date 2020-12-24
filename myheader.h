#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <unistd.h>
#include <map>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>

using namespace std;

extern map <int, vector<string> > jobs_list;
extern bool boolbg;
extern bool boolfin;
extern bool boolfout;
extern int status;

void myexit();
string mypipe(string pid);
int check_alive(int pid);
void clean_jobs();
void jobs();
void mykill(int pid);
void resume(int pid);
void mysleep(int seconds);
void suspend(int pid);
void wait(int pid);
void add_job(pid_t pid, string cmd);
void handler(int sig);
void run_processes(vector <string> parsed_input);
void readline(string &input);
void run_shell();
void jobs();