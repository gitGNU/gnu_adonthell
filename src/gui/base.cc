/* 
   $Id$ 

   (C) Copyright 2000, 2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.
   
   See the COPYING file for more details
*/


#include "base.h"
#include "container.h"

using namespace gui;


base::base(): border(this),background(this)
{
  wb_father_= NULL;
  
  pad_y_ = pad_x_ = 0;
  
  move(0,0);
  
  set_visible(false);

  set_focus(false);

  set_activate(false);
  
  set_brightness(false);
  
  set_trans(false);

  set_can_be_selected(true);

  set_align(ALIGN_NONE);
}


base::~base()
{
}


void base::set_container(container * wc)
{
  /* define the father */
  wb_father_=wc;
  
  /* update position */
  update_position();
  
  /* update the alignement */
  update_align();
}


void base::update_position()
{
  if(wb_father_) {
    drawing_area::move(wb_father_->real_x() + x() + pad_x(), wb_father_->real_y() + y() + pad_y() );
  }
  else {
    drawing_area::move( x() + pad_x(), y() + pad_y() );		   
  }
}


void base::move(s_int16 tx, s_int16 ty)
{
  
  x_= tx;
  
  y_= ty;
  
  update_position();
}


void base::resize(u_int16 tl, u_int16 th)
{
  drawing_area::resize(tl, th);
  
  border::update();
  
  background::update();
}

bool base::update()
{
    if(event::update())
    {
      // if(focus_) ADDME: ajouter l'appel a update_input 
      on_update();
      
      return true;
    }
  return false;
}


bool base::input_update()
{
  return (focus_ && activate_);
}


bool base::draw()
{
  on_draw();
  
  if(visible_) on_draw_visible();
  
  return visible_;
}


void base::update_align()
{ 
  switch(align_)
    {
    case ALIGN_LEFT:
      move((wb_father_) ? ((container*)wb_father_)->space_with_border() : 0 , y() );
      break;
    case ALIGN_RIGHT:
      move(((wb_father_) ? wb_father_->length() : gfx::screen::length())-((wb_father_)?((container*)wb_father_)->space_with_border() : 0 ) - length() , y() );
      break;
    case ALIGN_CENTER:
      if(((wb_father_)?wb_father_->length():gfx::screen::length())>length()) 
	move((((wb_father_)?wb_father_->length():gfx::screen::length()) - length()) >>1,y());
      break;
    }
}
