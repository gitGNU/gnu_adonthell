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
  static u_int16 curletter;
  static bool alwayspush;
  u_int16 n=input::getkeypressed();  
  if(input::is_pushed(curletter))
    {
      if(!alwayspush)     
	{
	  if(text.lenght<TEXT_MAX)
	    { 
	      if(n==Enter_Key)
		{
		  beg_select=end_select+2;
		  end_select=(text.lenght-1);
		  ok_text=true;
		  text.text[text.lenght++]='\n';
		}      
	      else   
		{
		  if(n==Backspace_Key && text.lenght>0) 
		    {text.lenght--;
		    if(text.pos>0) text.pos--;
		    }
		  else 
		    //ERROR TO SEE UPPERCASE
		    if(input::is_pushed(Shift_Key))
		      text.text[text.lenght++]=n-30;
		    else text.text[text.lenght++]=n;
		}
	    }
	  alwayspush=true;
	}
    }
  else
    {
      alwayspush=false;
    }
  curletter=n;
}







