#include <stdio.h>
#include <stdlib.h>

// A structure to represent a stack
typedef struct Stack
{
    int top;
    unsigned capacity;
    int *array;
} Stack;

typedef struct Node
{
    char data;
    Node *next;
}Node;

void addNode(Node* list,char data){
    Node* new=malloc(sizeof(Node));
    new.next=list;
    new.data=data;
    list=new;
}
int listSize(Node* list){
    int size=0;
    Node* temp=list;
    while(temp!=NULL){
        size++;
        temp=temp->next;
    } 
}
void printList(Node* list){
    Node* temp=list;
    while(temp!=NULL){
        printf("%x",temp->data & 0xff);
        temp=temp->next;
    } 
}

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

int convertOctToBinary(char octC, int position)
{
    int oct = octC - '0';
    oct = oct << position * 3;

    int add = (0b1);
    int bin = 0b000;
    for (int i = 0; oct > 0; i++)
    {
        if (oct % 2 == 1)
        {
            bin = bin + add;
        }
        oct = oct / 2;
        add = add << 1;
    }
    return bin;
}

void bin(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

int main(int argc, char **argv)
{

    Stack *stack = createStack(63);
    stack->capacity = 5;
    char buf[80];
    int number = 0;
    fgets(buf, 80, stdin);
    for (int i = 0; i < 8; i++){
        number += convertOctToBinary(buf[i], i);
    }
    bin(number);
    
    Node* list=NULL;

    

    return 0;
}