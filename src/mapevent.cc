#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "mapevent.h"
#include "instruction.h"

#define casei(x) case x : pi=new event_ ## x; break;

void mapevent::get(FILE * file)
{
   instruction *pi;
   u_int16 type;

   fread(&type,sizeof(type),1,file);
   switch (type)
   {
      casei(0);
      casei(1);
      casei(2);
      casei(3);
      casei(4);
      casei(5);
      casei(6);
      casei(7);
      casei(8);
      casei(9);
      casei(10);
      casei(11);
      casei(255);
   }
   
   pi->get(file);
   push_back(pi);
}

void mapevent::run(mapcharacter*aguy, map*amap, u_int16 x, u_int16 y)
{
   iterator i;

   i=begin();
   while(i!=end())
   {
      (*i)->exec(aguy,amap,x,y);
   }
}
