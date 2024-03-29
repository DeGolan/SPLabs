﻿#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5;//Uninitialized data segment, often called the "bss" segment
int addr6;//Uninitialized data segment, often called the "bss" segment

int foo();
void point_at(void *p); 
void foo1(); 
void foo2(); 

int main (int argc, char** argv){
    int addr2; //stack
    int addr3; //stack
    char* yos="ree";
    int * addr4 = (int*)(malloc(50));
    printf("- &addr2: %p\n",&addr2);
    printf("- &addr3: %p\n",&addr3);
    printf("- foo: %p\n",foo);
    printf("- &addr5: %p\n",&addr5);
    
	point_at(&addr5);
	
    printf("- &addr6: %p\n",&addr6);
    printf("- yos: %p\n",yos);
    printf("- addr4: %p\n",addr4);
    printf("- &addr4: %p\n",&addr4);
    
    printf("- &foo1: %p\n" ,&foo1);
    printf("- &foo1: %p\n" ,&foo2);
    printf("- &foo2 - &foo1: %ld\n" ,&foo2 - &foo1);
    return 0;
}

int foo(){
    return -1;
}

void point_at(void *p){
    int local; //stack dif activation frame?
	static int addr0 = 2; //init value?
    static int addr1;     //Uninitialized data segment, often called the "bss" segment


    long dist1 = (size_t)&addr6 - (size_t)p; //4 bytes different (same area)
    long dist2 = (size_t)&local - (size_t)p; //different area
    long dist3 = (size_t)&foo - (size_t)p;   //different area
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p\n", & addr0);
    printf("- addr1: %p\n",&addr1);
}

void foo1 (){    
    printf("foo1\n"); 
}

void foo2 (){    
    printf("foo2\n");    
}
