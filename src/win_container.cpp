#include <iostream.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"
#include "win_base.h"
#include "win_write.h"
#include "win_label.h"
#include "win_border.h"
#include "win_container.h"

win_container::win_container():win_base()
{
  l_label.clear();
  l_write.clear();
  l_container.clear();
  wc=NULL;
  wb=NULL;
  wback=NULL;
  da=NULL;
}

win_container::~win_container()
{
  l_label.clear();
  l_write.clear();
  l_container.clear();
  delete wc;
  delete wb;
  delete wback;
  delete da;
}

win_container::win_container(u_int16 x,u_int16 y,u_int16 l,u_int16 h):win_base(x,y,l,h)
{
  l_label.clear();
  l_write.clear();
  l_container.clear();
  wc=NULL;
  wb=NULL;
  wback=NULL;
  da=new drawing_area(x,y,l,h);
}

win_container::win_container(u_int16 x,u_int16 y,u_int16 l,u_int16 h,win_container *tmpwc):win_base(tmpwc->getx()+x,tmpwc->gety()+y,l,h)
{
  l_label.clear();
  l_write.clear();
  l_container.clear();
  wc=tmpwc;
  wb=NULL;
  wback=NULL;
  da=new drawing_area(wc->getx()+x,wc->gety()+y,l,h);
}

void win_container::move(u_int16 tx , u_int16 ty)
{
  if(wc)
    {
      x=wc->getx()+tx;
      y=wc->gety()+ty;
      //move area
      if(da)
	{
	  da->x=x;
	  da->y=y;
	}
    }
  else
    {
      x=tx;
      y=ty;
      if(da)
	{
	  da->x=x;
	  da->y=y;
	}
    }
}

void win_container::resize(u_int16 tl, u_int16 th)
{
  l=tl;
  h=th;
}

win_label * win_container::add_label(u_int16 tx,u_int16 ty,u_int16 tl,u_int16 th,win_font *ft)
{
  win_label * tmp = new win_label(tx,ty,tl,th,ft,this,NULL);
  l_label.push_front(tmp);
  return (tmp);
}

win_write * win_container::add_write(u_int16 tx,u_int16 ty,u_int16 tl,u_int16 th,win_font *ft)
{
  win_write * tmp = new win_write(tx,ty,tl,th,ft,this);
  l_write.push_front(tmp);
  return (tmp);
}

win_container * win_container::add_container(u_int16 tx,u_int16 ty,u_int16 tl,u_int16 th)
{
  win_container * tmp = new win_container(tx,ty,tl,th,this);
  l_container.push_front(tmp);
  return (tmp);
}

win_border * win_container::set_border(char * rep)
{
  if(wb) delete wb;
  // wb->assign_drawing_area(da);
  wb=new win_border(rep,this);
  return(wb);
}

win_background * win_container::set_background(char * rep)
{
  if(wback) delete wback;
  // wb->assign_drawing_area(da);
  wback=new win_background(rep,this);
  return(wback);
}

void win_container::remove_label(win_label * tmp)
{
  list<win_label *>::iterator i=l_label.begin();
  while(i!=l_label.end() && tmp!=(*i))
    i++;
  if(i!=l_label.end())
    l_label.erase(i);
}

void win_container::remove_write(win_write * tmp)
{
  list<win_write *>::iterator i=l_write.begin();
  while(i!=l_write.end() && tmp!=(*i))
    i++;
  if(i!=l_write.end())
    l_write.erase(i);
}

void win_container::remove_container(win_container * tmp)
{
  list<win_container *>::iterator i=l_container.begin();
  while(i!=l_container.end() && tmp!=(*i))
    i++;
  if(i!=l_container.end())
    l_container.erase(i);
}

void win_container::draw()
{
  if(visible)
    {
      if(wback) wback->draw();
      if(wb) wb->draw();
      
      list<win_container *>::iterator k=l_container.begin();
      while(k!=l_container.end())
	{
	  (*k)->draw();
	  k++;
	}

      list<win_label *>::iterator i=l_label.begin();
      while(i!=l_label.end())
	{
	  (*i)->draw();
	  i++;
	  }

      list<win_write *>::iterator j=l_write.begin();
      while(j!=l_write.end())
	{
	  (*j)->draw();
	  j++;
	}      
    }
}

void win_container::update()
{
  if(visible)
    {
      if(wback) wback->update();
      if(wb) wb->update();


      list<win_label *>::iterator i=l_label.begin();
      while(i!=l_label.end())
	{
	  (*i)->update();
	  i++;
	}
      list<win_write *>::iterator j=l_write.begin();
      while(j!=l_write.end())
	{
	  (*j)->update();
	  j++;
	}
      list<win_container *>::iterator k=l_container.begin();
      while(k!=l_container.end())
	{
	  (*k)->update();
	  k++;
	}
    }
}


void win_container::show()
{
  visible=true;
}


void win_container::hide()
{
  visible=false;
}


void win_container::show_all()
{
  visible=true;
  if(wb) wb->show();


  list<win_label *>::iterator i=l_label.begin();
  while(i!=l_label.end())
    {
      (*i)->show();
      i++;
    }
  list<win_write *>::iterator j=l_write.begin();
  while(j!=l_write.end())
    {
      (*j)->show();
      j++;
    }
  list<win_container *>::iterator k=l_container.begin();
  while(k!=l_container.end())
    {
      (*k)->show();
      k++;
    }
}


void win_container::hide_all()
{
  visible=false;
  list<win_label *>::iterator i=l_label.begin();
  while(i!=l_label.end())
    {
      (*i)->hide();
      i++;
    }
  list<win_write *>::iterator j=l_write.begin();
  while(j!=l_write.end())
    {
      (*j)->hide();
      j++;
    }
  list<win_container *>::iterator k=l_container.begin();
  while(k!=l_container.end())
    {
      (*k)->hide();
      k++;
    }
}


drawing_area * win_container::get_da()
{
  return(da);
}

