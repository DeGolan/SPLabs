#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fun_desc
{
  char *name;
  char (*fun)(char);
};

char censor(char c)
{
  if (c == '!')
    return '.';
  else
    return c;
}

char encrypt(char c)
{ /* Gets a char c and returns its encrypted form by adding 3 to its value. 
                        If c is not between 0x20 and 0x7E it is returned unchanged */
  if (c > 0x20 && c < 0x7E)
  {
    c = c + 3;
  }
  return c;
}

char decrypt(char c)
{ /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
                        If c is not between 0x20 and 0x7E it is returned unchanged */
  if (c > 0x20 && c < 0x7E)
  {
    c = c - 3;
  }
  return c;
}

char cprt(char c)
{ /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
  if (c > 0x20 && c < 0x7E)
  {
    fprintf(stdout, "%c\n", c);
  }
  else
  {
    fprintf(stdout, ".\n");
  }
  return c;
}

char my_get(char c)
{ /* Ignores c, reads and returns a character from stdin using fgetc. */
  return fgetc(stdin);
}
char *map(char *array, int array_length, char (*f)(char))
{
  char *mapped_array = (char *)(malloc(array_length * sizeof(char)));
  for (int i = 0; i < array_length; i++)
  {
    mapped_array[i] = f(array[i]);
  }
  return mapped_array;
}

int main(int argc, char **argv)
{
  char carray[5] = "";
  struct fun_desc menu[] = {{"Censor", censor}, {"Encrypt", encrypt}, {"Decrypt", decrypt}, {"Print string", cprt}, {"Get string", my_get}, {NULL, NULL}};
  int upperBound = sizeof(menu) / 8 - 1; // /8 for size ofstruct (2 pointers) -1 for the null at the end
  int i = 0;
  int option;
  while (1)
  {
    i = 0;
    while (1)
    {
      if (menu[i].name == NULL)
      {
        break;
      }
      fprintf(stdout, "%d)%s\n", i, menu[i].name);
      i++;
    }
    printf("Option: ");
    while (scanf("%d", &option) != 1)
    {
      while (getchar() != '\n');
    }
    if (option > 0 && option < upperBound)
    {
      fprintf(stdout, "WITHIN BOUNDS\n");
      char *tmp = map(carray, 5, menu[option].fun);
      strcpy(carray, tmp);
      free(tmp);
      fprintf(stdout, "DONE.\n");
    }
    else
    {
      fprintf(stdout, "OUT OF BOUNDS \n");
      break;
    }
  }
  return 0;
}