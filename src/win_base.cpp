#include "types.h"
#include "win_base.h"

win_base::win_base()
{
  x=0;
  y=0;
  l=0;
  h=0;
  select=false;
  visible=false;
}

win_base::win_base(u_int16 x,u_int16 y, u_int16 l,u_int16 h)
{
  win_base::x=x;
  win_base::y=y;
  win_base::l=l;
  win_base::h=h;
  select=false;
  visible=false;
}



bool win_base::is_selected()
{
  return(visible);
}

void win_base::set_selected(bool b)
{
  select=b;
}

bool win_base::is_visible()
{
  return(visible);
}

void win_base::set_visible(bool b)
{
  visible=b;
}

u_int16 win_base::getx()
{
  return(x);
}

u_int16 win_base::gety()
{
  return(y);
}

u_int16 win_base::getl()
{
  return(l);
}

u_int16 win_base::geth()
{
  return(h);
}



