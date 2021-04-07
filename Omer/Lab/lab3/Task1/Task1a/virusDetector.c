#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

void PrintHex(char* buffer,int length,FILE* output){
    for (int i = 0; i < length; i++){
        if(i%22==0 && i>0){
            fprintf(output,"\n");
        }
        fprintf(output,"%02X ",buffer[i] &0Xff);
        
    }
    fprintf(output,"\n\n");
}

virus* readVirus(FILE* input){
    virus* newVirus=(virus*)calloc (sizeof(virus),1);
    fread(newVirus,1,18,input);
    newVirus->sig=(unsigned char*)calloc (sizeof(unsigned char*)*(newVirus->SigSize),1);
    fread(newVirus->sig,1,newVirus->SigSize,input);
    return newVirus;
}

void printVirus(virus* virus, FILE* output){ 
    fprintf(output,"Virus Name: %s",virus->virusName);  
    fprintf(output,"\nVirus Size: %d\nSignature:\n",virus->SigSize);
    PrintHex(virus->sig,virus->SigSize,output);
}


int main(int argc, char **argv) {
    FILE * input=stdin;
    char *buffer;
    
    input=fopen(argv[1],"r");
    if(input==NULL){ 
        fprintf(stderr,"File not found\n");
        exit (1);
    }

    char* header=(char*)malloc(sizeof(char)*4);
    fread(header,1,4,input);
    
    while (1){
        virus *virus = readVirus(input);
        if (feof(input)){
            free(virus->sig);
            free(virus);
            break;
        }
        printVirus(virus, stdout);
        free(virus->sig);
        free(virus);
    }
    
    free(header);
    fclose(input);
    return 0;

 }
