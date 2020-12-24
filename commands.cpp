#include "myheader.h"

void myexit(){
//Removes suspended processes then waits for running processes to finish before exiting

    //Kill any suspend process
    for(auto const& x : jobs_list){
        if(x.second[0] == "S"){
            mykill(x.first);
        }
    }

    //Keep waiting until all processes are done running
    while(!jobs_list.empty()){
        clean_jobs();
    }

    struct rusage usage;
    getrusage(RUSAGE_CHILDREN, &usage);
    cout.flush();
    cout << "\nResources used";
    cout << "\nuser time: " << usage.ru_utime.tv_sec << " seconds";
    cout << "\nsystem time: " << usage.ru_stime.tv_sec << " seconds";
    cout.flush();

    _exit(1);
}//end exit


void mykill(int pid){
//Kills process at a given pid

    kill(pid, SIGKILL);
}//end mykill


void resume(int pid){
//Resumes process and updates entry at jobs list to reflect that process is in state 'R'

    kill(pid, SIGCONT);
    vector <string> temp = jobs_list.at(pid);
    temp[0] = "R";
    jobs_list.at(pid) = temp;
}//end resume


void mysleep(int seconds){
//Sleep for x amt of seconds

    sleep(seconds);
}//end mysleep


void suspend(int pid){
//Suspend process at given pid and take its current run time

    kill(pid, SIGSTOP);
    vector <string> temp = jobs_list.at(pid);
    temp[0] = "S";
    temp[2] = mypipe(to_string(pid));
    jobs_list.at(pid) = temp;   
}//end suspend


void wait(int pid){
//Wait for a given pid tp finish execution

    waitpid(pid, NULL, 0);
}//end wait


void jobs(){
//Print out jobs_list along with relevant information

    //Remove completed jobs before printing information
    clean_jobs();

    //Initializat variables
    int active_process = jobs_list.size();
    int job_counter = 0;
    cout << "Running processes:";
    cout << "\n#      PID     S     COMMAND      SEC";

    //Loop through jobs listand print information on each process
    for(auto const& x : jobs_list){
        string seconds = mypipe(to_string(x.first));
        if(x.second[0] != "R"){
            active_process--;
            cout << "\n" << job_counter << "    " << x.first << "    " << x.second[0] << "    " << x.second[1] << "    " << x.second[2];
        } else {
        cout << "\n" << job_counter << "    " << x.first << "    " << x.second[0] << "    " << x.second[1] << "    " << seconds;
        }
    }
    cout << "\nProcesses = " << active_process << " active";

    //Print time
    struct rusage usage;
    getrusage(RUSAGE_CHILDREN, &usage);
    cout.flush();
    cout << "\nCompleted processes: ";
    cout << "\nuser time: " << usage.ru_utime.tv_sec << " seconds";
    cout << "\nsystem time: " << usage.ru_stime.tv_sec << " seconds";
    cout.flush();
}//end jobs