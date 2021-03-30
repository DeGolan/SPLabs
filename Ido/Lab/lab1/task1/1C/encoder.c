#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int main(int argc, char **argv)
{
    FILE *input = stdin;
    char inChar;
    bool debug = false;
    int numOfChangedChars = 0, i, encoder = 0;
    char tempChar,temp;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-D") == 0)
        {
            debug = true;
            printf("%s\n", argv[1]);
        }
        temp=(argv[i][2]);
        if (strncmp(argv[i], "+e", 2) == 0 && isxdigit(temp))
        {
            if(temp>='A')
            {
                encoder = (temp-'A'+10);
            }
            else{
                encoder = (temp-'0');
            }
        }
        if (strncmp(argv[i], "-e", 2) == 0 && isxdigit(temp))
        {
            if(temp>='A')
            {
                encoder = -(temp-'A'+10);
            }
            else{
                encoder = -(temp-'0');
            };
        }
    }

    do
    {
        inChar = fgetc(input);
        if ((debug) && ((inChar == EOF) || inChar == 10))
        {
            fprintf(stderr, "number of changed letters= %i\n", numOfChangedChars);
            numOfChangedChars = 0;
        }
        if (feof(input))
        {
            break;
        }
        tempChar = inChar;
        inChar = inChar + encoder;
        numOfChangedChars++;

        if (debug && inChar != 10)
        {
            fprintf(stderr, "%d %d\n", inChar, tempChar);
        }
        if (inChar != 10+encoder){
        printf("%c", inChar);
        }

    } while (1);

    fclose(input);

    return 0;
}
