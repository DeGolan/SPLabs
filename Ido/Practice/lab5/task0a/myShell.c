#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "LineParser.h"

#define BUF_SIZE 2048

void execute(cmdLine *pCmdLine)
{
    if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1)
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
