/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <string.h>
#include <list>
#include <iostream.h>
#include "types.h"
#include "input.h"
#include "win_types.h"
#include "win_font.h"
#include "win_border.h"
#include "win_base.h"
#include "win_label.h"
#include "win_select.h"
#include "win_container.h"
#include "win_write.h"



win_write::win_write(s_int16 x,s_int16 y,u_int16 l,u_int16 h,win_font *fo,win_container * tmpwc):win_label(x,y,l,h,fo,tmpwc)
{
  tmp_input[0]='\0';
  tmp_input[1]='\0';
  //  text_get[0]='\0';
  ok_text=false;
}


bool win_write::is_text()
{
  return(ok_text);
}

char * win_write::get_text_entry()
{
  if(ok_text)
    {
      ok_text=false;
      /*      for(int i=0;i<size_texte;i++)
	text_get[j++]=texte[i];
	text_get[j]='\0';
	return text_get;*/
      return texte;
    }
  else return NULL;
}

void win_write::write()
{
  static s_int32 c;
  while((c=input::get_next_unicode())>0)
    {
#ifdef _DEBUG_
      cout << "win_write::write() key pushed: " << (char) c << endl;
#endif
      if(font->in_table(c) || c==SDLK_BACKSPACE || c==SDLK_RETURN)
	if(size_texte<WIN_TEXT_MAX_LENGTH)
	  {
	    if(c==SDLK_BACKSPACE && size_texte>0) 
	      {
		texte[--size_texte]='\0';
		init_draw();
	      }
	    else 
	      {
		if(c!=SDLK_BACKSPACE)
		  {
		    if(c == SDLK_RETURN) 
		      {
			ok_text=true;
			tmp_input[0]='\n';
		      }
		    else 
		      {
			tmp_input[0]=(char) c;
			add_text(tmp_input);
		      }
		  }
	      }
	  }
    }
}

void win_write::update()
{
  win_label::update();
  write();
}


/*
win_write::win_write(u_int16 tx,u_int16 ty,u_int16 tl,u_int16 th,win_font *fo,win_container *tmpwc):win_base(tx,ty,tl,th,tmpwc,tmpwc->get_drawing_area())
{
  font=fo;
  text.lenght=0;
  text.pos=0;
  text.pos_tmp=0;
  text.end_win=false;
  beg_select=0;
  end_select=-2;
  ok_text=false;
}

win_write::~win_write()
{
  if(wselect)
    {
       wselect->remove(this);
       wselect=NULL;
     }
}

bool win_write::is_text()
{
  return((end_select>=0) && (end_select-beg_select>0) && ok_text);
}

char * win_write::get_text()
{
  if (is_text())
    { 
      register int i=0;
      register int j=beg_select;
      while(i<=(end_select-beg_select))
	text_result[i++]=text.text[j++];
      text_result[i]='\0';
      ok_text=false;
      return text_result;
    }
  else return NULL;
}

void win_write::set_font(win_font *fo)
{
  font=fo;
}


void win_write::draw()
{
  if(visible && wc)
    {
      draw_background();
      if(wselect && !selected && select_mode==WIN_SELECT_MODE_BRIGHTNESS) draw_text(real_x,real_y,real_x+length,real_y+height,font,text,da,true);
      else 
	{
	  draw_text(real_x,real_y,real_x+length,real_y+height,font,text,da,false);
	  
	}
	  draw_border();
      //if text reach end window
      if(text.end_win) 
	text.pos+=text.len_fl; 
    }
}

void win_write::update()
{
  win_write::write();
  cout << "update\n";
}



void win_write::write()
{
  static s_int32 c;
  while((c=input::get_next_key())>0)
    {
      if(text.lenght<WIN_TEXT_MAX_LENGTH)
	{
	  if(c==SDLK_RETURN)
	    {
	      beg_select=end_select+2;
	      end_select=(text.lenght-1);
	      cout << beg_select << " " << end_select << endl;
	      ok_text=true;
	      text.text[text.lenght++]='\n';
	    }
	  else
	    {
	      if(c==SDLK_BACKSPACE && text.lenght>0) 
		{
		  text.lenght--;
		  if(text.pos>0) text.pos--;
		}
	      else text.text[text.lenght++]=c;
	    }
	}
    }
}

*/





