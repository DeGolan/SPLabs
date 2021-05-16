#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define  NAME_LEN  128
#define  BUF_SZ    10000

typedef struct {
  char debug_mode;
  char file_name[NAME_LEN];
  int unit_size;
  unsigned char mem_buf[BUF_SZ];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

struct fun_desc{
  char *name;
  void (*fun)(state*);
};

void toggleDebugMode(state* s){
  if(s->debug_mode){
    fprintf(stderr,"Debug flag now off\n");
    s->debug_mode=0;
  }
  else{
    fprintf(stderr,"Debug flag now on\n");
     s->debug_mode=1;
  }
}

void setFileName(state *s){
  printf("Enter File Name: ");
  scanf("%s", s->file_name);
  if(s->debug_mode){
    fprintf(stderr,"Debug: file name set to %s\n",s->file_name);
  }
}

void setUnitSize(state* s){
  int size;
  printf("Enter Unit Size: ");
  scanf("%d", &size);
  if(size==1||size==2||size==4){
    if(s->debug_mode){
      fprintf(stderr,"Debug: set size to %d\n",size);
    }
    s->unit_size=size;
  }
  else{
    fprintf(stderr,"error: size needs to be one of the following (1,2,4)\n");
  }
}

void quit(state* s){
  free(s);
  exit(0);
}

int hexToDec(char* hexNumber){
  int decNumber=0,base=1,i=0,length=strlen(hexNumber);
  for(i=length--;i>=0;i--){

     if(hexNumber[i] >= '0' && hexNumber[i] <= '9')
        {
            decNumber += (hexNumber[i] - 48) * base;
            base *= 16;
        }
        else if(hexNumber[i] >= 'A' && hexNumber[i] <= 'F')
        {
            decNumber += (hexNumber[i] - 55) * base;
            base *= 16;
        }
        else if(hexNumber[i] >= 'a' && hexNumber[i] <= 'f')
        {
            decNumber += (hexNumber[i] - 87) * base;
            base *= 16;
        }
    }
    return decNumber;
}

void loadIntoMemory(state *s){
  if(!strcmp(s->file_name,"")){
    fprintf(stderr,"Error: file_name is empy");
    return;
  }
  int fd=open(s->file_name,O_RDONLY);
  if(fd==-1){
    perror("Error");
    return;
  }
  printf("Enter <location> and <length>\n");
  char location[100];
  int length;
  scanf("%s %d",location,&length);
  if(s->debug_mode){
    fprintf(stderr,"location: %s, length: %d\n",location,length);
  }
  int location_as_number=hexToDec(location);
  lseek(fd,location_as_number,SEEK_SET);//going to the location in the file
  read(fd,s->mem_buf,s->unit_size*length);//reading length*unit
  printf("Load %d units into memory\n",length);
  close(fd);
}

void memoryDisplay(state *s){
  printf("Enter <address> and <length>\n");
  char address[100];
  int length;
  scanf("%s %d",address,&length);
  if(s->debug_mode){
    fprintf(stderr,"address: %s, length: %d\n",address,length);
  }
  int address_as_number=hexToDec(address);
  if(address_as_number==0){
    printf("Hexdecimal\n==========");
    for(int i=0;i<length;i++){
      printf("%x ",s->mem_buf[i]);
      if(i%10==0){
        fputc(10,stdout);
      }
    }
    
  }
}

void dummy(state* s){
}

struct fun_desc menu[] = {{"Toggle Debug Mode", toggleDebugMode}, {"Set File Name", setFileName}, {"Set Unit Size", setUnitSize},
 {"Load Into Memory", loadIntoMemory}, {"Memory Display", memoryDisplay}, {"Save Into File", dummy},
  {"Memory Modify", dummy}, {"Quit", quit}, {NULL, NULL}};

int main(int argc, char **argv)
{
  int bound = sizeof(menu) / 8 - 1;
  int option;
  state* my_state=(state *)malloc(sizeof(my_state));
  do
  {
    fputs("Please choose a function:\n", stdout);
    for (int i = 0; menu[i].name != NULL; i++){
      fprintf(stdout, "%d) %s\n", i, menu[i].name);
    }
    if(option>=0 && option<bound){
      menu[option].fun(my_state);
    }
    printf("Option: ");
    scanf("%d", &option);
  } while (1);
  puts("Not within bounds");
  free(my_state);
  return 0;
}
