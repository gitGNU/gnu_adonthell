/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_theme.h"
#include "win_container.h"


win_container::win_container(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth):win_base(tx,ty,tl,th,wth)
{
  list_obj.clear();
  space_between_border_=WIN_SPACE_BETWEEN_BORDER;
  space_between_object_=WIN_SPACE_BETWEEN_OBJECT;
  justify_=WIN_JUSTIFY_NO;
  layout_=WIN_LAYOUT_NO;
  type_obj_=WIN_OBJ_CONTAINER;
}

void win_container::set_space_between_object(u_int16 tmp)
{
  space_between_object_=tmp;
  update_layout();
}

void win_container::set_space_between_border(u_int16 tmp)
{
  space_between_border_=tmp;
  update_layout();
}

win_container::~win_container()
{
  destroy();
}

void win_container::add(win_base * tmp)
{
  list_obj.push_back(tmp);

  tmp->wb_father_=this;
  tmp->update_real_position();

  set_justify(tmp,justify_);
  update_layout();
  if(draw_brightness_) tmp->set_draw_brightness(true);
}

void win_container::remove(win_base * tmp)
{
  list<win_base *>::iterator i=list_obj.begin();
  while(i!=list_obj.end() && tmp!=(*i)) i++;
  if(i!=list_obj.end()) 
    {
      tmp->wb_father_=NULL;
      list_obj.erase(i);
      update_layout();
    }
}

void win_container::remove_all()
{
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->wb_father_=NULL;
  list_obj.clear();
}

void win_container::destroy()
{
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    delete *i;
  list_obj.clear(); 
}

void win_container::update()
{
  win_base::update();
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->update();
}

void win_container::draw()
{
  if(!visible_) return;
  on_draw();
  assign_drawing_area();
  draw_background();
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->draw();
  draw_border();
  detach_drawing_area();
}

void win_container::set_visible_all(bool b)
{
  if(b)
    {
      visible_=true;
      for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
	(*i)->set_visible(true);
    }
  else
    {
      visible_=false;
      for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
	(*i)->set_visible(false);
    }
}

void win_container::set_draw_brightness(bool b)
{
  win_base::set_draw_brightness(b);
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->set_draw_brightness(b);
}

void win_container::set_justify(win_base * wb, u_int8 just)
{
  switch(just)
    {
    case WIN_JUSTIFY_LEFT:
      wb->move(space_between_border_,wb->y());
      break;
    case WIN_JUSTIFY_RIGHT:
      wb->move(length_-space_between_border_-wb->length(),wb->y());
      break;
    case WIN_JUSTIFY_CENTER:
      if(length_>wb->length()) 
	wb->move((length_-wb->length())>>1,wb->y());
      break;
    }
} 

void win_container::set_justify(u_int8  just)
{
  justify_=just;
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    set_justify((*i),just);
}

void win_container::set_layout(u_int8 lay)
{
  layout_=lay;
  update_layout();
}

void win_container::update_real_position()
{
  win_base::update_real_position();
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->update_real_position();
}

void win_container::move(s_int16 tx,s_int16 ty)
{
  win_base::move(tx,ty);
  update_real_position();
}

void win_container::update_layout()
{
  u_int16 indice=space_between_border_;
  switch(layout_)
    {
    case WIN_LAYOUT_LIST:
      for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
	{
	  (*i)->move((*i)->x(),indice);
	  indice+=(*i)->height()+space_between_object_;
	}
      break;
    default:
      break;
    }
}









