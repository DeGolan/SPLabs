#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {
FILE * input=stdin;

char inChar;

do
{
    inChar=fgetc(input);
     if(feof(input))
        break;
    if(inChar >= 'A' && inChar <= 'Z'){
        inChar=inChar+32;
    }
    printf("%c",inChar);

} while(1);

fclose(input);

return 0;
}
