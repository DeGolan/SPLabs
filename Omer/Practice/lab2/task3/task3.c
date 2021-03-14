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
 
int main(int argc, char **argv){

  char* carry=(char*)(malloc(sizeof(char)*5));
  fprintf(stdout,"s%",carry);

  return 0;  
}