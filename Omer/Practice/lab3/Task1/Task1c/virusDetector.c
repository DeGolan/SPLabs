#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

char *file_name;

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
void list_print(link *virus_list, FILE* output){
    link*tmp=virus_list;
    while (tmp!=NULL){
        printVirus(tmp->vir,output);
        tmp=tmp->nextVirus;  
    }   
}

link* list_append(link* virus_list, virus* data){

    link*tmp=virus_list;
    link *new_link=(link*)malloc(sizeof(link));
    if(virus_list==NULL){
        new_link->nextVirus=NULL;
        new_link->vir=data;
        return new_link;
    }
    while (tmp->nextVirus!=NULL){
        tmp=tmp->nextVirus;
    }
    new_link->nextVirus=NULL;
    new_link->vir=data;
    tmp->nextVirus=new_link;
    
    return virus_list; 
}
void list_free(link *virus_list){
    link*tmp=virus_list;
    while (tmp!=NULL){
        link* tmp2=tmp;
        tmp=tmp->nextVirus;
        free(tmp2->vir->sig);
        free(tmp2->vir);
        free(tmp2);
    } 
}
struct fun_desc {
  char *name;
  link* (*fun)(link*);
};
link* load_signatures(link* virus_list){
  
    char *path = NULL;
    int read;
    unsigned int len;
    fprintf(stdout, "Enter file path:\n");  
    read = getline(&path, &len, stdin);
    if (-1 == read)
        printf("No line read...\n");
    path[strlen(path)-1] = '\0';
    FILE *input = fopen(path, "rb");
   

    if (input == NULL){
        fputs("File error\n", stderr);
        fclose(input);
        exit(1);
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
        virus_list=list_append(virus_list,virus);  
    }
    free(path);
    free(header);
    fclose(input);
    
    return virus_list;
}
link* print_signatures(link* virus_list){
    printf("Printing\n");
    list_print(virus_list,stdout);
    return virus_list;
}
link* detect_viruses(char* buffer, unsigned int size, link *virus_list,FILE* output){
    int counter=1;
    link*tmp=virus_list;
    while (tmp!=NULL){
        //action
        for (int i = 0; i <= size-tmp->vir->SigSize; i++){
            if(!memcmp(buffer+i,tmp->vir->sig,tmp->vir->SigSize)){
                printf("The Starting byte loaction: %d\n",i);
                printf("The Virus name: %s\n",tmp->vir->virusName);
                printf("The size of the virus: %d\n\n",tmp->vir->SigSize);
            }
        }
        tmp=tmp->nextVirus;
        
    }  
    return virus_list;
 
}
link* detect_viruses_1(link* virus_list){

    FILE * input;
    input=fopen(file_name,"r");
    char *buffer=(char*)malloc(sizeof(char)*1000);
    //file size
    fseek (input , 0 , SEEK_END);
    int length = ftell (input);
    rewind (input);
    fread(buffer,1,10000,input);

    if(length>10000){
        length=10000;
    }
    virus_list=detect_viruses(buffer,length,virus_list,stdout);
    free(buffer);
    fclose(input);
    return virus_list;
}

int main(int argc, char **argv) {
    file_name=argv[1];
    link* virus_list=NULL;
    struct fun_desc menu[]= {{"Load signatures",load_signatures},{"Print signatures",print_signatures},{"Detect viruses",detect_viruses_1},{NULL,NULL}};
    int bound=sizeof(menu)/8-1;
    int option;

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
            virus_list=menu[option].fun(virus_list);
            puts("DONE.\n");
        }
        else{
            break;
        }
    }while(1);
    
    puts("Not within bounds");
    list_free(virus_list);
    

    return 0;

}
