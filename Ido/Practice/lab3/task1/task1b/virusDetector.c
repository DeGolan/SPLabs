#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

typedef struct link link;

struct link
{
    link *nextVirus;
    virus *vir;
};

struct fun_desc
{
    char *name;
    link *(*fun)(link *);
};

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
    newVirus->sig = (unsigned char *)calloc(sizeof(unsigned char) * (newVirus->SigSize), 1);
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
/* Print the data of every link in list to the given stream. Each item followed by a newline character. */
void list_print(link *virus_list, FILE *output)
{
    link *temp = virus_list;
    while (temp != NULL)
    {
        printVirus(temp->vir, output);
        temp = temp->nextVirus;
    }
}

/* Add a new link with the given data to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - create a new entry and return a pointer to the entry. */
link *list_append(link *virus_list, virus *data)
{
    link *temp = virus_list;
    link *new_link = (link *)malloc(sizeof(link));
    new_link->nextVirus = NULL;
    new_link->vir = data;
    if (virus_list == NULL)
    {
        return new_link;
    }
    while (temp->nextVirus != NULL)
    {
        temp = temp->nextVirus;
    }
    temp->nextVirus = new_link;
    return virus_list;
}

/* Free the memory allocated by the list. */
void list_free(link *virus_list)
{
    link *temp = virus_list;
    link *toFree;
    while (temp->nextVirus != NULL)
    {
        toFree = temp;
        temp = temp->nextVirus;
        free(toFree->vir);
        free(toFree);
    }
    free(temp->vir);
    free(temp);
}

link *loadSignatures(link *viruses)
{
    char *path = NULL;
    int read;
    unsigned int len;
    fprintf(stdout, "Enter file path:\n");
    getchar();
    read = getline(&path, &len, stdin);
    if (-1 == read)
        printf("No line read...\n");
    char *buffer;
    path[strlen(path) - 1] = '\0';
    FILE *input = fopen(path, "r");
    free(path);

    if (input == NULL)
    {
        fputs("File error\n", stderr);
        fclose(input);
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
        viruses = list_append(viruses, virus);
        free(virus->sig);
    }

    fclose(input);
    free(buffer);
    return viruses;
}

link *printSignature(link *viruses)
{
    list_print(viruses, stdout);
    return viruses;
}

int main(int argc, char **argv)
{
    struct fun_desc menu[] = {{"Load Signatures", loadSignatures}, {"Print Signatures", printSignature}, {NULL, NULL}};
    int upperBound = sizeof(menu) / 8 - 1; // /8 for size ofstruct (2 pointers) -1 for the null at the end
    link *viruses = NULL;
    int i = 0;
    int option;
    while (1)
    {
        i = 0;
        while (1)
        {
            if (menu[i].name == NULL)
            {
                break;
            }
            fprintf(stdout, "%d)%s\n", i, menu[i].name);
            i++;
        }
        scanf("%d", &option);
        if (option < 0 || option > upperBound - 1)
        {
            fprintf(stdout, "OUT OF BOUNDS \n");
            break;
        }
        else
        {
            fprintf(stdout, "WITHIN BOUNDS\n");
            viruses = menu[option].fun(viruses);
            fprintf(stdout, "DONE.\n");
        }
    }
    list_free(viruses);
    return 0;
}
