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

#include <iostream>
#include "border_ui.h"

using namespace gui;

border_ui::border_ui (container * ac) : container_ (ac), btempl_ (NULL) 
{    
  /*init tab */
  for (int i = 0; i < border_template::_LAST; i++)
    img_[i] = NULL;   
}


void border_ui::draw (gfx::drawing_area* da, gfx::surface * sf)
{
  if (!btempl_ || !container_) return;  
  
  /* BORDER, to draw border we use the template picture */

  img_[border_template::B_TOP]->assign_drawing_area(da);
  btempl_->get_border (border_template::B_TOP)->draw (img_[border_template::B_TOP]->x(),
							  img_[border_template::B_TOP]->y(),
							  img_[border_template::B_TOP] , sf);
  img_[border_template::B_TOP]->detach_drawing_area();
  
  img_[border_template::B_BOTTOM]->assign_drawing_area(da);
  btempl_->get_border (border_template::B_BOTTOM)->draw (img_[border_template::B_BOTTOM]->x(),
							     img_[border_template::B_BOTTOM]->y(),
							     img_[border_template::B_BOTTOM], sf);
  img_[border_template::B_BOTTOM]->detach_drawing_area();

  img_[border_template::B_LEFT]->assign_drawing_area(da);
  btempl_->get_border (border_template::B_LEFT)->draw (img_[border_template::B_LEFT]->x(),
							   img_[border_template::B_LEFT]->y(),
							   img_[border_template::B_LEFT], sf);
  img_[border_template::B_LEFT]->detach_drawing_area();
  
  img_[border_template::B_RIGHT]->assign_drawing_area(da);
  btempl_->get_border (border_template::B_RIGHT)->draw (img_[border_template::B_RIGHT]->x(),
							    img_[border_template::B_RIGHT]->y(),
							    img_[border_template::B_RIGHT], sf);
  img_[border_template::B_RIGHT]->detach_drawing_area();

  
  /* NOW DRAW CORNER */
  img_[border_template::C_TL]->assign_drawing_area(da);
  btempl_->get (border_template::C_TL)->draw (img_[border_template::C_TL]->x(),
							 img_[border_template::C_TL]->y(),
							 img_[border_template::C_TL], sf);
  img_[border_template::C_TL]->detach_drawing_area();

  img_[border_template::C_TR]->assign_drawing_area(da);
  btempl_->get (border_template::C_TR)->draw (img_[border_template::C_TR]->x(),
							 img_[border_template::C_TR]->y(),
							 img_[border_template::C_TR], sf);
  img_[border_template::C_TR]->detach_drawing_area();

  img_[border_template::C_BL]->assign_drawing_area(da);
  btempl_->get (border_template::C_BL)->draw (img_[border_template::C_BL]->x(),
							 img_[border_template::C_BL]->y(),
							 img_[border_template::C_BL], sf);
  img_[border_template::C_BL]->detach_drawing_area();

  img_[border_template::C_BR]->assign_drawing_area(da);
  btempl_->get (border_template::C_BR)->draw (img_[border_template::C_BR]->x(),
							 img_[border_template::C_BR]->y(),
							 img_[border_template::C_BR], sf);
  img_[border_template::C_BR]->detach_drawing_area();
}


void border_ui::move ()
{
  if (!btempl_ || !container_) return;  
  
  /* move the border */
  img_[border_template::B_TOP]->move (container_->get_x_real(), container_->get_y_real() - img_[border_template::B_TOP]->height());
  img_[border_template::B_BOTTOM]->move (container_->get_x_real(), container_->get_y_real() + container_->get_height());
  img_[border_template::B_LEFT]->move (container_->get_x_real() - img_[border_template::B_LEFT]->length(), container_->get_y_real());
  img_[border_template::B_RIGHT]->move (container_->get_x_real() + container_->get_length(), container_->get_y_real());

  /* now move corner */
  img_[border_template::C_TL]->move ( container_->get_x_real() - (img_[border_template::C_TL]->length()>>1) - 
					  (img_[border_template::B_LEFT]->length()>>1),
					  container_->get_y_real() - (img_[border_template::C_TL]->height()>>1) -
					  (img_[border_template::B_TOP]->height()>>1));
  img_[border_template::C_TR]->move ( container_->get_x_real() - (img_[border_template::C_TR]->length()>>1) + 
					  (img_[border_template::B_RIGHT]->length()>>1) + container_->get_length(),
					  container_->get_y_real() - (img_[border_template::C_TR]->height()>>1) -
					  (img_[border_template::B_TOP]->height()>>1));
  img_[border_template::C_BL]->move ( container_->get_x_real() - (img_[border_template::C_BL]->length()>>1) - 
					  (img_[border_template::B_LEFT]->length()>>1),
					  container_->get_y_real() - (img_[border_template::C_BL]->height()>>1) +
					  (img_[border_template::B_BOTTOM]->height()>>1) + container_->get_height());
  img_[border_template::C_BR]->move ( container_->get_x_real() - (img_[border_template::C_BR]->length()>>1) + 
					  (img_[border_template::B_RIGHT]->length()>>1) + container_->get_length(),
					  container_->get_y_real() - (img_[border_template::C_BR]->height()>>1) +
					  (img_[border_template::B_BOTTOM]->height()>>1) + container_->get_height());

  std::cout << "border_ui::move x:" << container_->get_x_real () << " y:" << container_->get_y_real () << " l:" 
	    << container_->get_length() << " h:" << container_->get_height() << std::endl;
}


bool border_ui::is_in_ctl(s_int32 x, s_int32 y)
{
  if( !btempl_ || !container_) return false;

  return img_[border_template::C_TL]->point_belong (x, y);
}



bool border_ui::is_in_ctr(s_int32 x, s_int32 y)
{
  if( !btempl_ || !container_) return false;
  
  return img_[border_template::C_TR]->point_belong (x, y);
}


bool border_ui::is_in_cbr(s_int32 x, s_int32 y)
{
  if( !btempl_ || !container_) return false;
  
  return img_[border_template::C_BR]->point_belong (x, y);
}


bool border_ui::is_in_bt (s_int32 x, s_int32 y)
{
  if ( !btempl_ || !container_) return false;
    
  return img_[border_template::B_TOP]->point_belong (x, y);
}


void border_ui::resize ()
{
  if (!btempl_ || !container_) return;
  
  /* now we can resize the border */
  /* corner are not resized,  just each horizontal and vertical border */
  
  /* resize horizontal bar */
  img_[border_template::B_TOP]->resize (container_->get_length (), btempl_->get (border_template::B_TOP)->height ());  
  img_[border_template::B_BOTTOM]->resize (container_->get_length (), btempl_->get (border_template::B_BOTTOM)->height ());
  
  /* we are doing exactly the same for vertival border */
  /* resize horizontal bar */
  img_[border_template::B_LEFT]->resize (btempl_->get (border_template::B_LEFT)->length (), container_->get_height ());  
  img_[border_template::B_RIGHT]->resize (btempl_->get (border_template::B_RIGHT)->length (), container_->get_height ());

  move();
}


void border_ui::set_border (border_template * bt)
{
    btempl_ = bt;

    /* free memory */
    clear (); 

    /* now if bt is not null alloc all image */
    if (bt)  alloc (); 
    
    /* resize */
    resize();

    /* move */
    move();


    /* set corner */
    if (bt)
      {
	img_[border_template::C_TL]->resize (bt->get (border_template::C_TL)->length(), 
						 bt->get (border_template::C_TL)->height());
	img_[border_template::C_TR]->resize (bt->get (border_template::C_TR)->length(), 
						 bt->get (border_template::C_TR)->height());
	img_[border_template::C_BL]->resize (bt->get (border_template::C_BL)->length(), 
						 bt->get (border_template::C_BL)->height());
	img_[border_template::C_BR]->resize (bt->get (border_template::C_BR)->length(), 
						 bt->get (border_template::C_BR)->height());
      }
}



void border_ui::clear ()
{
    for (int i = 0; i < border_template::_LAST; i++)
    {
      delete img_[i];
      img_[i] = NULL;
    }
}


border_template * border_ui::get_template () const
{
  return btempl_; 
}



void border_ui::alloc ()
{
  for (int i = 0; i < border_template::_LAST; i++)
    img_[i] =  new gfx::drawing_area;  
}


border_ui::~border_ui()
{
  clear();
}




