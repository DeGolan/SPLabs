#include <stdio.h>

int main(int argc, char **argv) {
    /*
    for(int i=1;i<argc;i++){
        printf("%s ",argv[i]);
    }*/
    /*
    const short bufsize=254;
    char buf[254];
    fgets(buf,bufsize,stdin);
     printf("%s",buf);
    */
   
   char inputC;
    do
    {
        inputC=fgetc(stdin);
        if(feof(stdin))
            break;           
        printf("%c",inputC);

    } while(1);

    return 0;
}