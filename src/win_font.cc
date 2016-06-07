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

win_font::win_font()
{
  cursor=NULL;
}

win_font::win_font(char * fic)
{
  cursor=NULL;
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
  for(hash_map<u_int16, image*>::iterator i = glyphs.begin(); i != glyphs.end(); i++)
  {
	  delete i->second;
  }
  glyphs.clear ();
}


void win_font::load(char * rep)
{
  erase();
  
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

  char i;
  u_int16 pos,tl;
  
  while(!f.eof())
    {
      
      i << f;
      pos << f;
      tl << f; 
      if(i>0 && i<WIN_NB_TABLE_CHAR)
	{
        image *glph = new image (tl + 1,font->height()-1);
        font->draw (0, 0, pos, 0, tl + 1, font->height () -1, NULL, glph);
        glyphs[i] = glph; 
	} 
    }
  
  height_=font->height()-1;
  
  length_=glyphs[' ']->length();
  
  if(font)delete font;
  
  f.close ();  
}


bool win_font::in_table(u_int16 tmp)
{
    if (glyphs.find (tmp) != glyphs.end ()) return true;
    else return false;
}

image & win_font::operator[](u_int16 i)
{
    if (in_table (i)) return *(glyphs[i]);
    else return *(glyphs[' ']);
}

