#include <stdio.h>
#include "types.h"
#include "fileops.h"

void getstringfromfile(char strg[],FILE * file)
{
  char c;
  int gc=0;
  while ((c=fgetc(file))!=0)
    {
      strg[gc]=c;
      gc++;
    }
  strg[gc]=0;
}

void putstringtofile(char strg[],FILE * file)
{
  fputs(strg,file);
  fputc(0,file);
}
