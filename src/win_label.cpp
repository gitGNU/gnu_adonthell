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

win_label::win_label(win_font * fo,win_container * tmpwc):win_base()
{
  wc= tmpwc;
  da= wc->get_da(); 
  font=fo;
  text.lenght=0;
  text.pos=0;
  text.pos_tmp=0;
  ch_page=false;
}

win_label::win_label(u_int16 x,u_int16 y,u_int16 l,u_int16 h,win_font *fo,win_container * tmpwc,char t[]):win_base(x,y,l,h)
{
  wc= tmpwc;
  da=wc->get_da();
  font=fo;
  if(t) {
    strcpy(text.text,t);
    text.lenght=strlen(text.text);
  }else {
    text.lenght=0;
  }
  text.pos=0;
  text.pos_tmp=0;
  ch_page=false;
}

void win_label::set_drawing_area(drawing_area * da)
{
  win_label::da=da;
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
  if(visible) {
    draw_text(wc->getx()+x,wc->gety()+y,wc->getx()+x+l,wc->gety()+y+h,font,text,da);
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

void win_label::show()
{
  visible=true;
}

void win_label::hide()
{
  visible=false;
}

void win_label::move(u_int16 tx,u_int16 ty)
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

void win_label::resize(u_int16 tl,u_int16 th)
{
  l=tl;
  h=th;
}













