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


#include "atk_border_ui.h"


atk_border_ui::atk_border_ui (atk_container * ac) : container_ (ac), btempl_ (NULL) 
{    
  /*init tab */
  for (int i = 0; i < atk_border_template::_LAST; i++)
    img_[i] = NULL;   
}


void atk_border_ui::draw (drawing_area* da = NULL, surface * sf = NULL)
{
  if (!btempl_ || !container_) return;  
  
  img_[atk_border_template::B_TOP]->assign_drawing_area(da);
  btempl_->get_border (atk_border_template::B_TOP)->draw (img_[atk_border_template::B_TOP]->x(),
							  img_[atk_border_template::B_TOP]->y(),
							  img_[atk_border_template::B_TOP] , sf);
  img_[atk_border_template::B_TOP]->detach_drawing_area();
  
  img_[atk_border_template::B_BOTTOM]->assign_drawing_area(da);
  btempl_->get_border (atk_border_template::B_BOTTOM)->draw (img_[atk_border_template::B_BOTTOM]->x(),
							     img_[atk_border_template::B_BOTTOM]->y(),
							     img_[atk_border_template::B_BOTTOM], sf);
  img_[atk_border_template::B_BOTTOM]->detach_drawing_area();

  img_[atk_border_template::B_LEFT]->assign_drawing_area(da);
  btempl_->get_border (atk_border_template::B_LEFT)->draw (img_[atk_border_template::B_LEFT]->x(),
							   img_[atk_border_template::B_LEFT]->y(),
							   img_[atk_border_template::B_LEFT], sf);
  img_[atk_border_template::B_LEFT]->detach_drawing_area();
  
  img_[atk_border_template::B_RIGHT]->assign_drawing_area(da);
  btempl_->get_border (atk_border_template::B_RIGHT)->draw (img_[atk_border_template::B_RIGHT]->x(),
							    img_[atk_border_template::B_RIGHT]->y(),
							    img_[atk_border_template::B_RIGHT], sf);
  img_[atk_border_template::B_RIGHT]->detach_drawing_area();

  
  img_[atk_border_template::C_TL]->assign_drawing_area(da);
  btempl_->get (atk_border_template::C_TL)->draw (img_[atk_border_template::C_TL]->x(),
							 img_[atk_border_template::C_TL]->y(),
							 img_[atk_border_template::C_TL], sf);
  img_[atk_border_template::C_TL]->detach_drawing_area();

  img_[atk_border_template::C_TR]->assign_drawing_area(da);
  btempl_->get (atk_border_template::C_TR)->draw (img_[atk_border_template::C_TR]->x(),
							 img_[atk_border_template::C_TR]->y(),
							 img_[atk_border_template::C_TR], sf);
  img_[atk_border_template::C_TR]->detach_drawing_area();

  img_[atk_border_template::C_BL]->assign_drawing_area(da);
  btempl_->get (atk_border_template::C_BL)->draw (img_[atk_border_template::C_BL]->x(),
							 img_[atk_border_template::C_BL]->y(),
							 img_[atk_border_template::C_BL], sf);
  img_[atk_border_template::C_BL]->detach_drawing_area();

  img_[atk_border_template::C_BR]->assign_drawing_area(da);
  btempl_->get (atk_border_template::C_BR)->draw (img_[atk_border_template::C_BR]->x(),
							 img_[atk_border_template::C_BR]->y(),
							 img_[atk_border_template::C_BR], sf);
  img_[atk_border_template::C_BR]->detach_drawing_area();
}


void atk_border_ui::move ()
{
  if (!btempl_ || !container_) return;  
  
  img_[atk_border_template::B_TOP]->move (container_->get_x_real(), container_->get_y_real() - img_[atk_border_template::B_TOP]->height());
  img_[atk_border_template::B_BOTTOM]->move (container_->get_x_real(), container_->get_y_real() + container_->get_height());
  img_[atk_border_template::B_LEFT]->move (container_->get_x_real() - img_[atk_border_template::B_LEFT]->length(), container_->get_y_real());
  img_[atk_border_template::B_RIGHT]->move (container_->get_x_real() + container_->get_length(), container_->get_y_real());

  /* now move corner */
  img_[atk_border_template::C_TL]->move ( container_->get_x_real() - (img_[atk_border_template::C_TL]->length()>>1) - 
					  (img_[atk_border_template::B_LEFT]->length()>>1),
					  container_->get_y_real() - (img_[atk_border_template::C_TL]->height()>>1) -
					  (img_[atk_border_template::B_TOP]->height()>>1));
  img_[atk_border_template::C_TR]->move ( container_->get_x_real() - (img_[atk_border_template::C_TR]->length()>>1) + 
					  (img_[atk_border_template::B_RIGHT]->length()>>1) + container_->get_length(),
					  container_->get_y_real() - (img_[atk_border_template::C_TR]->height()>>1) -
					  (img_[atk_border_template::B_TOP]->height()>>1));
  img_[atk_border_template::C_BL]->move ( container_->get_x_real() - (img_[atk_border_template::C_BL]->length()>>1) - 
					  (img_[atk_border_template::B_LEFT]->length()>>1),
					  container_->get_y_real() - (img_[atk_border_template::C_BL]->height()>>1) +
					  (img_[atk_border_template::B_BOTTOM]->height()>>1) + container_->get_height());
  img_[atk_border_template::C_BR]->move ( container_->get_x_real() - (img_[atk_border_template::C_BR]->length()>>1) + 
					  (img_[atk_border_template::B_RIGHT]->length()>>1) + container_->get_length(),
					  container_->get_y_real() - (img_[atk_border_template::C_BR]->height()>>1) +
					  (img_[atk_border_template::B_BOTTOM]->height()>>1) + container_->get_height());
}


bool atk_border_ui::is_in_ctl(s_int32 x, s_int32 y)
{
  if( !btempl_ || !container_) return false;

  return img_[atk_border_template::C_TL]->point_belong (x, y);
}



bool atk_border_ui::is_in_ctr(s_int32 x, s_int32 y)
{
  if( !btempl_ || !container_) return false;
  
  return img_[atk_border_template::C_TR]->point_belong (x, y);
}


bool atk_border_ui::is_in_cbr(s_int32 x, s_int32 y)
{
  if( !btempl_ || !container_) return false;
  
  return img_[atk_border_template::C_BR]->point_belong (x, y);
}


bool atk_border_ui::is_in_bt (s_int32 x, s_int32 y)
{
  if ( !btempl_ || !container_) return false;
    
  return img_[atk_border_template::B_TOP]->point_belong (x, y);
}


void atk_border_ui::resize ()
{
  if (!btempl_ || !container_) return;
  
  /* now we can resize the border */
  /* corner are not resized,  just each horizontal and vertical border */
  
  /* resize horizontal bar */
  img_[atk_border_template::B_TOP]->resize (container_->get_length (), btempl_->get (atk_border_template::B_TOP)->height ());  
  img_[atk_border_template::B_BOTTOM]->resize (container_->get_length (), btempl_->get (atk_border_template::B_BOTTOM)->height ());
  
  /* we are doing exactly the same for vertival border */
  /* resize horizontal bar */
  img_[atk_border_template::B_LEFT]->resize (btempl_->get (atk_border_template::B_LEFT)->length (), container_->get_height ());  
  img_[atk_border_template::B_RIGHT]->resize (btempl_->get (atk_border_template::B_RIGHT)->length (), container_->get_height ());

  move();
}


void atk_border_ui::set_border (atk_border_template * bt)
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
	img_[atk_border_template::C_TL]->resize (bt->get (atk_border_template::C_TL)->length(), 
						 bt->get (atk_border_template::C_TL)->height());
	img_[atk_border_template::C_TR]->resize (bt->get (atk_border_template::C_TR)->length(), 
						 bt->get (atk_border_template::C_TR)->height());
	img_[atk_border_template::C_BL]->resize (bt->get (atk_border_template::C_BL)->length(), 
						 bt->get (atk_border_template::C_BL)->height());
	img_[atk_border_template::C_BR]->resize (bt->get (atk_border_template::C_BR)->length(), 
						 bt->get (atk_border_template::C_BR)->height());
      }
}



void atk_border_ui::clear ()
{
    for (int i = 0; i < atk_border_template::_LAST; i++)
    {
      delete img_[i];
      img_[i] = NULL;
    }
}


atk_border_template * atk_border_ui::get_template () const
{
  return btempl_; 
}



void atk_border_ui::alloc ()
{
  for (int i = 0; i < atk_border_template::_LAST; i++)
    img_[i] =  new drawing_area;  
}


atk_border_ui::~atk_border_ui()
{
  clear();
}




