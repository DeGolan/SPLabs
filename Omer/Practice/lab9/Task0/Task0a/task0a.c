#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
// #include <libelf.h>
#include <elf.h>
#define NAME_LEN 128

int Currentfd = -1;

typedef struct
{
    char debug_mode;
    char file_name[NAME_LEN];
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    Elf32_Phdr *phdr;
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
    free(e->ehdr);
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
    e->phdr=(Elf32_Phdr *)(e->addr+e->ehdr->e_phoff);
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
void printProgramHeader(elf *e)
{
    //const char *const ph_strtab_p = e->addr + e->phdr->p_offset;
    printf("TYPE\tOffset\tVirAddr\tPhysAddr\tFileSiz\tMemSiz\tFlg\tAlign\n");
    for (int i = 0; i < e->ehdr->e_phnum; i++)
    {
        printf("%d\t%x\t%x\t%x\t%x\t%x\t%d\t%x\n",e->phdr[i].p_type,e->phdr[i].p_offset,e->phdr[i].p_vaddr,
        e->phdr[i].p_paddr,e->phdr[i].p_filesz,e->phdr[i].p_memsz,e->phdr[i].p_flags,e->phdr[i].p_align);
    }
}


int main(int argc, char **argv)
{
    elf *my_elf = (elf *)malloc(sizeof(my_elf));
    examine(my_elf);
    printProgramHeader(my_elf);
    free(my_elf);
    return 0;
}