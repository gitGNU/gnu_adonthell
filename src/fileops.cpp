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

#include "types.h"
#include "fileops.h"
/*
void getstringfromfile(char strg[],SDL_RWops * file)
{
  char c;
  u_int16 gc=0;
  do
    {
      SDL_RWread(file,&c,sizeof(c),1);
      strg[gc]=c;
      gc++;
    }
  while(strg[gc-1]!=0);
}

void putstringtofile(char strg[],SDL_RWops * file)
{
  u_int16 gc=0;
  do
    {
      SDL_RWwrite(file,&strg[gc],sizeof(strg[gc]),1);
      gc++;
    }
  while(strg[gc-1]!=0);
}
*/
void put_string (gzFile file, char *string)
{
    u_int16 length = 0;

    if (string != NULL)
    {
        length = strlen (string);
        gzwrite (file, &length, sizeof (length));    
        gzwrite (file, string, length);
    } 
    else gzwrite (file, &length, sizeof (length));   
}

char *get_string (gzFile file)
{
    char *string = NULL;
    u_int16 length;

    gzread (file, &length, sizeof (length));
    if (length != 0)
    {
        string = new char[length+1];
        gzread (file, string, length);
        string[length] = 0;
    }

    return string;
}