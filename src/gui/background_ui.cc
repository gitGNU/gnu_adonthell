#include <iostream>
#include "background_ui.h"


using namespace gui;

background_ui::background_ui (container * tmp) : btmpl_ (NULL)
{
  container_ = tmp;
}

void background_ui::draw (gfx::drawing_area * da, gfx::surface * sf)
{
  if (!container_ || !btmpl_ ) return;
  
  switch (btmpl_->get_type())
    {
    case background_template::COLOR:
      gfx::screen::display.fillrect (container_->get_x_real(), container_->get_y_real(),
				container_->get_length(), container_->get_height(),
				btmpl_->get_r(),btmpl_->get_g(), btmpl_->get_b(),  da);
      //     cout << container_->get_x_real() << " " << container_->get_y_real() <<" " << container_->get_length() << " " << container_->get_height() << endl;	
      break;
    }
}

void background_ui::resize ()
{
}

void background_ui::move()
{
}


void background_ui::set_background( background_template * btmp)
{
  btmpl_ = btmp;
}
