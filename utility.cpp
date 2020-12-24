#include "myheader.h"

int status;
map <int, vector<string>> jobs_list;

string mypipe(string pid){
//Retrive seconds a process has been running for a given pid

    //Initalize variables and add entered pid to the ps cmd
    array<char, 128> buffer;
    string cmd = "ps -o etimes= -p ";
    cmd.append(pid);
    cout.flush();
    string seconds;
    char ccmd[cmd.length()];

    //Convert string to char*
    for(unsigned int i = 0; i < sizeof(ccmd); i++){
        ccmd[i] = cmd[i];
    }
    
    //https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
    //Code used to pipe and retrive data
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(ccmd, "r"), pclose);
    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr){
        seconds += buffer.data();
    }
    return seconds;
}//end mypipe


int check_alive(int pid){
//Check whether a process is alive or dead (returns 1 on alive and -1 on dead)

    if(kill(pid, 0) == 0){
        return 1;
    } else {
        return -1;
    }
}//end check_alive


void clean_jobs(){
//Remove dead processes from the jobs list

    for(auto it = jobs_list.begin(), next_it = it; it != jobs_list.end(); it = next_it){
        ++next_it;
        if(check_alive(it->first) != 1){
            jobs_list.erase(it);
        }
    }
}//end clean_jobs


void add_job(pid_t pid, string cmd){
//Take the pid and command and add the info along with other relevant info to jobs_list

    vector<string> p_info;
    p_info.push_back("R");
    p_info.push_back(cmd);
    p_info.push_back("0");
    jobs_list.insert({(int) pid, p_info});
}//end add_job


void handler(int sig){
    waitpid(-1, &status, WNOHANG);
}


void readline(string &input){
//Prompts user to enter something into shell

    cout << "\nSHELL379: ";
    getline(cin, input);
}//end readline


void run_processes(vector <string> parsed_input){
//Run the speific process

    //Initalize variables
    bool boolbg = false;
    bool boolfin = false;
    bool boolfout = false;
    string cmd = parsed_input[0];;
    string fullcmd;
    for(const auto &piece: parsed_input) fullcmd += piece; //Transforms vector parsed_input into a string
    int fin;
    int fout;

    signal(SIGCHLD, handler);

    //Check for special commands otherwise execvp on parsed_input
    if(cmd == "exit"){
        myexit();
    } else if (cmd == "jobs"){
        jobs();
    } else if (cmd == "kill"){
        mykill(stoi(parsed_input[1]));
    } else if (cmd == "resume"){
        resume(stoi(parsed_input[1]));
    } else if (cmd == "sleep"){
        mysleep(stoi(parsed_input[1]));
    } else if (cmd == "suspend"){
        suspend(stoi(parsed_input[1]));
    } else if (cmd == "wait"){
        wait(stoi(parsed_input[1]));
    } else if (cmd == "check_alive"){
        check_alive(stoi(parsed_input[1]));
    } else {
        //Check for special arguments in parsed_input and keep note of what appears
        for(unsigned int i = 0; i < parsed_input.size(); i++){
            for(unsigned int j = 0; j < parsed_input[i].size(); j++){
                if(parsed_input[i][j] == '&'){
                    boolbg = true;
                    parsed_input.erase(parsed_input.begin() + i);
                }
                if(parsed_input[i][j] == '<'){
                    boolfin = true;
                    fin = (int) i;
                }
                if(parsed_input[i][j] == '>'){
                    boolfout = true;
                    fout = (int) i;
                }
            }
        }

        //Fork the process
        pid_t pid = fork();
        if(pid < 0){
            cout << "Error forking process";
            _Exit(EXIT_FAILURE);
        } else if (pid == 0){ //0 indicates a child process

            //Set file to stdin if '<' symbol was found
            if(boolfin == true){
                int fd = open(parsed_input[fin].erase(0,1).c_str(), S_IRUSR);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            //Set file to stdout if '>' symbol was found
            if(boolfout == true){
                int fd = open(parsed_input[fout].erase(0,1).c_str(), O_CREAT|O_WRONLY, 0777);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            //Convert vector to char** for execvp and run comamnd
            vector <char*> ptr_list;
            for(size_t i = 0; i < parsed_input.size(); i++){
                ptr_list.push_back(&parsed_input.at(i)[0]);
            }
            ptr_list.push_back(nullptr);
            char **argvv = &ptr_list[0];
            execvp(argvv[0], ptr_list.data());

        } else { //Parent code
            //Add to jobs list and check if cmd is running in bg
            add_job(pid, fullcmd);
            if(boolbg == true){
            } else {
                waitpid(pid, &status, 0);
            }
            boolbg = false;
            boolfin = false;
            boolfout = false;
        }
    }
}//end run_processes
