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
 
  for (i=0;i<9;++i) cout << instr.param_val[i];

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
