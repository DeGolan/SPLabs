#include <stdio.h>
#include <string.h>

extern void assFunc(int x);

char c_checkValidity(int x){
  if(x%2==0) return 1;
  return 0;  
}
int main(int argc, char** argv) {
  int num;
  printf("Enter the integer: ");
  scanf("%d", &num);

  assFunc(num);		/* call your assembly function */
  
  return 0;
}