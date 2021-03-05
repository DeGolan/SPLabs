#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

    //FILE * input=stdin;
    //input=fopen(argv[1],"r");

   char inputC;
   int d=0;
   
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-D")==0){
            d=1;
            printf("-D\n");
        }
        else{
            printf("invalid parameter - %s\n",argv[i]);
            return 1;
        }
    }  
    do
    {
        inputC=fgetc(stdin);
        if(feof(stdin))
            break;
        if(inputC >= 'A' && inputC <= 'Z'){
            inputC=inputC+32;
        }
        printf("%c",inputC);

    } while(1);

    //fclose(input);
    return 0;

 }
