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

#include "instruction_class.h"
#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include "fileops.h"

instruction::instruction()
{}

void instruction::get(FILE* file)
{
   int i;

   for(i=0;i<9;++i)
      fread(&param_val[i],sizeof(param_val[i]),1,file);
   getstringfromfile(string_val,file);
}

void instruction::put(FILE* file)
{
   int i;

   for(i=0;i<9;++i)
      fwrite(&param_val[i],sizeof(param_val[i]),1,file);
   putstringtofile(string_val,file);
}

istream& operator >> (istream& stream, instruction &instr)
{
  int i;
  unsigned int val;

  for(i=0;i<9;++i)
  {
    stream >> val;
    instr.param_val[i]=val;
  }

  return stream;
}

ostream& operator << (ostream& stream,instruction &instr)
{
  int i;
 
  for (i=0;i<9;++i) stream << instr.param_val[i];

  return stream;
}

u_int16 instruction::param(int n)
{
   return param_val[n-1];
}

char* instruction::string()
{
   return string_val;
}
