#include "util.h"
#define _GNU_SOURCE
#include <dirent.h>

#define SYS_WRITE 4
#define STDOUT 1
#define SYS_READ 3
#define SYS_OPEN 5
#define STDIN 0
#define STDERR 2
#define SYS_GETDENTS 141
#define SYS_CLOSE 6

extern int system_call();

typedef struct ent
{
    int inode;  /* Inode number */
    int offset; /* Offset to next linux_dirent */
    short len;
    char buf[1]; /* Filename (null-terminated) */

} ent;

int main(int argc, char **argv)
{
    char prefix;
    int p = 0;
    int j = 1;
    while (j < argc)
    {
        if (strlen(argv[j]) == 3 && strncmp(argv[j], "-p", 2) == 0)
        {
            prefix = *(argv[j] + 2);
            p = 1;
        }
        else
        {
            system_call(1, 0x55);
        }
        j++;
    }
    char d_type;
    char *d_type_string;
    int fd = system_call(SYS_OPEN, ".", 0, 0777);
    char buf[8192];
    ent *entp;
    int i = 0;
    int nread = system_call(SYS_GETDENTS, fd, buf, 8192);
    if (fd == -1 || nread == -1)
    {
        system_call(1, 0x55);
    }
    while (i < nread)
    {
        entp = (ent *)(buf + i);
        d_type = *(buf + i + (entp->len) - 1);
        if (d_type != DT_DIR)
        {
            if ((p && entp->buf[0] == prefix) || !p)
            {
                system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));
                system_call(SYS_WRITE, STDOUT, " ", 1);
                d_type_string = (d_type == DT_REG) ? "regular" : (d_type == DT_DIR) ? "directory"
                                                             : (d_type == DT_FIFO)  ? "FIFO"
                                                             : (d_type == DT_SOCK)  ? "socket"
                                                             : (d_type == DT_LNK)   ? "symlink"
                                                             : (d_type == DT_BLK)   ? "block dev"
                                                             : (d_type == DT_CHR)   ? "char dev"
                                                                                    : "???";
                system_call(SYS_WRITE, STDOUT, d_type_string, strlen(d_type_string));
                system_call(SYS_WRITE, STDOUT, "\n", 1);
            }
        }

        i += entp->len;
    }
    system_call(SYS_CLOSE, fd);

    return 0;
}
