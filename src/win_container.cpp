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
#include "win_border.h"
#include "win_base.h"
#include "win_label.h"
#include "win_write.h"
#include "win_image.h"
#include "win_anim.h"
#include "win_select.h"
#include "win_container.h"



win_container::~win_container()
{
  list<win_label *>::iterator l=l_label.begin();
  while(l!=l_label.end()) delete *l++;

  list<win_container *>::iterator k=l_container.begin();
  while(k!=l_container.end()) delete *k++;
      
  list<win_write *>::iterator m=l_write.begin();
  while(m!=l_write.end()) delete *m++;

  list<win_image *>::iterator n=l_image.begin();
  while(n!=l_image.end()) delete *n++;

  list<win_anim *>::iterator o=l_anim.begin();
  while(o!=l_anim.end()) delete *o++;

  da->detach_drawing_area();
  delete da;
  
  dettach_select();
  
  sb=NULL;
  destroy_scrollbar();
}

void win_container::init_scrollbar()
{
  scroll_index=NULL;
  scroll_middle=NULL;
  scroll_bottom=NULL;
  scroll_top=NULL;
  scroll_bar_draw=NULL;
}


void win_container::destroy_scrollbar()
{
  if(scroll_index) delete scroll_index;
  if(scroll_bottom) delete scroll_bottom;
  if(scroll_middle) delete scroll_middle;
  if(scroll_top) delete scroll_top;
  if(scroll_bar_draw) delete scroll_bar_draw;
}
win_container::win_container(s_int16 x,s_int16 y,u_int16 l,u_int16 h):win_base(x,y,l,h)
{
   //normally used for the fisrt container
   l_label.clear();
   l_write.clear();
   l_container.clear();
   l_image.clear();
   da=new drawing_area(x,y,length,height);

   //init scroll bar image
   sb=NULL;
   pad_scroll= WIN_SCROLLBAR_PAD_DEFAULT;
   init_scrollbar();
}


win_container::win_container(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_container *tmpwc):win_base(tx,ty,tl,th,tmpwc)
{
   //children of another wc
   l_label.clear();
   l_write.clear();
   l_container.clear();
   l_image.clear();
   da=new drawing_area(real_x,real_y,length,height);
   da->assign_drawing_area(wc->get_drawing_area());

   //init scroll bar image
   sb=NULL;
   pad_scroll= WIN_SCROLLBAR_PAD_DEFAULT;
   init_scrollbar();
}

void win_container::is_object_max_y(s_int16 tmp)
{
  if(tmp>max_y_object)
    max_y_object=tmp;
}

void win_container::update_scrollbar_index()
{
  if(sb)
    cur_pos_index_scrollbar=(s_int16)(real_y+scroll_top->height + ((float)cur_pos_scrollbar/(float)(max_y_object-height)) * (height-scroll_bottom->height-scroll_top->height-scroll_index->height));
  
}

void win_container::set_scrollbar(win_scrollbar * tmpsb)
{
  sb=tmpsb;
  if(sb)
    {
      destroy_scrollbar();
      scroll_middle=new image();
      scroll_bar_draw=new image();
      scroll_top=new image();
      scroll_bottom=new image();
      scroll_index=new image();
      *scroll_index=*(sb->bar);
      *scroll_bottom=*(sb->bottom);
      *scroll_top=*(sb->top);
      cur_pos_scrollbar=0;
      update_scrollbar();
    }
}



void win_container::update_scrollbar()
{
  if(sb)
    {
      scroll_middle->resize(sb->middle->length,height-(sb->top->height)-(sb->bottom->height));
      scroll_middle->putbox_tile_img(sb->middle);
      scroll_bar_draw->resize(sb->middle->length,height);
      scroll_bar_draw->putbox_img(scroll_middle,
				  0,sb->top->height);
      scroll_bar_draw->putbox_img(sb->top,
				  0,0);
      scroll_bar_draw->putbox_img(sb->bottom,
				  0,height-sb->bottom->height);
      update_scrollbar_index();
    }
}


void win_container::draw_scrollbar()
{
  if(sb)
    {
      scroll_bar_draw->putbox_mask(real_x+length-scroll_bar_draw->length,
				 real_y,da);
      scroll_index->putbox_mask(real_x+length-scroll_bar_draw->length,
			      cur_pos_index_scrollbar,da);
    }
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

      list<win_anim *>::iterator o=l_anim.begin();
      while(o!=l_anim.end()) (*o++)->update_da();
    }
}


void win_container::move(s_int16 tx , s_int16 ty,bool move_by_scrollbar=false)
{  
   win_base::move(tx,ty,move_by_scrollbar);
   win_container::update_da();    
}

win_label * win_container::add_label(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_font *ft)
{
  win_label * tmp = new win_label(tx,ty,tl,th,ft,this);
  l_label.push_front(tmp);
  is_object_max_y(ty+th);//for scrollbar
  return (tmp);
}

win_write * win_container::add_write(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_font *ft)
{
  win_write * tmp = new win_write(tx,ty,tl,th,ft,this);
  l_write.push_front(tmp);
  is_object_max_y(ty+th);//for scrollbar
  return (tmp);
}

win_container * win_container::add_container(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th)
{
  win_container * tmp = new win_container(tx,ty,tl,th,this);
  l_container.push_front(tmp);
  is_object_max_y(ty+th);//for scrollbar
  return (tmp);
}

win_image * win_container::add_image(s_int16 tx,s_int16 ty,image * tpic)
{
  win_image * tmp = new win_image(tx,ty,tpic,this);
  l_image.push_front(tmp);
  is_object_max_y(ty+tmp->height);//for scrollbar
  return (tmp);
}

win_anim * win_container::add_anim(s_int16 tx,s_int16 ty,animation * tpic)
{
  win_anim * tmp = new win_anim(tx,ty,tpic,this);
  l_anim.push_front(tmp);
  is_object_max_y(ty+tmp->height);//for scrollbar
  return (tmp);
}

void win_container::remove(win_label * tmp)
{
  list<win_label *>::iterator i=l_label.begin();
  while(i!=l_label.end() && tmp!=(*i)) i++;
  if(i!=l_label.end()) l_label.erase(i);
  find_obj_max_y();//for scrollbar
}

void win_container::remove(win_write * tmp)
{
  list<win_write *>::iterator i=l_write.begin();
  while(i!=l_write.end() && tmp!=(*i)) i++;
  if(i!=l_write.end()) l_write.erase(i);
  find_obj_max_y();//for scrollbar
}

void win_container::remove(win_container * tmp)
{
  list<win_container *>::iterator i=l_container.begin();
  while(i!=l_container.end() && tmp!=(*i)) i++;
  if(i!=l_container.end()) l_container.erase(i);
  find_obj_max_y();//for scrollbar
}

void win_container::remove(win_image * tmp)
{
  list<win_image *>::iterator i=l_image.begin();
  while(i!=l_image.end() && tmp!=(*i)) i++;
  if(i!=l_image.end()) l_image.erase(i);
  find_obj_max_y();//for scrollbar
}

void win_container::remove(win_anim * tmp)
{
  list<win_anim *>::iterator i=l_anim.begin();
  while(i!=l_anim.end() && tmp!=(*i)) i++;
  if(i!=l_anim.end()) l_anim.erase(i);
  find_obj_max_y();//for scrollbar
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

      list<win_anim *>::iterator m=l_anim.begin();
      while(m!=l_anim.end()) (*m++)->draw();
      
      draw_scrollbar();
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

      list<win_anim *>::iterator m=l_anim.begin();
      while(m!=l_anim.end()) (*m++)->update();
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

  list<win_anim *>::iterator m=l_anim.begin();
  while(m!=l_anim.end()) (*m++)->show();      
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

  list<win_anim *>::iterator m=l_anim.begin();
  while(m!=l_anim.end()) (*m++)->hide(); 
}

void win_container::resize(u_int16 tl,u_int16 th)
{
  win_base::resize(tl,th);
  da->w=tl;
  da->h=th;
  update_scrollbar();
}

void win_container::attach_select(win_select * tmp)
{
  wselect=tmp;
}

void win_container::dettach_select()
{
  if(wselect)
    {
      wselect->remove(this);
      wselect=NULL;
    }
}

void win_container::up_scrollbar()
{
  if(sb)
    {
      s_int16 tmp;
      if(cur_pos_scrollbar>pad_scroll) tmp=pad_scroll;
      else tmp=cur_pos_scrollbar;
      cur_pos_scrollbar-=tmp;
      list<win_container *>::iterator k=l_container.begin();
      while(k!=l_container.end()) {(*k)->move((*k)->x,(*k)->y+tmp,true);*k++;}
      
      list<win_label *>::iterator i=l_label.begin();
      while(i!=l_label.end()) {(*i)->move((*i)->x,(*i)->y+tmp,true);*i++;}
      
      list<win_write *>::iterator j=l_write.begin();
      while(j!=l_write.end()) {(*j)->move((*j)->x,(*j)->y+tmp,true);*j++;}

      list<win_image *>::iterator l=l_image.begin();
      while(l!=l_image.end()) {(*l)->move((*l)->x,(*l)->y+tmp,true);*l++;}

      list<win_anim *>::iterator m=l_anim.begin();
      while(m!=l_anim.end()) {(*m)->move((*m)->x,(*m)->y+tmp,true);*m++;}

      update_scrollbar_index();
    }
}


void win_container::down_scrollbar()
{
  if(sb)
    {     
      s_int16 tmp;
      if(cur_pos_scrollbar>=max_y_object-height)
	tmp=0;
      else
	{
	  if(cur_pos_scrollbar + pad_scroll < (max_y_object-height)) tmp=pad_scroll;
	  else tmp=(max_y_object-height)-cur_pos_scrollbar;
	}
      
      cur_pos_scrollbar+=tmp;
      list<win_container *>::iterator k=l_container.begin();
      while(k!=l_container.end()) {(*k)->move((*k)->x,(*k)->y-tmp,true);*k++;}
      
      list<win_label *>::iterator i=l_label.begin();
      while(i!=l_label.end()) {(*i)->move((*i)->x,(*i)->y-tmp,true);*i++;}
      
      list<win_write *>::iterator j=l_write.begin();
      while(j!=l_write.end()) {(*j)->move((*j)->x,(*j)->y-tmp,true);*j++;}
      
      list<win_image *>::iterator l=l_image.begin();
      while(l!=l_image.end()) {(*l)->move((*l)->x,(*l)->y-tmp,true);*l++;}

      list<win_anim *>::iterator m=l_anim.begin();
      while(m!=l_anim.end()) {(*m)->move((*m)->x,(*m)->y-tmp,true);*m++;}

      update_scrollbar_index();
    }
}

void win_container::find_obj_max_y()
{
  max_y_object=0;
  list<win_container *>::iterator k=l_container.begin();
  while(k!=l_container.end()) 
    {
      if(max_y_object<(*k)->y+(*k)->height)
	max_y_object=(*k)->y+(*k)->height;
      *k++;
    }
      
      list<win_label *>::iterator i=l_label.begin();
      while(i!=l_label.end()) 
	{
	  if(max_y_object<(*i)->y+(*i)->height)
	    max_y_object=(*i)->y+(*i)->height;
	  *i++;
	}
      
      list<win_write *>::iterator j=l_write.begin();
      while(j!=l_write.end()) 
	{
	  if(max_y_object<(*j)->y+(*j)->height)
	    max_y_object=(*j)->y+(*j)->height;
	  *j++;
	}
      
      list<win_image *>::iterator l=l_image.begin();
      while(l!=l_image.end()) 
	{
	  if(max_y_object<(*l)->y+(*l)->height)
	    max_y_object=(*l)->y+(*l)->height;
	  *l++;
	}
}











