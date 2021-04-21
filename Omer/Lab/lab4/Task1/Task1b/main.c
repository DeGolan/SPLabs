#include "util.h"

#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_LSEEK 19
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_WRONLY 1
#define O_RDONLY 0
#define O_RDRW 2
#define O_CREAT 64
extern int system_call();


int main (int argc , char* argv[], char* envp[])
{
    int input=STDIN;
    int output=STDOUT;
    int i=1;
    while(i < argc){ 
        if(strncmp(argv[i],"-i",2) == 0){ 
            input = system_call(SYS_OPEN,argv[i]+2,O_RDONLY,0777);
            if(input <0){ 
                system_call(1,0x55); 
            }
        } 
        else if(strncmp(argv[i],"-o",2) == 0){ 
            output = system_call(SYS_OPEN,argv[i]+2,O_CREAT,0777);
            output = system_call(SYS_OPEN,argv[i]+2,O_WRONLY,0777);
        }
        else{
	        system_call(1,0x55);
        }  
        i++;
    }

    char before;
    char after;
    do {
      if(system_call(SYS_READ,input,&after,1)<=0){
          system_call(1,0x55);
      }
      before=after;
      if(before >= 'A' && before <= 'Z'){
          after=before+('a'-'A');
      }
  
      system_call(SYS_WRITE,output,&after,1);
    } while(1);
    if(input!=STDIN){
        system_call(SYS_CLOSE,input);
    }
    if(output!=STDOUT){
        system_call(SYS_CLOSE,output);
    }

  return 0;
}
