#include <stdio.h>

int main(int argc, char **argv) {
    FILE* input=stdin;
    FILE* output=stdout;
    char before,after;
    do {
        before=after=fgetc(input);
        if(before==EOF){
            break;
        }
        if(before >= 'A' && before <= 'Z'){
            after=before+('a'-'A');
        }
        fputc(after,output);
    } while (1);
}