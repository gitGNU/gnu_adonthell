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
#include "stdio.h"
#include "types.h"
#include "input.h"
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_friend.h"
#include "win_container.h"
#include "win_select.h"
#include "win_write.h"


win_write::win_write(u_int16 tx,u_int16 ty,u_int16 tl,u_int16 th,win_font *fo,win_container *tmpwc):win_base(tx,ty,tl,th,tmpwc,tmpwc->get_drawing_area())
{
  font=fo;
  text.lenght=0;
  text.pos=0;
  text.pos_tmp=0;
  text.end_win=false;
  /*  beg_select=0;
      end_select=-2;*/
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
  //  return((end_select>=0) && (end_select-beg_select>0) && ok_text);
  return(ok_text);
}

char * win_write::get_text()
{
  if (is_text())
    { 
      u_int16 i;
      //      register int j=beg_select;
      //      while(i<=(end_select-beg_select))
      //	text_result[i++]=text.text[j++];
      for(i=0;i<text.lenght;i++)
	text_result[i]=text.text[i];
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
       else draw_text(real_x,real_y,real_x+length,real_y+height,font,text,da,false);
       draw_border();
       //if text reach end window
       if(text.end_win) 
	 text.pos+=text.len_fl; 
    }
}

void win_write::update()
{
  win_write::write();
}

void win_write::write()
{
  static s_int32 c;
  while((c=input::get_next_key())>0)
    {
      if(c==SDLK_BACKSPACE) 
	{
	  if(text.lenght>0)
	    {
	      text.lenght--;
	      if(text.pos>0) text.pos--;
	    }
	}
      else if(text.lenght<TEXT_MAX)
	{
	  if(c==SDLK_RETURN)
	    {
	      //	      beg_select=end_select+2;
	      //      end_select=(text.lenght-1);
	      ok_text=true;
	      text.text[text.lenght++]='\0';
	    }
	  else text.text[text.lenght++]=c;
	}
    }
}







