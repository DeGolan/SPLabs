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
#include <errno.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0
#define BUF_SIZE 2048
#define MAX_HISTORY 10
#define PIPE_SIZE 2

int historyIndex = 0;

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
        fd1 = open(pCmdLine->outputRedirect, O_CREAT | O_WRONLY, 0777);
        dup2(fd1, STDOUT_FILENO);
        close(fd1);
    }
    if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1)
    {
        perror("Error");
        _exit(1);
    }
}

void insertToHistory(cmdLine **history, cmdLine *line)
{
    if (historyIndex <= 9)
    {
        history[historyIndex] = line;
        historyIndex++;
    }
    else
    {
        freeCmdLines(history[0]);
        for (size_t i = 0; i < 9; i++)
        {
            history[i] = history[i + 1];
        }
        history[9] = line;
    }
}

int main(int argc, char **argv)
{
    int debug = 0;
    int piped = 0;
    int pipefd[PIPE_SIZE];
    cmdLine *history[MAX_HISTORY];

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
        pid_t child2;

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
                for (size_t i = 0; i < historyIndex; i++)
                {
                    for (size_t j = i; j < historyIndex; j++)
                    {
                        if (&history[i] == &history[j])
                        {
                            history[j] = NULL;
                        }
                    }

                    if (history[i] != NULL) //can not delete what is already deleted
                    {
                        freeCmdLines(history[i]);
                        history[i] = NULL; //need help avi help us please
                    }
                }
                freeCmdLines(input);
                _exit(1);
            }
            if (input->arguments[0][0] == '!')
            { //run from history
                int historicCommandIndex = input->arguments[0][1] - '0';
                if (historicCommandIndex < historyIndex)
                {
                    input = history[historicCommandIndex];
                }
                else
                {
                    fprintf(stderr, "invalid history index \n log has :%d\n you asked for:%d\n", historyIndex, historicCommandIndex);
                    _exit(1);
                }
            }
            if (input->next != NULL)
            { //PIPE
                piped = 1;
                if (pipe(pipefd) == -1)
                {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
            }
            if (strcmp(input->arguments[0], "cd") == 0)
            { //Change directory
                if (chdir(input->arguments[1]) == -1)
                {
                    perror("Error");
                    _exit(1);
                }
                insertToHistory(history, input); //add here because no fork
            }

            else if (strcmp(input->arguments[0], "history") == 0)
            { //print history
                for (size_t i = 0; i < historyIndex; i++)
                {
                    fprintf(stdout, "%s\n", history[i]->arguments[0]);
                }
                insertToHistory(history, input); //add here because no fork
            }

            else if (!(cpid = fork()))
            { //Code child
                if (piped)
                {
                    close(STDOUT_FILENO);
                    if (debug)
                        fprintf(stderr, "child1>redirecting stdout to the write end of the pipe...\n");
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                }
                execute(input);
            }
            else
            { //Code of parent
                if (piped)
                {
                    cmdLine *rightEnd = input->next; //PIPE
                    close(pipefd[1]);                /* Close write end */
                    child2 = fork();
                    if (child2 == 0) //child1
                    {
                        // close(pipefd[1]);                /* Close write end */
                        close(STDIN_FILENO);
                        if (debug)
                            fprintf(stderr, "child2>redirecting stdin to the read end of the pipe...\n");
                        dup2(pipefd[0], STDIN_FILENO);
                        close(pipefd[0]);
                        if (debug)
                            fprintf(stderr, "child2>going to execute cmd:\"tail\"...\n");
                        execute(rightEnd);
                    }
                    else
                    {
                        if (debug)
                            fprintf(stderr, "parent_process> closing the read end of the pipe...\n");
                        close(pipefd[0]); /*close read end*/
                        //can actually not close read end, does not impact code except leaving file open for no reason
                    }
                    piped = 0;
                    waitpid(child2, NULL, 0);
                }

                if (input->blocking)
                {
                    waitpid(cpid, NULL, 0);
                }
                insertToHistory(history, input);
            }
        }
        // freeCmdLines(input);
    } while (1);

    return 0;
}
