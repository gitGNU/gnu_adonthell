/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "scroll.h"
#include "keys.h"
using namespace gui;

scroll::scroll():scrollbar(this)
{
  
  cursor_y_=0;
  
  max_amplitude_=0;
  
  cur_amplitude_=0;
  
  index_pad_=PAD_DEFAULT;

  set_auto_scrollbar(false);
  
  set_auto_refresh(false);

}


bool scroll::down()
{
  
  if( max_amplitude_ == 0 ) return false;
  
  if( cur_amplitude_ + index_pad_ > max_amplitude_ ) cur_amplitude_ = max_amplitude_;  
  else cur_amplitude_ += index_pad_;
  
  update_amplitude();
  
  on_down();
  
  return true;

}


void scroll::resize(u_int16 tl,u_int16 th)
{
  container::resize(tl,th);
  
  find_amplitude();
  
  scrollbar::update_back();
  
  scrollbar::update_bar();
}


bool scroll::up()
{
  if( max_amplitude_==0 ) return false ;
  
  if( cur_amplitude_-index_pad_ < 0) cur_amplitude_ = 0;
  else cur_amplitude_ -= index_pad_;
  
  update_amplitude();
  
  on_up();
  
  return true;
}

void scroll::set_pos (const u_int8 pos) 
{
    cur_amplitude_ = (u_int16)((((float) max_amplitude_) / 255) * pos); 
    update_amplitude(); 
}

void scroll::update_amplitude()
{
  for(lwb::iterator i=list_wb_.begin() ; i!=list_wb_.end() ; i++) 
    {  
      (*i)->pad_y() = -cur_amplitude_ ;  
      (*i)->update_position();
    }
  
  cursor_y_=(u_int16) ( (float) ((float)(height() - scrollbar::height_bar() ) / max_amplitude_ ) * cur_amplitude_ );
}


void scroll::set_space_between_border(u_int16 tmp)
{
  
  container::set_space_with_border(tmp);
  
  find_amplitude();
  
  scrollbar::update_bar();

}


void scroll::set_space_between_object(u_int16 tmp)
{
  container::set_space_with_object(tmp);
  
  find_amplitude();
  
  scrollbar::update_bar();
}


void scroll::add(base *wb)
{
  container::add(wb);
  
  find_amplitude();
  
  scrollbar::update_bar();
}


void scroll::remove(base *wb)
{
  container::remove(wb);
  
  find_amplitude();
  
  scrollbar::update_bar();
}


void scroll::remove_all()
{
  container::remove_all();
  
  max_amplitude_=0;
  
  cur_amplitude_=0;
  
  scrollbar::update_bar();
}


void scroll::destroy()
{
  container::destroy();
  
  max_amplitude_=0;
  
  cur_amplitude_=0;
  
  scrollbar::update_bar();
}


bool scroll::draw()
{
  if(base::draw())
    {
      assign_drawing_area(wb_father_); 
      
      background::draw(this);   
      
      for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
	(*i)->draw();
      
      scrollbar::draw(wb_father_);

      border::draw(wb_father_);
      
      detach_drawing_area();
      
      return true;
    }
  return false;
}


bool scroll::update()
{
  if(container::update())
    {
      if(auto_scrollbar_)
	{
	  u_int16 old = amplitude();
          find_amplitude(); 
	  if(old != amplitude())
	    {
	      scrollbar::set_visible_scrollbar(amplitude()!=0);
	      scrollbar::update_bar();
	    }
	}
      else if(auto_refresh_) 
	{
	  u_int16 old = amplitude(); find_amplitude(); 
	  if(old != amplitude())
	    scrollbar::update_bar();
	} 
      return true;
    }
  return false;
}


bool scroll::input_update()
{
  if(container::input_update())
    {
      if(focus_object_) return true;
      if(input::is_pushed(keys::KEY_UP)) up();
      if(input::is_pushed(keys::KEY_DOWN)) down();
      return true;
    }
  return false;
}


void scroll::find_amplitude()
{
  //search the max y object to calcul the amplitude
  max_amplitude_ = cursor_y_ = cur_amplitude_ = 0;
  
  for(lwb::iterator i=list_wb_.begin() ; i!=list_wb_.end() ; i++) 
    if((*i)->y() + (*i)->height() > height() - space_with_border() && (*i)->y() + (*i)->height() - height() + space_with_border() > max_amplitude_ )
      max_amplitude_ = (*i)->y() + (*i)->height() - height() + space_with_border(); 
}
