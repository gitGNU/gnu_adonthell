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
#include "win_select.h"
#include "win_container.h"


win_container::~win_container()
{
   l_label.clear();
   l_write.clear();
   l_container.clear();
   l_image.clear();
   da->detach_drawing_area();
   delete da;
   if(wselect)
     {
       wselect->remove(this);
       wselect=NULL;
     }
}


win_container::win_container(u_int16 x,u_int16 y,u_int16 l,u_int16 h):win_base(x,y,l,h)
{
   //normally used for the fisrt container
   l_label.clear();
   l_write.clear();
   l_container.clear();
   l_image.clear();
   da=new drawing_area(x,y,length,height);
}


win_container::win_container(u_int16 tx,u_int16 ty,u_int16 tl,u_int16 th,win_container *tmpwc):win_base(tx,ty,tl,th,tmpwc)
{
   //children of another wc
   l_label.clear();
   l_write.clear();
   l_container.clear();
   l_image.clear();
   da=new drawing_area(real_x,real_y,length,height);
   da->assign_drawing_area(wc->get_drawing_area());
}

void win_container::update_da()
{
  if(da)//normally no need this test
     {
	     win_base::update_da();
	     da->x=real_x;
	     da->y=real_y;
	     //move children drawing area
	     list<win_container *>::iterator k=l_container.begin();
	     while(k!=l_container.end()) (*k++)->update_da();
	        
	     list<win_label *>::iterator l=l_label.begin();
	     while(l!=l_label.end())(*l++)->update_da();
	
	     list<win_write *>::iterator m=l_write.begin();
	     while(m!=l_write.end()) (*m++)->update_da();

	     list<win_image *>::iterator n=l_image.begin();
	     while(n!=l_image.end()) (*n++)->update_da();
     }
}


void win_container::move(u_int16 tx , u_int16 ty)
{  
   win_base::move(tx,ty);
   win_container::update_da();    
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

void win_container::remove(win_label * tmp)
{
  list<win_label *>::iterator i=l_label.begin();
  while(i!=l_label.end() && tmp!=(*i)) i++;
  if(i!=l_label.end()) l_label.erase(i);
}

void win_container::remove(win_write * tmp)
{
  list<win_write *>::iterator i=l_write.begin();
  while(i!=l_write.end() && tmp!=(*i)) i++;
  if(i!=l_write.end()) l_write.erase(i);
}

void win_container::remove(win_container * tmp)
{
  list<win_container *>::iterator i=l_container.begin();
  while(i!=l_container.end() && tmp!=(*i)) i++;
  if(i!=l_container.end()) l_container.erase(i);
}

void win_container::remove(win_image * tmp)
{
  list<win_image *>::iterator i=l_image.begin();
  while(i!=l_image.end() && tmp!=(*i)) i++;
  if(i!=l_image.end()) l_image.erase(i);
}

void win_container::draw()
{
  if(visible)
    { 
      draw_background();
      list<win_container *>::iterator k=l_container.begin();
      while(k!=l_container.end()) (*k++)->draw();
	
      list<win_label *>::iterator i=l_label.begin();
      while(i!=l_label.end()) (*i++)->draw();
	
      list<win_write *>::iterator j=l_write.begin();
      while(j!=l_write.end()) (*j++)->draw();

      list<win_image *>::iterator l=l_image.begin();
      while(l!=l_image.end()) (*l++)->draw();
      
      draw_border();
    }
}

void win_container::update()
{
  if(visible)
    {
      list<win_label *>::iterator i=l_label.begin();
      while(i!=l_label.end()) (*i++)->update();
	
      list<win_write *>::iterator j=l_write.begin();
      while(j!=l_write.end()) (*j++)->update();
	
      list<win_container *>::iterator k=l_container.begin();
      while(k!=l_container.end()) (*k++)->update();

      list<win_image *>::iterator l=l_image.begin();
      while(l!=l_image.end()) (*l++)->update();
    }
}


void win_container::show_all()
{
  visible=true;
  list<win_label *>::iterator i=l_label.begin();
  while(i!=l_label.end()) (*i++)->show();
  
  list<win_write *>::iterator j=l_write.begin();
  while(j!=l_write.end()) (*j++)->show();
  
  list<win_container *>::iterator k=l_container.begin();
  while(k!=l_container.end()) (*k++)->show_all();
  
  list<win_image *>::iterator l=l_image.begin();
  while(l!=l_image.end()) (*l++)->show();      
}


void win_container::hide_all()
{
  visible=false;
  list<win_label *>::iterator i=l_label.begin();
  while(i!=l_label.end()) (*i++)->hide();
 
  list<win_write *>::iterator j=l_write.begin();
  while(j!=l_write.end()) (*j++)->hide();
  
  list<win_container *>::iterator k=l_container.begin();
  while(k!=l_container.end()) (*k++)->hide();
     
  list<win_image *>::iterator l=l_image.begin();
  while(l!=l_image.end()) (*l++)->hide(); 
}

void win_container::resize(u_int16 tl,u_int16 th)
{
  win_base::resize(tl,th);
  da->w=tl;
  da->h=th;
}













