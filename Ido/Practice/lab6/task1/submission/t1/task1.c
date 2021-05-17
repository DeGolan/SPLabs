#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int pipefd[2];
    // char buf;
    char *const ls[3] = {"ls", "-l", 0};
    char *const tail[4] = {"tail", "-n", "1", 0};
    pid_t child2;
    pid_t child1;
    int debug = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
        { //debug mode
            debug = 1;
            fprintf(stderr, "-d\n");
        }
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (debug)
        fprintf(stderr, "parent_process> forking..........\n");
    child1 = fork();

    if (child1 == 0) //child1
    {
        close(STDOUT_FILENO);
        if (debug)
            fprintf(stderr, "child1>redirecting stdout to the write end of the pipe...\n");
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        if (debug)
            fprintf(stderr, "child1>going to execute cmd:\"ls\"\n");
        execvp(ls[0], ls);
        exit(EXIT_SUCCESS);
    }
    else
    {
        if (debug)
        {
            fprintf(stderr, "parent_process> created process with id: %d\n", child1);
             fprintf(stderr, "parent_process> closing the write end of the pipe...\n");
        }
        close(pipefd[1]); /* Close write end */
        //if close write child is still waiting for more shit to be written
        child2 = fork();
        if (child2 == 0) //child1
        {
            close(STDIN_FILENO);
            if (debug)
                fprintf(stderr, "child2>redirecting stdin to the read end of the pipe...\n");
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (debug)
                fprintf(stderr, "child2>going to execute cmd:\"tail\"...\n");
            execvp(tail[0], tail);
            exit(EXIT_SUCCESS);
        }
        if (debug)
            fprintf(stderr, "parent_process> closing the read end of the pipe...\n");
        close(pipefd[0]);/*close read end*/
        //can actually not close read end, does not impact code except leaving file open for no reason
    }
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);
    //if we dont wait and dont close write then program finishes without running the command
    if (debug)
        fprintf(stderr, "parent_process> exiting...\n I'll be back motherfuckers....\n");
}