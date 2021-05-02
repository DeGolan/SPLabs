#include <unistd.h>
#include "LineParser.h"
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUF_SIZE 2048
int debug=0;

void execute(cmdLine *pCmdLine) {
   
    if(fork()==0){
        if(debug){
            fprintf(stderr,"PID: %d\n",getpid());
            fprintf(stderr,"Executing command: %s\n",pCmdLine->arguments[0]);
        }
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
            perror("Error");
            //free(pCmdLine);
            _exit(1);
        }
    }
        else{
            wait(NULL);

        }

}
    

int main(int argc, char **argv) {
    
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-D")==0){ //debug mode
            debug=1;
            fprintf(stderr,"-D\n");
        }
    }
    do{
        //Display a prompt - the current working directory
        char workingDir[PATH_MAX];
        if(getcwd(workingDir,PATH_MAX)==NULL){
            perror("Error");
            _exit(1);
        }
        printf("%s\n",workingDir);
        //Read a line from the "user"
        char buf[BUF_SIZE];
        fgets(buf,BUF_SIZE,stdin);
        if(!strcmp(buf,"quit\n")){
            break;
        }
        //Parse the input using parseCmdLines() 
        cmdLine* input=parseCmdLines(buf);
        execute(input);
        //free(input);
    }while(1);
    return 0;
}
