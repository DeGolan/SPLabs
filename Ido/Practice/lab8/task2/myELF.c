#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <elf.h>
#define NAME_LEN 128

int Currentfd = -1;

typedef struct
{
    char debug_mode;
    char file_name[NAME_LEN];
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    char *addr;
} elf;

struct fun_desc
{
    char *name;
    void (*fun)(elf *);
};

char *getSectionType(int type)
{
    switch (type)
    {
    case 0:
        return "SHT_NULL";
        break;
    case 1:
        return "SHT_PROGBITS";
        break;
    case 2:
        return "SHT_SYMTAB";
        break;
    case 3:
        return "SHT_STRTAB";
        break;
    case 4:
        return "SHT_RELA";
        break;
    case 5:
        return "SHT_HASH";
        break;
    case 6:
        return "SHT_DYNAMIC";
        break;
    case 7:
        return "SHT_NOTE";
        break;
    case 8:
        return "SHT_NOBITS";
        break;
    case 9:
        return "SHT_REL";
        break;
    case 10:
        return "SHT_SHLIB";
        break;
    case 11:
        return "SHT_DYNSYM";
        break;
    case 12:
        return "SHT_LOPROC, SHT_HIPROC";
        break;
    case 13:
        return "SHT_LOUSER";
        break;
    case 14:
        return "SHT_HIUSER";
        break;

    default:
        return "UNKNOWN SECTION TYPE";
        break;
    }
}

void quit(elf *e)
{
    printf("Quitting...\n");
    // free(e->ehdr);
    free(e);
    exit(1);
}

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
    size_t length;
    if (Currentfd != -1)
    {
        close(Currentfd);
    }
    struct stat sb;
    printf("Please enter ELF file name:\n");
    scanf("%s", e->file_name);
    Currentfd = open(e->file_name, O_RDONLY);
    if (fstat(Currentfd, &sb) == -1)
    { /* To obtain file size */
        fprintf(stderr, "Invalid File\n Quitting...\n");
        quit(e);
    }
    length = sb.st_size;

    e->addr = mmap(NULL, length, PROT_READ,
                   MAP_PRIVATE, Currentfd, 0);
    if (e->addr == MAP_FAILED)
    { /* To obtain file size */
        fprintf(stderr, "Could Not Map\n Quitting...\n");
        quit(e);
    }
    if (e->addr[0] != 0x7f || e->addr[1] != 0x45 || e->addr[2] != 0x4c)
    {
        fprintf(stderr, "Not an ELF file\n Quitting...\n");
        quit(e);
    }
    e->ehdr = (Elf32_Ehdr *)e->addr;
    e->shdr = (Elf32_Shdr *)(e->addr + e->ehdr->e_shoff);
    char *data_format;
    if (e->ehdr->e_ident[EI_DATA] == 0)
        data_format = "ELFCLASSNONE";
    if (e->ehdr->e_ident[EI_DATA] == 1)
        data_format = "ELFCLASS32";
    if (e->ehdr->e_ident[EI_DATA] == 2)
        data_format = "ELFCLASS64";
    printf("Data Encoding Scheme:%s\n", data_format);
    printf("Entry Point:%x\n", e->ehdr->e_entry);
    printf("File Offset:%d\n", e->ehdr->e_shoff);
    printf("Num of Section Headers:%d\n", e->ehdr->e_shnum);
    printf("Section Header Size:%d\n", e->ehdr->e_shentsize); //all section headers are the same size
    printf("Program Header Offset:%d\n", e->ehdr->e_phoff);
    printf("Num of Program Headers:%d\n", e->ehdr->e_phnum);
    printf("Program Header Size:%d\n", e->ehdr->e_phentsize);
}
void printSections(elf *e)
{
    Elf32_Shdr *sh_strtab = &e->shdr[e->ehdr->e_shstrndx];
    const char *const sh_strtab_p = e->addr + sh_strtab->sh_offset;
    printf("INDEX NAME   ADDRESS   OFFSET   SIZE   TYPE\n");
    for (int i = 0; i < e->ehdr->e_shnum; ++i)
    {
        printf("[%d] '%s' %4d %d %d %s\n", i, sh_strtab_p + e->shdr[i].sh_name, e->shdr[i].sh_addr, e->shdr[i].sh_offset, e->shdr[i].sh_size, getSectionType(e->shdr[i].sh_type));
    }
}
void printSymbols(elf *e)
{
    Elf32_Sym *symtab;
    char *strtab;
    Elf32_Shdr *sh_strtab = &e->shdr[e->ehdr->e_shstrndx];
    int num_of_symbols = 0;

    char *sh_strtab_p = e->addr + sh_strtab->sh_offset;
    for (int i = 0; i < e->ehdr->e_shnum; ++i)
    {
        //  printf("[%d]\n", i);
        if (strcmp(sh_strtab_p + e->shdr[i].sh_name, ".symtab") == 0)
        {
            printf("Found SymTable\n");
            symtab = (Elf32_Sym *)(e->addr + e->shdr[i].sh_offset);
            num_of_symbols = e->shdr[i].sh_size / sizeof(Elf32_Sym);
        }
        if (strcmp(sh_strtab_p + e->shdr[i].sh_name, ".strtab") == 0)
        {
            strtab = (char *)(e->addr + e->shdr[i].sh_offset);
        }
    }
    printf("INDEX\tVALUE\tSECTION INDEX\tSECTION NAME\tSYMBOL NAME\n");
    for (size_t i = 0; i < num_of_symbols; i++)
    {
        char *index = symtab[i].st_shndx == SHN_UNDEF ? "UND" : symtab[i].st_shndx == SHN_ABS  ? "ABS"
                                                            : symtab[i].st_shndx == SHN_COMMON ? "COM"
                                                                                               : "";
        Elf32_Shdr *temp =(Elf32_Shdr *)( e->addr+e->ehdr->e_shoff);
        if (strcmp(index, "") == 0)
        {
            printf("[%d]\t%x\t%d\t\t%s\t\t%s\n", i, symtab[i].st_value, symtab[i].st_shndx, sh_strtab_p+temp[symtab[i].st_shndx].sh_name, strtab + symtab[i].st_name);
        }
        else
        {
            printf("[%d]\t%x\t%s\t\t%s\t\t%s\n", i, symtab[i].st_value, index, index, strtab + symtab[i].st_name);
        }
    }
}
void relocationTables(elf *e)
{
    printf("not implemented\n");
}

int main(int argc, char **argv)
{
    elf *my_elf = (elf *)malloc(sizeof(my_elf));

    struct fun_desc menu[] = {{"Toggle Debug Mode", debug}, {"Examine ELF File", examine}, {"Print Section Names", printSections}, {"Print Symbols", printSymbols}, {"Relocation Tables", relocationTables}, {"Quit", quit}, {NULL, NULL}};

    int upperBound = sizeof(menu) / 8 - 1; // /8 for size ofstruct (2 pointers) -1 for the null at the end
    int option;
    printf("myELF:\n");
    do
    {
        printf("\nPlease choose a function:\n");
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