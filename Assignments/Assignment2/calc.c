#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
// A structure to represent a stack
typedef struct Stack
{
    int top;
    unsigned capacity;
    int *array;
} Stack;

// function to create a stack of given capacity. It initializes size of
// stack as 0
struct Stack *createStack(unsigned capacity)
{
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));
    return stack;
=======
char* toShorterStr(char* str,int size,int leadingZeroes){
    //printf("SIZE: %d\n",size);
    char* newResult=NULL;
    if(size==0){
        newResult=(char*)malloc(2);
        newResult[0]='0';
        newResult[1]=0;
    }
    else{
        newResult=(char*)malloc(size+1);
        for(int i=0;i<size;i++){
            newResult[i]=str[i+leadingZeroes];
        }
    }
    free(str);
    return newResult;

>>>>>>> 9a2d4b87f451ea326b8e24e621144cc5b43a051d
}

char* octOperation(char* num1,char* num2,char* op){
    int size1,size2,maxSize;
    //get the size of each number
    for(size1=0; num1[size1]!=0;size1++);
    //printf("Size of first number: %d\n",size1);
    
    for(size2=0; num2[size2]!=0;size2++);
    //printf("Size of second number: %d\n",size2);
    
    maxSize=(size1>size2)?size1:size2;
    int resultSize=maxSize;

    size1-=1;//index to the msb
    size2-=1;//index to the msb
    

    char* result=(char*)malloc(maxSize+2);//plus one for carry, plus another one for null
    result[maxSize+1]=0;//NULL TERMINATED STRING

    //result[0]=0;//to know if we used the extra bit in case of carry
    //printf("%d,%d,%d\n",size1,size2,maxSize);
    int digit1,digit2,resultDigit,carry=0;

<<<<<<< HEAD
// Function to return the top from stack without removing it
int peek(struct Stack *stack)
{
    if (isEmpty(stack))
        return -1;
    return stack->array[stack->top];
}

char* octAdd(char* num1,char* num2){
    int size1,size2,maxSize;
    //get the size of each number
    for(size1=0; num1[size1]!=0;size1++);
    //printf("Size of first number: %d\n",size1);
    
    for(size2=0; num2[size2]!=0;size2++);
    //printf("Size of second number: %d\n",size2);
    
    maxSize=(size1>size2)?size1:size2;

    size1-=1;//index to the msb
    size2-=1;//index to the msb
    //printf("Max Size: %d\n",maxSize+1);

    char* result=(char*)malloc(maxSize+2);
    result[maxSize+2]=0;//NULL TERMINATED STRING

    //result[0]=0;//to know if we used the extra bit in case of carry
    int digit1,digit2,resultDigit,carry=0;
    while(maxSize>=0){
        if(size1>=0){//in case the other number has more digits
            digit1=num1[size1]-'0';//convert from char to oct digit
        }
        else{
            digit1=0;
        }
       // printf("digit1: %d\n",digit1);
        if(size2>=0){
             digit2=num2[size2]-'0';//convert from char to oct digit
        }
        else{//in case the other number has more digits
            digit2=0;
        }
        //printf("digit2: %d\n",digit2);
        
        resultDigit=digit1+digit2+carry;//the main add
=======
    while(maxSize>=0){
        
        if(size1>=0){//in case the other number has more digits
            digit1=num1[size1]-'0';//convert from char to oct digit
             printf("a");
        }
        else{
            digit1=0;
            printf("b");
        }
        
        if(size2>=0){
             digit2=num2[size2]-'0';//convert from char to oct digit
             printf("c");
        }
        else{//in case the other number has more digits
            digit2=0;
            printf("d");
        }
       
       
        if(op[0]=='+'){//add
            resultDigit=digit1+digit2+carry;
              printf("ResultDigit: %d\n",resultDigit);
        }
        else if(op[0]=='&'){//and
            resultDigit=digit1&digit2;
        }
        
>>>>>>> 9a2d4b87f451ea326b8e24e621144cc5b43a051d
        //printf("result digit before carry: %d\n",resultDigit);
        if(resultDigit>7){//in case of carry
            resultDigit-=8;
            carry=1;
        }
        else{
            carry=0;
        }
         //("result digit after carry: %d\ncarry: %d\n",resultDigit,carry);
<<<<<<< HEAD
        

=======
      
>>>>>>> 9a2d4b87f451ea326b8e24e621144cc5b43a051d
        result[maxSize]=resultDigit+'0';//convert from oct to char and put int the result string

        maxSize--;
        size1--;
        size2--;
<<<<<<< HEAD
    }

    if(result[0]=='0'){
        //The lsb has not been used
        //printf("The lsb has not been used\n");
        result=result+1;
    }

=======
    }
    int leadingZeroes=0;
    while(result[leadingZeroes]=='0'){
        leadingZeroes++;
    }
    if(leadingZeroes>0){
        result=toShorterStr(result,resultSize+1-leadingZeroes,leadingZeroes);
    }

>>>>>>> 9a2d4b87f451ea326b8e24e621144cc5b43a051d
    return result;
    
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
    do{
        char num1[80];
        char num2[80];
=======
    
    do{
        char num1[80];
        char num2[80];
        char op[2];
        op[2]=0;
>>>>>>> 9a2d4b87f451ea326b8e24e621144cc5b43a051d
        printf("Calc: ");
        gets(num1);
        printf("Cals: ");
        gets(num2);
<<<<<<< HEAD
        printf("ADD Result: %s\n",octAdd(num1,num2));

    }while(1);
    


=======
        printf("Enter op: ");
        gets(op);
        if(op[0]=='q')break;
        else if(op[0]=='&'||op[0]=='+'){
            char*reuslt=octOperation(num1,num2,op);
            printf("%s %s %s = %s\n",num1,op,num2,reuslt);
            free (reuslt);
        }   
        else{

        }

    }while(1);
    


>>>>>>> 9a2d4b87f451ea326b8e24e621144cc5b43a051d
    /*
    Stack *stack = createStack(63);
    stack->capacity = 5;
    char buf[80];
    int number = 0;
    fgets(buf, 80, stdin);
    for (int i = 0; i < 8; i++){
        number += convertOctToBinary(buf[i], i);
    }
    bin(number);
    */

    
   

    

    return 0;
}