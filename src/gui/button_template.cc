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

#include "button_template.h"

using namespace gui;

button_template::button_template():background_(NULL), pressed_border_(NULL), released_border_(NULL)
{
  
}


void button_template::load (igzstream & is)
{
  if (background_) delete background_;
  if (pressed_border_) delete pressed_border_;
  if (released_border_) delete released_border_;
  
  name_ << is;
  background_ = new background_template();
  background_->load (is);
  pressed_border_ = new border_template();
  pressed_border_->load (is);
  released_border_ = new border_template();
  released_border_->load (is);
}

void button_template::save (ogzstream & os)
{
  name_ >> os;
  background_->save (os);
  pressed_border_->save (os);
  released_border_->save (os);
}

std::string button_template::get_name () const
{
  return name_;
}

void button_template::set_name (const std::string & name)
{
  name_ = name;
}

void button_template::set_released (border_template * bd)
{
  if (released_border_) delete released_border_;
  released_border_ = bd;
}

void button_template::set_pressed (border_template * bd)
{
  if (pressed_border_) delete pressed_border_;
  pressed_border_ = bd;
  
}

void button_template::set_background (background_template * bg)
{
  if (background_) delete background_;
  background_ = bg;
}


button_template::~button_template()
{
  if (background_) delete background_;
  if (pressed_border_) delete pressed_border_;
  if (released_border_) delete released_border_;
}


border_template * button_template::get_pressed () const
{
  return pressed_border_;
}

border_template * button_template::get_released () const
{
  return released_border_;
}

background_template * button_template::get_background () const
{
  return background_;
}

