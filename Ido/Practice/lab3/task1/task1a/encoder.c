#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

void PrintHex(char *buffer, int length, FILE *output)
{
    for (size_t i = 0; i < length; i++)
    {
        fprintf(output, "%02X ", buffer[i] & 0xff); // pads with 0 to length 2
    }
    fprintf(output, "\n");
}

virus *readVirus(FILE *file)
{
    virus *newVirus = (virus *)calloc(sizeof(virus), 1);
    fread(newVirus, 1, 18, file);
    newVirus->sig = (unsigned char*)calloc(sizeof(unsigned char)*(newVirus->SigSize), 1);
    fread(newVirus->sig, 1, newVirus->SigSize, file);
    return newVirus;
}

void printVirus(virus *virus, FILE *output)
{
    fprintf(output, "Virus name:%s\n", virus->virusName);
    fprintf(output, "Virus size:%d\n", virus->SigSize);
    fprintf(output, "Signature:\n");
    PrintHex(virus->sig, virus->SigSize, output);
}

int main(int argc, char **argv)
{
    char *buffer;
    FILE *input = fopen(argv[1], "r");

    if (input == NULL)
    {
        fputs("File error", stderr);
        exit(1);
    }

    buffer = (char *)malloc(sizeof(char) * 4);
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    fread(buffer, 1, 4, input); //reads the first 4 bytes with the signature
    while (1)
    {
        virus *virus = readVirus(input);
        if (feof(input)) //best way to exit loop
        {
            free(virus->sig);
            free(virus);
            break;
        }
        printVirus(virus, stdout);
        free(virus->sig);
        free(virus);
    }

    fclose(input);
    free(buffer);

    return 0;
}
