#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
 char encrypt(char c){
    if(c>0x20 && c<0x7E){
        c=c+3;
    }
    return c;
 }

 char decrypt(char c){ 
    if(c>0x20 && c<0x7E){
        c=c-3;
    }
    return c;
 }

 char cprt(char c){ 
    if(c>0x20 && c<0x7E){
        fprintf(stdout,"%c\n",c);
    }
    else{
        fprintf(stdout,".\n");
    }
    return c;

 }

char my_get(char c){
    return fgetc(stdin);
}

char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for(int i=0; i<array_length;i++){
      mapped_array[i]=f(array[i]);
  }
  return mapped_array;
}
struct fun_desc {
  char *name;
  char (*fun)(char);
};

struct fun_desc menu[]= {{"Censor",censor},{"Encrypt",encrypt},{"Decrypt",decrypt},{"Print string",cprt},{"Get string",my_get},{NULL,NULL}};

int main(int argc, char **argv){
  int bound=sizeof(menu)/8-1;
  int option;
  char* carray=(char*)(malloc(sizeof(char)*5));
  do
  {
    fputs("Please choose a function:\n",stdout);
    for (int i = 0; menu[i].name!=NULL; i++){
      fprintf(stdout,"%d) %s\n",i,menu[i].name);
    }
    printf("Option: ");
    scanf("%d",&option);
    if(option>=0 && option<bound){
      puts("Within bounds");
      __fpurge(stdin);
      carray = map(carray, 5, menu[option].fun);
      puts("DONE.\n");
    }
    else{
      break;
    }
  } while (1);

  puts("Not within bounds");
  free(carray);

  return 0;
  
}
