#include <iostream>
#include "atk_background_ui.h"


atk_background_ui::atk_background_ui (atk_container * tmp) : btmpl_ (NULL)
{
  container_ = tmp;
}

void atk_background_ui::draw (drawing_area * da = NULL, surface * sf = NULL)
{
  if (!container_ || !btmpl_ ) return;
  
  switch (btmpl_->get_type())
    {
    case atk_background_template::COLOR:
      screen::display.fillrect (container_->get_x_real(), container_->get_y_real(),
				container_->get_length(), container_->get_height(),
				btmpl_->get_r(),btmpl_->get_g(), btmpl_->get_b(),  da);
      //     cout << container_->get_x_real() << " " << container_->get_y_real() <<" " << container_->get_length() << " " << container_->get_height() << endl;	
      break;
    }
}

void atk_background_ui::resize ()
{
}

void atk_background_ui::move()
{
}


void atk_background_ui::set_background( atk_background_template * btmp)
{
  btmpl_ = btmp;
}
