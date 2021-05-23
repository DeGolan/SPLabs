#include <stdlib.h>
#include <stdio.h>

#define NAME_LEN 128

typedef struct
{
    char debug_mode;
    char file_name[NAME_LEN];
} elf;

struct fun_desc
{
    char *name;
    void (*fun)(elf *);
};

void debug(elf *e)
{
    if (e->debug_mode)
    {
        printf("Debug mode now OFF\n");
        e->debug_mode = 0;
    }
    else
    {
        printf("Debug mode now ON\n");
        e->debug_mode = 1;
    }
}

void examine(elf *e)
{
    printf("not implemented\n");
}
void printSections(elf *e)
{
    printf("not implemented\n");
}
void printSymbols(elf *e)
{
    printf("not implemented\n");
}
void relocationTables(elf *e)
{
    printf("not implemented\n");
}

int main(int argc, char **argv)
{
    elf *my_elf = (elf *)malloc(sizeof(my_elf));

    struct fun_desc menu[] = {{"Toggle Debug Mode", debug}, {"Examine ELF File", examine}, {"Print Section Names", printSections}, {"Print Symbols", printSymbols}, {"Relocation Tables", relocationTables}, {NULL, NULL}};

    int upperBound = sizeof(menu) / 8 - 1; // /8 for size ofstruct (2 pointers) -1 for the null at the end
    int option;
    printf("myELF:\n");
    do
    {
        printf("Please choose a function:\n");
        for (int i = 0; menu[i].name != NULL; i++)
        {
            printf("%d) %s\n", i, menu[i].name);
        }
        printf("Option: ");
        scanf("%d", &option);
        if (option >= 0 && option < upperBound)
        {
            menu[option].fun(my_elf);
        }

    } while (1);
    puts("Not within bounds");
    free(my_elf);
    return 0;
}