#include <unistd.h>
#include "LineParser.h"
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0
#define BUF_SIZE 2048

void execute(cmdLine *pCmdLine)
{
    int fd0;
    int fd1;
    //execute command
    if (pCmdLine->inputRedirect != NULL)
    {
        fd0 = open(pCmdLine->inputRedirect, O_RDONLY);
        dup2(fd0, STDIN_FILENO);
        close(fd0);
    }
    if (pCmdLine->outputRedirect != NULL)
    {
        fd1 = open(pCmdLine->outputRedirect, O_CREAT|O_WRONLY);
        dup2(fd1, STDOUT_FILENO);
        close(fd1);
    }
    if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1)
    {
        perror("Error");
        _exit(1);
    }
}

int main(int argc, char **argv)
{
    int debug = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-D") == 0)
        { //debug mode
            debug = 1;
            fprintf(stderr, "-D\n");
        }
    }

    do
    {
        char workingDir[PATH_MAX];
        char buf[BUF_SIZE];
        pid_t cpid = getpid();

        //Display a prompt - the current working directory
        if (getcwd(workingDir, PATH_MAX) == NULL)
        {
            perror("Error");
            _exit(1);
        }
        printf("%s$ ", workingDir);

        fgets(buf, BUF_SIZE, stdin); //Read a line from the "user"
        if (debug)
        { //debug mode
            fprintf(stderr, "PID: %d\nExecuting command: %s\n", cpid, buf);
        }

        cmdLine *input = parseCmdLines(buf); //Parse the input using parseCmdLines()
        if (input)
        {
            if (!strcmp(buf, "quit\n"))
            { //quit
                _exit(1);
            }

            else if (strcmp(input->arguments[0], "cd") == 0)
            { //Change directory
                if (chdir(input->arguments[1]) == -1)
                {
                    perror("Error");
                    _exit(1);
                }
            }
            else if (!(cpid = fork()))
            { //Code child
                execute(input);
            }
            else
            { //Code of parent
                if (input->blocking)
                {
                    waitpid(cpid, NULL, 0);
                }
            }
        }
        freeCmdLines(input);
    } while (1);

    return 0;
}
