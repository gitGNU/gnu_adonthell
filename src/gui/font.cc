/*
  
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "font.h"
#include "../game.h"

using namespace gui;

font::font()
{
  table=NULL;
  cursor=NULL;
}

font::font(char * fic)
{
  cursor=NULL;
  table=NULL; 
  load(fic);
}

font::font(font & tmpfont)
{
  *this=tmpfont;
}

font::~font()
{
  erase();
}
void font::erase()
{
  if(cursor) delete cursor;
  if(table) delete [] table;
  table=NULL;
}


void font::load(char * rep)
{
  if(table!=NULL) erase();
  
  //file which contains font information and cursor
  igzstream f; 
  
  //path where is the file
  std::string path = DIRECTORY; 
  
  //add win font directory path
  path += FONT_DIRECTORY; 
  
  //add theme pass
  path += std::string (rep) + "/"; 
  
  //add font filename 
  path += FONT_FILE; 

  path = game::find_file (path); 
  
  //open gzfile 
  if (!f.open (path)) 
  {
    std::cout << path << " not found !\n";
    exit(1);
  } 
  
  //create image wich contain the main font image
  gfx::image * font = new gfx::image();
  font->get(f);
  
  //get the cursor
  cursor=new gfx::image();
  cursor->get(f);

  //create a table for each letter
  table=new gfx::image[NB_TABLE_CHAR];
  
  //init the boolean table 
  init_in_table();
  
  char i;int j=0;
  u_int16 pos,tl;
  
  while(!f.eof())
    {
      
      i << f;
      pos << f;
      tl << f; 
      if(i>0 && i<NB_TABLE_CHAR)
	{
          table_core[i]=true;
	  table[i].resize(tl + 1,font->height()-1);
	  font->draw (0, 0, pos, 0, tl + 1, font->height () -1, NULL, &(table[i])); 
	} 
      j++;
    }
  
  height_=font->height()-1;
  
  length_=table[' '].length();
  
  if(font)delete font;
  
  f.close ();  
}


bool font::in_table(u_int16 tmp)
{
  if(tmp>NB_TABLE_CHAR) return false;
  return table_core[tmp];
}

void font::init_in_table()
{
  register u_int16 i;
  for(i=0;i<NB_TABLE_CHAR;i++)
    table_core[i]=false;
}

gfx::image & font::operator[](int i)
{
  return table[i];
}
