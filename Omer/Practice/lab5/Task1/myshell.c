#include <unistd.h>
#include "LineParser.h"
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0
#define BUF_SIZE 2048

void execute(cmdLine *pCmdLine) {
    //execute command
    if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
        perror("Error");
        _exit(1);
    }  
}

int main(int argc, char **argv) {
    int debug=0;
    
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-D")==0){ //debug mode
            debug=1;
            fprintf(stderr,"-D\n");
        }
    }

    do{
        char workingDir[PATH_MAX];
        char buf[BUF_SIZE];
        pid_t cpid=getpid();

        //Display a prompt - the current working directory
        if(getcwd(workingDir,PATH_MAX)==NULL){
            perror("Error");
            _exit(1);
        }
        printf("%s\n",workingDir);

        fgets(buf,BUF_SIZE,stdin);//Read a line from the "user"
        if(debug){//debug mode
            fprintf(stderr,"PID: %d\nExecuting command: %s\n",cpid,buf);
        }
        if(!strcmp(buf,"quit\n")){//quit
            exit(1);
        }
        cmdLine* input=parseCmdLines(buf); //Parse the input using parseCmdLines() 
        if(input){
            if(strcmp(input->arguments[0],"cd")==0){//Change directory 
                if(chdir(input->arguments[1])==-1){
                    perror("Error");
                    _exit(1);
                }
            }
            else if(!(cpid=fork())){//Code child
                execute(input);
            }
            else{//Code of parent
                if(input->blocking)
                    waitpid(cpid,NULL,0);
            }
        }
      
    }while(1);

    return 0;
}
