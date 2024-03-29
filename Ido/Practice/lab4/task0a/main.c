#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1
extern void system_call(int op,int loc,char* str,unsigned int length);

int main (int argc , char* argv[], char* envp[])
{
  int i;
  char * str;
  str = itoa(argc);
  system_call(SYS_WRITE,STDOUT, str,strlen(str));
  system_call(SYS_WRITE,STDOUT," arguments \n", 12);
  for (i = 0 ; i < argc ; i++)
    {
      system_call(SYS_WRITE,STDOUT,"argv[", 5);
	  str = itoa(i);
      system_call(SYS_WRITE,STDOUT,str,strlen(str));
      system_call(SYS_WRITE,STDOUT,"] = ",4);
      system_call(SYS_WRITE,STDOUT,argv[i],strlen(argv[i]));
      system_call(SYS_WRITE,STDOUT,"\n",1);
    }
  system_call(SYS_WRITE,STDOUT,"hello world\n",3);
  return 0;
}
