#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
 char encrypt(char c){ /* Gets a char c and returns its encrypted form by adding 3 to its value. 
                        If c is not between 0x20 and 0x7E it is returned unchanged */
    if(c>0x20 && c<0x7E){
        c=c+3;
    }
    return c;
 }

 char decrypt(char c){ /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
                        If c is not between 0x20 and 0x7E it is returned unchanged */
    if(c>0x20 && c<0x7E){
        c=c-3;
    }
    return c;
 }

 char cprt(char c){ /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
    if(c>0x20 && c<0x7E){
        fprintf(stdout,"%c\n",c);
    }
    else{
        fprintf(stdout,".\n");
    }
    return c;

 }

char my_get(char c){ /* Ignores c, reads and returns a character from stdin using fgetc. */
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
 
int main(int argc, char **argv){
  struct fun_desc menu[]= {{"Censor",censor},{"Encrypt",encrypt},{"Decrypt",decrypt},{"Print string",cprt},{"Get string",my_get},{NULL,NULL}};
  int bound=sizeof(menu)/8-1;
  int option;
  int toFree=0;
  char temp[5]="";
  char *carray=temp;
  char *temp2;

  do{
    puts("Pleas choose a function:");
    for(int i=0; menu[i].name != NULL ; i++){
      printf("%d) %s\n",i,menu[i].name);
    }
    printf("Option: ");
    scanf("%d",&option);
    if(option >= 0 && option < bound){
      printf("Within bounds\n");
      __fpurge(stdin); 
      temp2=carray;
      carray = map(carray, 5, menu[option].fun);   
      if(toFree){
        free(temp2);
      }
      toFree=1;
      puts("DONE.\n");

    }
    else{
      break;
    }
  }while(1);
  
  puts("Not within bounds");
  if(toFree){
    free(carray);
  }
  
  exit(1);
}