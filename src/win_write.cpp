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
#include "win_write.h"

win_write::win_write(win_font *fo,win_container * tmpwc):win_base()
{
  font=fo;
  text.lenght=0;
  text.pos=0;
  text.pos_tmp=0;
  text.end_win=false;
  beg_select=0;
  end_select=-2;
  ok_text=false;
  wc=tmpwc;
  da=wc->get_da();
}

win_write::win_write(u_int16 x,u_int16 y,u_int16 l,u_int16 h,win_font *fo,win_container *tmpwc):win_base(x,y,l,h)
{
  font=fo;
  text.lenght=0;
  text.pos=0;
  text.pos_tmp=0;
  text.end_win=false;
  beg_select=0;
  end_select=-2;
  ok_text=false;
  wc=tmpwc;
  da=wc->get_da();
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
  if(visible)
    {
      draw_text(wc->getx()+x,wc->gety()+y,wc->getx()+x+l,wc->gety()+y+h,font,text,da);  
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


void win_write::show()
{
  visible=true;
}


void win_write::hide()
{
  visible=false;
}


void win_write::set_drawing_area(drawing_area * da)
{
  win_write::da=da;
}


void win_write::move(u_int16 tx,u_int16 ty)
{
  if(wc)
    {
      x=wc->getx()+tx;
      y=wc->gety()+ty;
    }
  else
    {
      x=tx;
      y=ty;
    }
}

void win_write::resize(u_int16 tl,u_int16 th)
{
  l=tl;
  h=th;
}


