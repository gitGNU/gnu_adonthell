#include <string.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_friend.h"
#include "win_base.h"
#include "win_font.h"
#include "win_container.h"
#include "win_label.h"


win_label::win_label(u_int16 x,u_int16 y,u_int16 l,u_int16 h,win_font *fo,win_container * tmpwc):win_base(x,y,l,h,tmpwc,tmpwc->get_drawing_area())
{
  font=fo;
  text.lenght=0;
  text.pos=0;
  text.pos_tmp=0;
  ch_page=false;
}

void win_label::set_font(win_font * fo)
{
  font=fo;
}

void win_label::set_text(char t[])
{
  strcpy(text.text,t);
  text.lenght=strlen(text.text);
  text.pos=0;
  text.pos_tmp=0;
}

void win_label::erase_text()
{
  text.lenght=0;
  text.pos_tmp=0;
}

void win_label::draw()
{
  if(visible && wc) {
    draw_background();
    draw_text(wc->get_x()+x,wc->get_y()+y,wc->get_x()+x+length,wc->get_y()+y+height,font,text,da);
    draw_border();
  }
}

void win_label::update()
{
  if(ch_page)
    {ch_page=false;text.pos=text.pos_tmp;}
}

void win_label::next_page()
{
  ch_page=true;
}

bool win_label::end_text()
{
  return(text.pos==text.lenght);
}
















