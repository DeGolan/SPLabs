#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int main(int argc, char **argv)
{
    FILE *input = stdin;
    FILE *output = stdout;
    char inChar;
    bool debug = false;
    int numOfChangedChars = 0, i, encoder = 0;
    char tempChar, temp;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-D") == 0)
        {
            debug = true;
        }
        else
        {
            temp = (argv[i][2]);
            if (strncmp(argv[i], "+e", 2) == 0 && isxdigit(temp))
            {
                if (temp >= 'A')
                {
                    encoder = (temp - 'A' + 10);
                }
                else
                {
                    encoder = (temp - '0');
                }
            }
            else if (strncmp(argv[i], "-e", 2) == 0 && isxdigit(temp))
            {
                if (temp >= 'A')
                {
                    encoder = -(temp - 'A' + 10);
                }
                else
                {
                    encoder = -(temp - '0');
                };
            }
            else if (strncmp(argv[i], "-i", 2) == 0)
            {
                input = fopen(argv[i] + 2, "r");
            }

            else if (strncmp(argv[i], "-o", 2) == 0)
            { //write to a file
                output = fopen(argv[i] + 2, "w");
            }
        }
        if (debug)
        {
            printf("%s\n", argv[i]);
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

        if (debug && tempChar != 10)
        {
            fprintf(stderr, "%d %d\n", tempChar, inChar);
        }
        if (tempChar != 10)
        {
            fprintf(output,"%c", inChar);
        }
        if (output != stdout)
        { //flush the file writing stream
            fflush(output);
        }

    } while (1);

    printf("\n");
    fclose(input);
    if(output!=stdout)
        fclose(output);

    return 0;
}