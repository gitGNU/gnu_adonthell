/*
  
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "win_font.h"

#include "game.h"

win_font::win_font()
{
  table=NULL;
  cursor=NULL;
}

win_font::win_font(char * fic)
{
  cursor=NULL;
  table=NULL;
  load(fic);
}

win_font::win_font(win_font & tmpfont)
{
  *this=tmpfont;
}

win_font::~win_font()
{
  erase();
}
void win_font::erase()
{
  if(cursor) delete cursor;
  if(table) delete [] table;
  table=NULL;
}


void win_font::load(char * rep)
{
  if(table!=NULL) erase();
  
  //file which contains font information and cursor
  igzstream f; 
  
  //path where is the file
  string path = WIN_DIRECTORY; 
  
  //add win font directory path
  path += WIN_FONT_DIRECTORY; 
  
  //add theme pass
  path += string (rep) + "/"; 
  
  //add font filename 
  path += WIN_FONT_FILE; 

  path = game::find_file (path); 
  
  //open gzfile 
  if (!f.open (path)) 
  {
    cout << path << " not found !\n";
    exit(1);
  } 
  
  //create image wich contain the main font image
  image *font=new image();
  font->get(f);
  
  //get the cursor
  cursor=new image();
  cursor->get(f);

  //create a table for each letter
  table=new image[WIN_NB_TABLE_CHAR];
  
  //init the boolean table 
  init_in_table();
  
  char i;int j=0;
  u_int16 pos,tl;
  
  while(!f.eof())
    {
      
      i << f;
      pos << f;
      tl << f; 
      if(i>0 && i<WIN_NB_TABLE_CHAR)
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


bool win_font::in_table(u_int16 tmp)
{
  if(tmp>WIN_NB_TABLE_CHAR) return false;
  return table_core[tmp];
}

void win_font::init_in_table()
{
  register u_int16 i;
  for(i=0;i<WIN_NB_TABLE_CHAR;i++)
    table_core[i]=false;
}

image & win_font::operator[](int i)
{
  return table[i];
}


/*
win_font & win_font::operator=(win_font & tmpfont)
{
  erase();
  table=new image[WIN_NB_TABLE_CHAR];
  cursor=new image();
  *cursor=*(tmpfont.cursor);
  for(u_int16 i=0;i<WIN_NB_TABLE_CHAR;i++)
    if(table_core[i]=tmpfont.in_table(i))
      table[i]=tmpfont.table[i];
  height_=tmpfont.height();
  length_=tmpfont.length();
  return * this;
}
*/
 
