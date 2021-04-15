#include <stdio.h>
#define MAX_LEN 34 /* maximal input string size */
                   /* enough to get 32-bit string + '\n' + null terminator */
extern int convertor(char *buf);

void convertorInC(char *buf)
{
  char buf2[MAX_LEN];
  int hexDec = buf[0];
  int bufLoc=0;
  int j;
  for (j = 0; buf[j] != 0; j++)
  {
    hexDec = buf[j];

    for (int i = bufLoc; i < bufLoc+4; i++)
    {
      buf2[i] = '0';
    }
    

    if (hexDec >= '0' && hexDec <= '9')
    {
      hexDec = hexDec - '0';
    }
    else
    {
      hexDec = hexDec - 'A' + 10;
    }

    for (int i = 0; hexDec > 0; i++)
    {

      if (hexDec % 2 == 0)
      {
        buf2[bufLoc+3 - i] = '0';
      }
      else
      {
        buf2[bufLoc+3 - i] = '1';
      }
      hexDec = hexDec / 2;
    }

    bufLoc=bufLoc+4;
  }
  buf2[(4*(j-1))] = 0;

  printf("%s\n", buf2);
}

int main(int argc, char **argv)
{
  char buf[MAX_LEN];
  do
  {
    fgets(buf, MAX_LEN, stdin); /* get user input string */
    if (buf[0] == 'q')
      break;
    convertor(buf); /* call your assembly function */

  } while (1);

  return 0;
}