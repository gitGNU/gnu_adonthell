/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "container.h"

using namespace gui;

container::container()
{
  move(0,0);  
  /* no layout */
  set_layout(NO_LAYOUT);

  /* define space between border */
  set_space_with_border(SPACE_WITH_BORDER);

  /* define space between objet */
  set_space_with_object(SPACE_WITH_OBJECT);

  /* objet focused inside the container */
  focus_object_ = NULL;
}

container::~container()
{
  destroy();
}

void container::move(s_int16 tx, s_int16 ty)
{
  base::move(tx,ty);
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    (*i)->update_position();
}

void container::resize(u_int16 tl, u_int16 th)
{
  base::resize(tl,th);

  update_layout();
}

void container::update_position()
{
  base::update_position();
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    (*i)->update_position();
}

void container::add(base * w)
{
  list_wb_.push_back(w);
  
  w->set_container(this);
  
  update_layout();
}

void container::remove(base * w)
{
  list_wb_.remove(w);
  
  w->set_container(NULL);

  update_layout();
}

void container::remove_all()
{
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    {
      (*i)->set_container(NULL);
      list_wb_.erase(i);
    }
}

void container::destroy()
{
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    delete *i;
  list_wb_.clear();
}

bool container::update()
{
    if(base::update())
    {
      for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
	{
	  if(!(*i)->update())
	    {
	      list_wb_.erase(i);
	      delete *i--;
	    }
	}
      return true;
    } 
  return false;
}


bool container::input_update()
{
  if(base::input_update())
    {
      if(focus_object_) focus_object_->input_update();
      return true;
    }
  return false;
}


void container::set_focus_object(base * f)
{
  if(focus_object_) focus_object_->set_focus(false);
  if(f) f->set_focus(true);
  focus_object_ = f;
}

void container::set_brightness(bool b)
{
  base::set_brightness(b);
  
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    (*i)->set_brightness(b);
}

void container::set_trans(bool b)
{
  base::set_trans(b);
  
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    (*i)->set_trans(b);
}

void container::set_visible_all(bool b)
{
  set_visible(b);
   for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
     (*i)->set_visible(b);
}



bool container::draw()
{
  if(base::draw()) 
    {
      assign_drawing_area(wb_father_); 

      background::draw(this);   
      
      for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
	(*i)->draw();
      
      border::draw(wb_father_);
      
      detach_drawing_area();
      
      return true;
    }
  return false;
}



void container::update_layout()
{
  
  u_int16 indice_h=space_with_border_;
  
  switch(layout_)
    {
    case LIST_LAYOUT:
      
      for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
	{
	  (*i)->move((*i)->x(),indice_h);
	  
	  indice_h+=(*i)->height()+space_with_object_;
	}
      
      break;
    }
}
