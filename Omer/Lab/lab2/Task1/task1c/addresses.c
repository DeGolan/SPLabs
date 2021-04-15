#include <stdio.h>

int main (int argc, char** argv){
    int iarray[3]; //4 bytes
    float farray[3]; //4 bytes
    double darray[3]; //8 bytes
    char carray[3]; //1 byte
    int* iptrarray[3]; //4 byte
    char* cptrarray[3];//4 byte

    printf("iarray: %p\n",iarray);
    printf("iarray+1: %p\n",iarray+1);

    printf("farray: %p\n",farray);
    printf("farray+1: %p\n",farray+1);

    printf("darray: %p\n",darray);
    printf("darray+1: %p\n",darray+1);
    printf("carray: %p\n",carray);
    printf("carray+1: %p\n",carray+1);
    printf("iptrarray: %p\n",iptrarray);
    printf("iptrarray+1: %p\n",iptrarray+1);
    printf("cptrarray: %p\n",cptrarray);
    printf("cptrarray+1: %p\n",cptrarray+1);

    return 0;
}