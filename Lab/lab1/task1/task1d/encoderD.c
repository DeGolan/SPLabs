#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE* input = stdin;
    FILE* output = stdout;
    char before,after;
    int debug = 0,letters = 0,sign = 0,key = 0;

    for(int i=1 ; i < argc ; i++){//receiving arguments
        if(strcmp(argv[i],"-D") == 0){//debug mode
            debug = 1;
            fprintf(stderr,"-D\n");
        }
        else if(((argv[i])[0] == '-' ||(argv[i])[0] == '+') && (argv[i])[1] == 'e'&& (argv[i])[3] == 0 &&
         (((argv[i])[2] >= '0' && (argv[i])[2] <= '9') || ((argv[i])[2] >= 'A' && (argv[i])[2] <= 'F'))){ //encryption mode
            sign = ((argv[i])[0] == '-') ? -1 : 1;
            key = ((argv[i])[2] >= '0' && (argv[i])[2] <= '9') ? (argv[i])[2] - 48 : (argv[i])[2] - 55; 
        }
        else if(strncmp(argv[i],"-i",2) == 0){ //read from file
            input = fopen(argv[i]+2,"r");
            if(input == NULL){ 
                fprintf(stderr,"File not found\n");
                return 1;
            }
        } 
        else{//throw err if the parameter is invalid
            fprintf(stderr,"invalid parameter - %s\n",argv[i]);
	        return 1;
        }  
    }
    do {//encoder main loop
        before = after = fgetc(input);//receving char from input
        if((before == EOF || before == 10) && debug){//debug mode
            fprintf(stderr,"\nthe number of letters: %i\n\n",letters);
            letters=0;
        }
        if(before == EOF){//if eof break the loop
            break;
        }
        if(before != 10) {//check new line
            if(sign){//encryption mode
                after = before + (sign * key);
                letters++;
            }
            else if(before >= 'A' && before <= 'Z'){//to lowercase
                after = before + ('a' - 'A');
                letters++;
            }   
            if(debug){//debug mode
            fprintf(stderr,"%i %i\n", before,after);    
            } 
        }
        fputc(after,output);
    } while(1);
}