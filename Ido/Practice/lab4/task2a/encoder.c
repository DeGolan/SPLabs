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
    char d_type;
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
            system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));
            system_call(SYS_WRITE, STDOUT, "\n", 1);
        }

        i += entp->len;
    }
    system_call(SYS_CLOSE, fd);

    return 0;
}
