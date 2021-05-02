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

typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

void execute(cmdLine *pCmdLine) {
    if(execvp(pCmdLine->arguments[0],pCmdLine->arguments)==-1){
        perror("Error");
        _exit(1);
    }
    
}
    
void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *newProcess= (process*)malloc(sizeof(process));
    newProcess->cmd=cmd;
    newProcess->pid=pid;
    newProcess->status=RUNNING;
    newProcess->next=*process_list;
    *process_list=newProcess; 
}

void printProcessList(process** process_list){
    if(*process_list==NULL)
        return;
    printf("PID    Command    STATUS\n");
    process* temp=*process_list;
    while (temp!=NULL){
        printf("%d    %s           %d\n",temp->pid,temp->cmd->arguments[0],temp->status);
        temp=temp->next;
    }
}

void freeProcessList(process* process_list){
    process* curr = process_list;
    while(curr!=NULL){
        process* tmp = curr->next;
        free(curr->cmd);
        curr->next = NULL;
        curr = tmp;
    }
}

void updateProcessList(process **process_list){

}

int main(int argc, char **argv) {
    int debug=0;
    process* process_list=NULL;
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
        printf("%s$ ",workingDir);

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
            else if(strcmp(input->arguments[0],"procs")==0){//print all processes
                printProcessList(&process_list);
            }
            else if(!(cpid=fork())){//Code child
                execute(input);
            }
           else{//Code of parent
                addProcess(&process_list,input,cpid);
                if(input->blocking)
                    waitpid(cpid,NULL,0);
            }
        }
      
    }while(1);

    return 0;
}
