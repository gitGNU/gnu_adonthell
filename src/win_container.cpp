#include <iostream.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"
#include "win_base.h"
#include "win_write.h"
#include "win_label.h"
#include "win_image.h"
#include "win_border.h"
#include "win_container.h"


win_container::~win_container()
{
  l_label.clear();
  l_write.clear();
  l_container.clear();
  l_image.clear();
  delete da;
}

win_container::win_container(u_int16 x,u_int16 y,u_int16 l,u_int16 h):win_base(x,y,l,h)
{
  l_label.clear();
  l_write.clear();
  l_container.clear();
  l_image.clear();
  da=new drawing_area(x,y,length,height);
}


win_container::win_container(u_int16 tx,u_int16 ty,u_int16 tl,u_int16 th,win_container *tmpwc):win_base(tmpwc->get_x()+tx,tmpwc->get_y()+ty,tl,th,tmpwc)
{
  l_label.clear();
  l_write.clear();
  l_container.clear();
  l_image.clear();
  da=new drawing_area(wc->get_x()+x,wc->get_y()+y,length,height);
}

void win_container::move(u_int16 tx , u_int16 ty)
{
  /*if(wc)
    {
      x=wc->get_x()+tx;
      y=wc->get_y()+ty;
    }
  else
    {
      x=tx;
      y=ty;
      }*/
  
  win_base::move(tx,ty);
  if(da)
    {
      if(wc)
	{
	  da->x=tx+wc->get_x();
	  da->y=ty+wc->get_y();
	}
      else
	{
	  da->x=tx;
	  da->y=ty;
	}
    }
}

win_label * win_container::add_label(u_int16 tx,u_int16 ty,u_int16 tl,u_int16 th,win_font *ft)
{
  win_label * tmp = new win_label(tx,ty,tl,th,ft,this);
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

win_image * win_container::add_image(u_int16 tx,u_int16 ty,image * tpic)
{
  win_image * tmp = new win_image(tx,ty,tpic,this);
  l_image.push_front(tmp);
  return (tmp);
}

/*
win_border * win_container::set_border(char * rep)
{
  if(wb) delete wb;
  // wb->assign_drawing_area(da);
  wb=new win_border(rep,this);
  return(wb);
}*/
/*
win_background * win_container::set_background(char * rep)
{
  if(wback) delete wback;
  // wb->assign_drawing_area(da);
  wback=new win_background(rep,this);
  return(wback);
}
*/

void win_container::remove(win_label * tmp)
{
  list<win_label *>::iterator i=l_label.begin();
  while(i!=l_label.end() && tmp!=(*i))
    i++;
  if(i!=l_label.end())
    l_label.erase(i);
}

void win_container::remove(win_write * tmp)
{
  list<win_write *>::iterator i=l_write.begin();
  while(i!=l_write.end() && tmp!=(*i))
    i++;
  if(i!=l_write.end())
    l_write.erase(i);
}

void win_container::remove(win_container * tmp)
{
  list<win_container *>::iterator i=l_container.begin();
  while(i!=l_container.end() && tmp!=(*i))
    i++;
  if(i!=l_container.end())
    l_container.erase(i);
}

void win_container::remove(win_image * tmp)
{
  list<win_image *>::iterator i=l_image.begin();
  while(i!=l_image.end() && tmp!=(*i))
    i++;
  if(i!=l_image.end())
    l_image.erase(i);
}

void win_container::draw()
{
  if(visible)
    { 
      draw_background();
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
      
      list<win_image *>::iterator l=l_image.begin();
      while(l!=l_image.end())
	{
	  (*l)->draw();
	  l++;
	}    
      draw_border();
    }
}

void win_container::update()
{
  if(visible)
    {
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
      list<win_image *>::iterator l=l_image.begin();
      while(l!=l_image.end())
	{
	  (*l)->update();
	  l++;
	}      
    }
}


void win_container::show_all()
{
  visible=true;
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
   list<win_image *>::iterator l=l_image.begin();
   while(l!=l_image.end())
     {
       (*l)->show();
       l++;
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
  list<win_image *>::iterator l=l_image.begin();
  while(l!=l_image.end())
    {
      (*l)->hide();
      l++;
    }      
}

void win_container::resize(u_int16 tl,u_int16 th)
{
  win_base::resize(tl,th);
  da->w=tl;
  da->h=th;
}






