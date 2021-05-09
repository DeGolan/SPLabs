#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "LineParser.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct process
{
    cmdLine *cmd;         /* the parsed command line*/
    pid_t pid;            /* the process id that is running the command*/
    int status;           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next; /* next process in chain */
} process;

#define for_each(item, list) \
    for (process *item = *list; item != NULL; item = item->next)

#define BUF_SIZE 2048
int debug = 0;
#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0
process *process_List;

char *returnStatus(int status)
{
    switch (status)
    {
    case -1:
        return "TERMINATED";
    case 0:
        return "SUSPENDED";
    case 1:
        return "RUNNING";
    default:
        break;
    }
    return "";
}

void addProcess(process **process_list, cmdLine *cmd, pid_t pid)
{
    process *newProcess = (process *)malloc(sizeof(process));
    newProcess->cmd = cmd;
    newProcess->pid = pid;
    newProcess->status = RUNNING;
    newProcess->next = *process_list;
    *process_list = newProcess;
}

void updateProcessStatus(process *process_list, int pid, int status)
{
    for_each(proc, &process_list)
    {
        if (proc->pid == pid)
        {
            proc->status = status;
        }
    }
}

void updateProcessList(process **process_list)
{
    int status = 0;
    for_each(proc, process_list)
    {
        if (waitpid(proc->pid, &status, WNOHANG) != 0)
        {
            if (status == 0)
            {
                updateProcessStatus(*process_list, proc->pid, -1);
            }
        }
    }
}

void removeTerminatedProcesses(process **process_list)
{
    if (*process_list != NULL)
    {
        process *prev = *process_list;
        process *curr = prev->next;
        if (prev->status == -1 && curr == NULL)
        {
            free(prev->cmd);
        }
        while (prev->status == -1 && curr != NULL)
        {
            printf("%d, %d\n", prev->pid, prev->status);
            prev->next = NULL;
            //  free(prev->cmd);
            *prev = *curr;
            curr=prev->next;
        }
        
        process_list = &curr;
        // if (prev->status == -1)
        // {
        //     process_list = &curr;
        //     free(prev->cmd);
        //     prev->next = NULL;
        // }
        // while (curr != NULL)
        // {
        //     if (curr->status == -1)
        //     {
        //         prev->next = curr->next;
        //         free(curr->cmd);
        //         curr->next = NULL;
        //         curr = prev->next;
        //     }
        //     else
        //     {
        //         prev = curr;
        //         curr = curr->next;
        //     }
        // }
    }
}
void printProcessList(process **process_list)
{
    updateProcessList(process_list);
    printf("PID     Command     STATUS\n");
    for_each(proc, process_list)
    {
        printf("%d      %s      %s\n", proc->pid, proc->cmd->arguments[0], returnStatus(proc->status));
    }
    removeTerminatedProcesses(process_list);
}

void freeProcessList(process *process_list)
{
    process *curr = process_list;
    while (curr != NULL)
    {
        process *tmp = curr->next;
        free(curr->cmd);
        curr->next = NULL;
        curr = tmp;
    }
}

void execute(cmdLine *pCmdLine)
{
    if (debug)
    {
        fprintf(stderr, "PID: %d\n", getpid());
        fprintf(stderr, "Executing command: %s\n", pCmdLine->arguments[0]);
    }
    else if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1)
    {
        perror("Error");
        _exit(1);
    }
}

int main(int argc, char **argv)
{
    char workingDir[PATH_MAX];
    char buff[BUF_SIZE];
    cmdLine *line;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
        { //debug mode
            debug = 1;
            fprintf(stderr, "-d\n");
        }
    }

    do
    {
        //get working dir and print
        if (getcwd(workingDir, PATH_MAX) == NULL)
        {
            perror("Error");
            _exit(1);
        }
        fprintf(stdout, "%s\n", workingDir);
        //get buff from user
        fgets(buff, BUF_SIZE, stdin);
        if (!strcmp(buff, "quit\n"))
        {
            break;
        }
        //parse and execute command
        line = parseCmdLines(buff);
        if (!strcmp(line->arguments[0], "cd"))
        {
            if (chdir(line->arguments[1]) == -1)
            {
                perror("Error");
                _exit(1);
            }
        }
        else if (!strcmp(line->arguments[0], "procs"))
        {
            printProcessList(&process_List);
        }
        else
        {
            pid_t pid = fork();
            addProcess(&process_List, line, pid);
            if (pid == -1)
            {
                perror("Could not fork");
                _exit(1);
            }
            else if (pid == 0)
            {
                execute(line);
            }
            else if (line->blocking)
            {
                waitpid(pid, NULL, 0);
            }
        }

        //free(line);
    } while (1);
    freeProcessList(process_List);
    return 0;
}
