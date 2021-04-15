#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintHex(char* buffer,int length){
    for (int i = 0; i < length; i++){
        printf("%X ",buffer[i] &0Xff);
    }
}
int main(int argc, char **argv) {
    FILE * input=stdin;
    char *buffer;
    
    input=fopen(argv[1],"r");
    if(input==NULL){ 
        fprintf(stderr,"File not found\n");
        exit (1);
    }
    //get the file size
    fseek (input , 0 , SEEK_END);
    int length = ftell (input);
    rewind (input);
    //read and print
    buffer=(char*)malloc (sizeof(char)*length);
    fread(buffer,1,length,input);
    PrintHex(buffer,length);
   
    //mem free
    free(buffer);
    fclose(input);
    return 0;

 }
