#include "atk_button_template.h"


atk_button_template::atk_button_template():background_(NULL), pressed_border_(NULL), released_border_(NULL)
{
  
}


void atk_button_template::load (igzstream & is)
{
  if (background_) delete background_;
  if (pressed_border_) delete pressed_border_;
  if (released_border_) delete released_border_;
  
  name_ << is;
  background_ = new atk_background_template();
  background_->load (is);
  pressed_border_ = new atk_border_template();
  pressed_border_->load (is);
  released_border_ = new atk_border_template();
  released_border_->load (is);
}

void atk_button_template::save (ogzstream & os)
{
  name_ >> os;
  background_->save (os);
  pressed_border_->save (os);
  released_border_->save (os);
}

std::string atk_button_template::get_name () const
{
  return name_;
}

void atk_button_template::set_name (const std::string & name)
{
  name_ = name;
}

void atk_button_template::set_released (atk_border_template * bd)
{
  if (released_border_) delete released_border_;
  released_border_ = bd;
}

void atk_button_template::set_pressed (atk_border_template * bd)
{
  if (pressed_border_) delete pressed_border_;
  pressed_border_ = bd;
  
}

void atk_button_template::set_background (atk_background_template * bg)
{
  if (background_) delete background_;
  background_ = bg;
}


atk_button_template::~atk_button_template()
{
  if (background_) delete background_;
  if (pressed_border_) delete pressed_border_;
  if (released_border_) delete released_border_;
}


atk_border_template * atk_button_template::get_pressed () const
{
  return pressed_border_;
}

atk_border_template * atk_button_template::get_released () const
{
  return released_border_;
}

atk_background_template * atk_button_template::get_background () const
{
  return background_;
}

