/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

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
