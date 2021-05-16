#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

struct fun_desc
{
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
void dummy(state* s){
}

struct fun_desc menu[] = {{"Toggle Debug Mode", toggleDebugMode}, {"Set File Name", setFileName}, {"Set Unit Size", setUnitSize},
 {"Load Into Memory", dummy}, {"Memory Display", dummy}, {"Save Into File", dummy},
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
