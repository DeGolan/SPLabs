#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "LineParser.h"
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 2048
int debug = 0;

void execute(cmdLine *pCmdLine)
{
    pid_t pid = fork();
    int* statLoc;
    if (pid == -1)
    {
        perror("Could not fork");
        _exit(1);
    }
    if (pid == 0)
    {
        if (debug)
        {
            fprintf(stderr, "PID: %d\n", getpid());
            fprintf(stderr, "Executing command: %s\n", pCmdLine->arguments[0]);
        }
        if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1)
        {
            perror("Error");
            _exit(1);
        }
    }
    else
    {
        waitpid(pid,statLoc,WEXITED);
    }
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
        { //debug mode
            debug = 1;
            fprintf(stderr, "-d\n");
        }
    }
    char workingDir[PATH_MAX];
    char buff[BUF_SIZE];
    cmdLine *line;

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
        execute(line);
        //free(line);
    } while (1);
    return 0;
}
