/*
  
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"

win_font::win_font()
{
  table=NULL;
  cursor=NULL;
}

win_font::win_font(char * fic)
{
  cursor=NULL;
  table=NULL;
  load_font(fic);
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


void win_font::load_font(char * rep)
{
  if(table!=NULL) erase();

  //file which contains font information and cursor
  gzFile f;

  //path where is the file
  char path[255];

  //path win directory
  strcpy(path,WIN_DIRECTORY);

  //add win font directory path
  strcat(path,WIN_FONT_DIRECTORY);

  //add theme pass
  strcat(path,rep);

  //add font filename 
  strcat(path,WIN_FONT_FILE);
   
  //open gzfile 
  if((f=gzopen(path,"rb"))==NULL)
    { cout << path << " not found !\n";exit(1);}


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


  while(!gzeof(f))
    {
      gzread(f,&i,sizeof(i));
      gzread(f,&pos,sizeof(pos));
      gzread(f,&tl,sizeof(tl));
      if(i>0 && i<WIN_NB_TABLE_CHAR)
	{
	  table_core[i]=true;
	  table[i].resize(tl,font->height()-1);
	  table[i].putbox_part_img(font,0,0,tl,font->height()-1,pos,0);
	} 
      j++;
    }

  height_=font->height()-1;
  length_=table[' '].length();
  
  if(font)delete font;
  gzclose (f);
}

void win_font::load(char * rep)
{
  if(table!=NULL) erase();
  FILE *f;
  char path[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_FONT_DIRECTORY);
  strcat(path,rep);
  strcat(path,WIN_FONT_FILE_IDX);
 
 //open file with information about font
  if((f=fopen(path,"rb"))==NULL)
    { cout << WIN_FONT_FILE_IDX << " not found !\n";exit(1);}  

  // load font
  image *font=new image();
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_FONT_DIRECTORY);
  strcat(path,rep);
  strcat(path,WIN_FONT_FILE_PIC);
  font->load_pnm(path);//new
  table=new image[WIN_NB_TABLE_CHAR];
  init_in_table();
  char i;int j;
  u_int16 pos,tl;
  j=0;
  while(j<WIN_NB_TABLE_CHAR && !feof(f) )
    {
      fread(&i,sizeof(i),1,f);
      fread(&pos,sizeof(pos),1,f);
      fread(&tl,sizeof(tl),1,f);
      table_core[i]=true;
      table[i].resize(tl,font->height());
      table[i].putbox_part_img(font,0,0,tl,font->height(),pos,0);
      j++;
    }
  height_=font->height();
  length_=table[' '].length();
  if(font)delete font;


  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_FONT_DIRECTORY);
  strcat(path,rep);

  strcat(path,"cursor.pnm");

  cursor=new image();
  cursor->load_pnm(path);
  fclose(f);
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









