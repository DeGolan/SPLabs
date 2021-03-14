#include <stdio.h>

int main (int argc, char** argv){
    int iarray[3]; //4 bytes
    float farray[3]; //4 bytes
    double darray[3]; //8 bytes
    char carray[3]; //1 byte
    int* iptrarray[3]; //4 byte
    char* cptrarray[3];//4 byte

    printf("iarray: %x\n",iarray);
    printf("iarray+1: %x\n",iarray+1);

    printf("farray: %x\n",farray);
    printf("farray+1: %x\n",farray+1);

    printf("darray: %x\n",darray);
    printf("darray+1: %x\n",darray+1);
    printf("carray: %x\n",carray);
    printf("carray+1: %x\n",carray+1);
    printf("iptrarray: %x\n",iptrarray);
    printf("iptrarray+1: %x\n",iptrarray+1);
    printf("cptrarray: %x\n",cptrarray);
    printf("cptrarray+1: %x\n",cptrarray+1);

    return 0;
}