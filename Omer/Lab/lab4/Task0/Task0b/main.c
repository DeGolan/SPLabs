#include "util.h"

#define SYS_OPEN 5
#define STS_CLOSE 6
#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_LSEEK 19
#define STDOUT 1
#define O_RDRW 2

extern int system_call();


int main (int argc , char* argv[], char* envp[])
{
  char* pathName=argv[1];
  int input = system_call(SYS_OPEN,pathName,O_RDRW,0777);
  system_call(SYS_LSEEK,input,657,0);
  char* mira=argv[2];
  system_call(SYS_WRITE,input,mira,strlen(mira)+1);
 if(input<0){
   system_call(1, 0x55);
 }
  return 0;
}
