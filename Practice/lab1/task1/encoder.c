#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

   
    FILE * input=stdin;
    FILE * output=stdout;
    char inputC;
    int debug=0,letters=0,sign=0,key=0;
   
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-D")==0){ //debug mode
            debug=1;
            fprintf(stderr,"-D\n");
        }
        else if((argv[i])[1]=='e'&& (argv[i])[3]==0){ //encryption mode
            if((argv[i])[0]=='-'){
                sign=-1;
            }
            else if((argv[i])[0]=='+'){
                sign=1;
            }
            if((argv[i])[2]>='0' && (argv[i])[2]<='9'){
                key=(argv[i])[2]-48;
            }
            else if((argv[i])[2]>='A' && (argv[i])[2]<='F'){
                key=(argv[i])[2]-55;             
            }
        }
        
        if((argv[i])[0]=='-' && (argv[i])[1]=='i'){ //read from file
            input=fopen(argv[i]+2,"r");
            if(input==NULL){ 
                fprintf(stderr,"File not found\n");
                return 1;
            }           
        } 
         if((argv[i])[0]=='-' && (argv[i])[1]=='o'){ //write to file
            output=fopen(argv[i]+2,"w");
         }
    }  
    do
    {
        inputC=fgetc(input);
        if(feof(input))
            break;
        if(inputC!=10){
            if(debug){
             fprintf(stderr,"%i ", inputC);
            }
            if(sign){
                inputC=inputC+(sign*key);
            }
            else if(inputC >= 'A' && inputC <= 'Z'){
                letters++;
                inputC=inputC+32;
            }
            if(debug){
                fprintf(stderr,"%i\n", inputC);
            }     
        }
        else if(debug){
            fprintf(stderr,"\nthe number of letters: %i\n\n",letters);
            letters=0;

        }
        fprintf(output,"%c",inputC);
        fflush(output);
        

    } while(1);
    if(input!=stdin)
         fclose(input);
    if(output!=stdout)
         fclose(output);
   
    return 0;

 }
