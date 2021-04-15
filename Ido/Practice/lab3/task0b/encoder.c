#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void PrintHex(char * buffer,int length){
    for (size_t i = 0; i < length; i++)
    {
        printf("%X ", buffer[i] & 0xff);
    }

}

int main(int argc, char **argv) {
    char* buffer;
    FILE * input=fopen(argv[1],"r");
    if (input==NULL) {fputs ("File error",stderr); exit (1);}

    fseek (input , 0 , SEEK_END);
    long lSize = ftell (input);
    rewind (input);

    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) 
        {fputs ("Memory error",stderr); exit (2);}

    fread(buffer,1,lSize,input);

    PrintHex(buffer,lSize);

    free(buffer);
    fclose(input);
        
    return 0;

 }
