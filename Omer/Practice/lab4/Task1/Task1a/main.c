#include "util.h"

#define SYS_OPEN 5
#define STS_CLOSE 6
#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_LSEEK 19

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_RDRW 2

extern int system_call();


int main (int argc , char* argv[], char* envp[])
{
  char before;
  char after;
  do {
      if(system_call(SYS_READ,STDIN,&after,1)<0){
          system_call(1,0x55);
      }
      before=after;
      if(before >= 'A' && before <= 'Z'){
          after=before+('a'-'A');
      }
  
      system_call(SYS_WRITE,STDOUT,&after,1);
  } while(1);


  return 0;
}
