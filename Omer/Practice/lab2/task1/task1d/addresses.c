#include <stdio.h>

int main (int argc, char** argv){
    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr;
    char* carrayPtr;
    int *p; //garbage value
    iarrayPtr=&iarray[0];
    carrayPtr=&carray[0];

    for(int i=0;i<3 ; i++){
        printf("%d ",*(iarrayPtr+i));
    }
     for(int i=0;i<3 ; i++){
        printf("%c ",*(carrayPtr+i));
    }
    printf("\n%d",p);
    return 0;
}