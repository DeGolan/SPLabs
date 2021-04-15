#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char **argv) {
FILE * input=stdin;
char inChar;
bool debug= false;
int numOfChangedChars=0,i;
char tempChar;

for(i=1; i<argc; i++){
    if(strcmp(argv[i],"-D")==0){
        debug=true;
        printf("%s\n",argv[1]);
    }
}
do
{
    inChar=fgetc(input);
    if((debug)&&((inChar==EOF)||inChar==10)){
        fprintf(stderr,"number of changed letters= %i\n",numOfChangedChars);
        numOfChangedChars=0;
    }
    if(feof(input)){
        break;
    }
    tempChar=inChar;
    if(inChar >= 'A' && inChar <= 'Z'){
        inChar=inChar+32;
        numOfChangedChars++;
    }
    if(debug&&inChar!=10){
        fprintf(stderr,"%d %d\n",inChar,tempChar);

    }

    printf("%c",inChar);

} while(1);

fclose(input);

return 0;
}
