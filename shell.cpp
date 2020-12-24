#include "myheader.h"

void run_shell(){
//Gets input and calls run_processes to run something

    bool running = true;
    string input;
    vector <string> parsed_input (8);
    map <int, vector<string> > jobs_list;

    //Gets input, parses it then runs command and repeasts this until exit
    while(running){
        readline(input);
        boost::split(parsed_input, input, boost::is_any_of(" "));
        run_processes(parsed_input);   
    }
}//end run_shell


int main(int argc, char **argv){
//Makes a call to run the shell

    run_shell();
}//end main