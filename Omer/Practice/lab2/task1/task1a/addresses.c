#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5;
int addr6;

int foo();
void point_at(void *p);
void foo1();
void foo2();

int main (int argc, char** argv){
    int addr2;
    int addr3;
    char* yos="ree";
    int * addr4 = (int*)(malloc(50));
    printf("- &addr2: %p\n",&addr2); //stack
    printf("- &addr3: %p\n",&addr3); //stack
    printf("- foo: %p\n",foo);       //Uninitialized data segment, often called the "bss" segment
    printf("- &addr5: %p\n",&addr5); //Uninitialized data segment, often called the "bss" segment
    
	point_at(&addr5);
	
    printf("- &addr6: %p\n",&addr6); ////Uninitialized data segment, often called the "bss" segment
    printf("- yos: %p\n",yos); 
    printf("- addr4: %p\n",addr4);
    printf("- &addr4: %p\n",&addr4);
    
    printf("- &foo1: %p\n" ,&foo1);
    printf("- &foo1: %p\n" ,&foo2);
    //printf("- &foo2 - &foo1: %ld\n" ,&foo2 - &foo1);//
    return 0;
}

int foo(){
    return -1;
}

void point_at(void *p){
    int local;
	static int addr0 = 2;
    static int addr1;


    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1); //same area - Uninitialized data segment, often called the "bss" segment 4 bytes dif
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2); //loacl on stack, p on uninitialized data segment
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p\n", & addr0); //stack
    printf("- addr1: %p\n",&addr1); //stack
}

void foo1 (){    
    printf("foo1\n"); 
}

void foo2 (){    
    printf("foo2\n");    
}
