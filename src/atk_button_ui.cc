#include "atk_button_ui.h"


atk_button_ui::atk_button_ui( atk_button * but): pressed_border_ (NULL), released_border_ (NULL),background_ (NULL), btempl_ (NULL)
{
  button_ = but;
}


void atk_button_ui::draw (drawing_area * da, surface * sf)
{
  if (!btempl_ || !button_) return;
  
  if( button_->get_pressed()) pressed_border_->draw(da, sf);
  else released_border_->draw(da, sf);
  background_->draw (da, sf);
}


void atk_button_ui::resize()
{
  if (!btempl_ || !button_) return;
  
  pressed_border_->resize();
  released_border_->resize();
  background_->resize();
}


void atk_button_ui::move()
{
  if (!btempl_ || !button_) return;
  
  pressed_border_->move();
  released_border_->move();
  background_->move();
}


void atk_button_ui::set_button (atk_button_template * but)
{
  if ( pressed_border_ ) delete pressed_border_;
  if ( released_border_ ) delete released_border_;
  if (background_ ) delete background_;

  btempl_ = but;

  pressed_border_ = new atk_border_ui (button_);
  pressed_border_->set_border (but->get_pressed());
  released_border_ = new atk_border_ui (button_);
  released_border_->set_border (but->get_released());
  background_ = new atk_background_ui (button_);
  background_->set_background (but->get_background());
}


atk_button_ui::~atk_button_ui()
{
  if ( pressed_border_ ) delete pressed_border_;
  if ( released_border_ ) delete released_border_;
  if (background_) delete background_;
}
