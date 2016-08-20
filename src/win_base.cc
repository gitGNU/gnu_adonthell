/* 
   (C) Copyright 2000, 2001 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "win_base.h"
#include "win_manager.h"
#include "win_container.h"

win_base::win_base(): win_border(this),win_background(this)
{
  manager_ = NULL;
  
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

win_base::~win_base()
{
    if (manager_) manager_->remove (this);
    manager_ = NULL;
}

void win_base::set_container(win_container * wc)
{
  wb_father_=wc;
  
  update_position();
  
  update_align();
}

void win_base::update_position()
{
  
  if(wb_father_) {
    drawing_area::move(wb_father_->real_x() + x() + pad_x(), wb_father_->real_y() + y() + pad_y() );
  }
  else {
    drawing_area::move( x() + pad_x(), y() + pad_y() );		   
  }
}

void win_base::move(s_int16 tx, s_int16 ty)
{
  
  x_= tx;
  
  y_= ty;
  
  update_position();
}

void win_base::resize(u_int16 tl, u_int16 th)
{
  drawing_area::resize(tl, th);
  
  win_border::update();
  
  win_background::update();
}

bool win_base::update()
{
    if(win_event::update())
    {
      // if(focus_) ADDME: ajouter l'appel a update_input 
      on_update();
      
      return true;
    }
  return false;
}

bool win_base::input_update()
{
  return (focus_ && activate_);
}

bool win_base::draw()
{
  on_draw();
  
  if(visible_) on_draw_visible();
  
  return visible_;
}



void win_base::update_align()
{ 
  switch(align_)
    {
    case ALIGN_LEFT:
      move((wb_father_) ? ((win_container*)wb_father_)->space_with_border() : 0 , y() );
      break;
    case ALIGN_RIGHT:
      move(((wb_father_) ? wb_father_->length() : screen::length())-((wb_father_)?((win_container*)wb_father_)->space_with_border() : 0 ) - length() , y() );
      break;
    case ALIGN_CENTER:
      if(((wb_father_)?wb_father_->length():screen::length())>length()) 
	move((((wb_father_)?wb_father_->length():screen::length()) - length()) >>1,y());
      break;
    }
}

void win_base::set_manager (win_manager *m)
{
    manager_ = m;
}

