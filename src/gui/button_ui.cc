/*
   $Id$

   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "button_ui.h"


using namespace gui;

button_ui::button_ui( button * but): pressed_border_ (NULL), released_border_ (NULL),background_ (NULL), btempl_ (NULL)
{
  button_ = but;
}


void button_ui::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  if (!btempl_ || !button_) return;
  
  if( button_->get_pressed()) pressed_border_->draw(da, sf);
  else released_border_->draw(da, sf);
  background_->draw (da, sf);
}


void button_ui::resize()
{
  if (!btempl_ || !button_) return;
  
  pressed_border_->resize();
  released_border_->resize();
  background_->resize();
}


void button_ui::move()
{
  if (!btempl_ || !button_) return;
  
  pressed_border_->move();
  released_border_->move();
  background_->move();
}


void button_ui::set_button (button_template * but)
{
  if ( pressed_border_ ) delete pressed_border_;
  if ( released_border_ ) delete released_border_;
  if (background_ ) delete background_;

  btempl_ = but;

  pressed_border_ = new border_ui (button_);
  pressed_border_->set_border (but->get_pressed());
  released_border_ = new border_ui (button_);
  released_border_->set_border (but->get_released());
  background_ = new background_ui (button_);
  background_->set_background (but->get_background());
}


button_ui::~button_ui()
{
  if ( pressed_border_ ) delete pressed_border_;
  if ( released_border_ ) delete released_border_;
  if (background_) delete background_;
}
