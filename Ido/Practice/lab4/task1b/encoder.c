#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1
#define SYS_READ 3
#define SYS_OPEN 5
#define STDIN 0
#define STDERR 2
extern int system_call();

int main(int argc, char **argv)
{
    char inChar;
    int debug = 0;
    int numOfChangedChars = 0, i;
    char tempChar;
    int fdIn=STDIN;
    int fdOut=STDOUT;
    int error;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-D") == 0)
        {
            debug = 1;
            system_call(SYS_WRITE, STDOUT, argv[1], strlen(argv[1]));
            system_call(SYS_WRITE, STDOUT, "\n", 1);
        }
        else if (strncmp(argv[i], "-i", 2) == 0)
        {
            fdIn = system_call(SYS_OPEN,argv[i] + 2, 0,0777);
        }
        else if (strncmp(argv[i], "-o", 2) == 0)
        {
            fdOut = system_call(SYS_OPEN,argv[i] + 2, 1|64,0777);
        }
    }
    do
    {
        error=system_call(SYS_READ, fdIn, &inChar, 1);

        if ((debug) && ((inChar == 0) || inChar == 10))
        {
            system_call(SYS_WRITE, fdOut, "\nnumber of changed letters= ", 29);
            system_call(SYS_WRITE, fdOut, itoa(numOfChangedChars), 3);
            system_call(SYS_WRITE, fdOut, "\n", 1);
            numOfChangedChars = 0;
        }
        if (error <= 0)
        {
            break;
        }
        tempChar = inChar;
        if (inChar >= 'A' && inChar <= 'Z')
        {
            inChar = inChar + 32;
            numOfChangedChars++;
        }
        if (debug == 1 && inChar != 10)
        {
            system_call(SYS_WRITE, STDERR, inChar, 1);
            system_call(SYS_WRITE, STDERR, tempChar, 1);
            system_call(SYS_WRITE, STDERR, "\n", 1);
        }

        system_call(SYS_WRITE, fdOut, &inChar, 1);

    } while (1);
    system_call(SYS_WRITE, fdOut, "\n", 1);

    system_call(6, fdIn);

    return 0;
}
