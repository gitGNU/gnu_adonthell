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
  destroy_all();
  
  da->detach_drawing_area();
  delete da;
  
  //remove scrollbar
  sb=NULL;
  destroy_scrollbar();
  
  //tree_select
  if(tree_select)
    if(tree_select->father_select) tree_select->father_select->remove_select(tree_select);
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
  //init the l_base
  l_base.clear();
  //create a drawing area
  da=new drawing_area(x,y,length,height);

  //init scroll bar image
  sb=NULL;
  pad_scroll= WIN_SCROLLBAR_PAD_DEFAULT;
  init_scrollbar();
  tree_select=NULL;
  func_update=NULL;
}


win_container::win_container(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_container *tmpwc):win_base(tx,ty,tl,th,tmpwc)
{
 
  l_base.clear();
  //creating a drawing area
  da=new drawing_area(real_x,real_y,length,height);
  da->assign_drawing_area(wc->get_drawing_area());
  //init scroll bar image
  sb=NULL;
  pad_scroll= WIN_SCROLLBAR_PAD_DEFAULT;
  init_scrollbar();
  tree_select=NULL;
  
  func_update=NULL;
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
      list<win_base *>::iterator o=l_base.begin();
      while(o!=l_base.end()) (*o++)->update_da();
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
  l_base.push_front(tmp);
  is_object_max_y(ty+th);//for scrollbar
  return (tmp);
}

win_write * win_container::add_write(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_font *ft)
{
  win_write * tmp = new win_write(tx,ty,tl,th,ft,this);
  l_base.push_front(tmp);
  
  is_object_max_y(ty+th);//for scrollbar
  return (tmp);
}

win_container * win_container::add_container(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th)
{
  win_container * tmp = new win_container(tx,ty,tl,th,this);
  l_base.push_front(tmp);
  is_object_max_y(ty+th);//for scrollbar
  return (tmp);
}

win_image * win_container::add_image(s_int16 tx,s_int16 ty,image * tpic)
{
  win_image * tmp = new win_image(tx,ty,tpic,this);
    l_base.push_front(tmp);
  is_object_max_y(ty+tmp->height);//for scrollbar
  return (tmp);
}

win_anim * win_container::add_anim(s_int16 tx,s_int16 ty,animation * tpic)
{
  win_anim * tmp = new win_anim(tx,ty,tpic,this);
  l_base.push_front(tmp);
  is_object_max_y(ty+tmp->height);//for scrollbar
  return (tmp);
}

void win_container::remove(win_base * tmp)
{
  list<win_base *>::iterator i=l_base.begin();
  while(i!=l_base.end() && tmp!=(*i)) i++;
  if(i!=l_base.end()) l_base.erase(i);
  find_obj_max_y();//for scrollbar
}

void win_container::draw()
{
  if(visible)
    { 
      draw_background();
      list<win_base *>::iterator o=l_base.begin();
      while(o!=l_base.end()) (*o++)->draw();
      draw_scrollbar();
      draw_border();
    }
}

void win_container::update()
{

  if(visible)
    {
      if(func_update) func_update();
      list<win_base *>::iterator o=l_base.begin();
      while(o!=l_base.end()) (*o++)->update();
    }
}


void win_container::show_all()
{
  visible=true;
  list<win_base *>::iterator o=l_base.begin();
  while(o!=l_base.end()) (*o++)->show();
}


void win_container::hide_all()
{
  visible=false;  
  list<win_base *>::iterator o=l_base.begin();
  while(o!=l_base.end()) (*o++)->hide();
}

void win_container::resize(u_int16 tl,u_int16 th)
{
  win_base::resize(tl,th);
  da->w=tl;
  da->h=th;
  update_scrollbar();
}

void win_container::up_scrollbar()
{
  if(sb)
    {
      s_int16 tmp;
      if(cur_pos_scrollbar>pad_scroll) tmp=pad_scroll;
      else tmp=cur_pos_scrollbar;
      cur_pos_scrollbar-=tmp;
      list<win_base *>::iterator l=l_base.begin();
      while(l!=l_base.end()) {(*l)->move((*l)->x,(*l)->y+tmp,true);*l++;}
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
      list<win_base *>::iterator l=l_base.begin();
      while(l!=l_base.end()) {(*l)->move((*l)->x,(*l)->y-tmp,true);*l++;}
      update_scrollbar_index();
      
   }
}


void win_container::find_obj_max_y()
{
  max_y_object=0;
  list<win_base *>::iterator o=l_base.begin();
  while(o!=l_base.end()) 
    {
      if(max_y_object<(*o)->y+(*o)->height)
	max_y_object=(*o)->y+(*o)->height;
      *o++;
    }
}

/*
void win_container::destroy_all_label()
{
  list<win_label *>::iterator l=l_label.begin();
  while(l!=l_label.end()) delete *l++;
  l_label.clear();
}

void win_container::destroy_all_container()
{
  list<win_container *>::iterator k=l_container.begin();
  while(k!=l_container.end()) delete *k++;
  l_container.clear();
}

void win_container::destroy_all_write()
{
  list<win_write *>::iterator m=l_write.begin();
  while(m!=l_write.end()) delete *m++;
  l_write.clear();
}

void win_container::destroy_all_image()
{
  list<win_image *>::iterator n=l_image.begin();
  while(n!=l_image.end()) delete *n++;
  l_image.clear();
}

void win_container::destroy_all_anim()
{
  list<win_anim *>::iterator o=l_anim.begin();
  while(o!=l_anim.end()) delete *o++;
  l_anim.clear();
}
*/

void win_container::destroy_all()
{
  list<win_base *>::iterator o=l_base.begin();
  while(o!=l_base.end()) delete *o++;
  l_base.clear();
}

void win_container::attach_update_function(void (*f)())
{
  func_update=f;
}
