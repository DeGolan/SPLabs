#include <stdio.h>
#include <stdlib.h>

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
}

// Stack is full when top is equal to the last index
int isFull(struct Stack *stack)
{
    return stack->top == stack->capacity - 1;
}

// Stack is empty when top is equal to -1
int isEmpty(struct Stack *stack)
{
    return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1
void push(struct Stack *stack, int item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
    printf("%d pushed to stack\n", item);
}

// Function to remove an item from stack.  It decreases top by 1
int pop(struct Stack *stack)
{
    if (isEmpty(stack))
        return -1;
    return stack->array[stack->top--];
}

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
        //printf("result digit before carry: %d\n",resultDigit);
        if(resultDigit>7){//in case of carry
            resultDigit-=8;
            carry=1;
        }
        else{
            carry=0;
        }
         //("result digit after carry: %d\ncarry: %d\n",resultDigit,carry);
        

        result[maxSize]=resultDigit+'0';//convert from oct to char and put int the result string

        maxSize--;
        size1--;
        size2--;
    }

    if(result[0]=='0'){
        //The lsb has not been used
        //printf("The lsb has not been used\n");
        result=result+1;
    }

    return result;
    
}

int main(int argc, char **argv)
{
    do{
        char num1[80];
        char num2[80];
        printf("Calc: ");
        gets(num1);
        printf("Cals: ");
        gets(num2);
        printf("ADD Result: %s\n",octAdd(num1,num2));

    }while(1);
    


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