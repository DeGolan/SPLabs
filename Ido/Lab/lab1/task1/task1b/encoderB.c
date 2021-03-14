#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE* input=stdin;
    FILE* output=stdout;
    char before,after;
    int debug=0,letters=0;

    for(int i=1 ; i < argc ; i++){//receiving arguments
        if(strcmp(argv[i],"-D") == 0){//debug mode
            debug=1;
            fprintf(stderr,"-D\n");
        }
    }
    do {//encoder main loop
        before=after=fgetc(input);//receving char from input
        if((before==EOF ||before==10) && debug){//debug mode
            fprintf(stderr,"\nthe number of letters: %i\n\n",letters);
            letters=0;
        }
        if(before==EOF){//if eof break the loop
            break;
        }
        if(before != 10) {//check new line
            if(before >= 'A' && before <= 'Z'){//to lowercase
                after=before+('a'-'A');
                letters++;
            }   
            if(debug){//debug mode
            fprintf(stderr,"%i %i\n", before,after);    
            } 
        }
        fputc(after,output);
    } while (1);
}