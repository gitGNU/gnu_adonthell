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


#ifndef _WIN_OBJECT_H_
#define _WIN_OBJECT_H_

#include "win_base.h" 
#include "win_container.h"

template<class T>
class win_object : public win_base, public T
{
 public:
  
  win_object();
   
  
  ~win_object();
   
  bool draw();
  
  bool update();
    
  
  bool input_update();
   
  
  void set_brightness(bool b);
   
  void set_trans(bool b);
    
  
  void pack();
   
  
  void set_auto_refresh(bool b);
  

 protected:
  
  void refresh();

  image * img_tmp_;
  image * img_brightness_;

  bool auto_refresh_;
  
};


template<class T> 
win_object<T>::win_object()
{
  img_tmp_ = new image();

  img_tmp_->set_mask(true);
  
  img_brightness_ = new image();
      
  set_auto_refresh(false);
}


template<class T> 
win_object<T>::~win_object()
{
  if( img_tmp_ !=NULL ) delete img_tmp_ ;
  if( img_brightness_ != NULL) delete img_brightness_ ;
}


template<class T> bool
win_object<T>::draw()
{
  if(win_base::draw())
    {
      assign_drawing_area(wb_father_);
      
      win_background::draw(this);
      
      if(brightness_ || trans_)
	{
	  if( auto_refresh_ ) refresh();
	  if( brightness_ ) img_brightness_->draw(win_base::real_x(), win_base::real_y(), this);
	  else img_tmp_->draw(win_base::real_x(), win_base::real_y(), this);
	}
      else
	{
	  T::draw(win_base::real_x(), win_base::real_y(), this);
	}
      
      win_border::draw(wb_father_);
      
      detach_drawing_area();
      
      return true;
    }
  return false;
}


template<class T> bool
win_object<T>:: update()
{
  
  if(win_base::update())
	{
	  T::update();
	  return true;
	}
  return false;
}


template<class T> bool
win_object<T>::input_update()
{
  if(win_base::input_update())
    {
      
      if(input::has_been_pushed(win_keys::KEY_ACTIVATE_ENTRY)) on_activate_key();
      T::input_update();
      
      return true;
    }
  return false; 
}


template<class T> void
win_object<T>::set_brightness(bool b)
{ 
  win_base::set_brightness(b);
  refresh();
}



template<class T> void
win_object<T>::set_trans(bool b)
{
  win_base::set_trans(b);
  refresh();
}

template<class T> void
win_object<T>::pack()
{
  if(T::length() != win_base::length() || T::height() != win_base::height())
    {
      win_base::resize(T::length(), T::height());
      img_tmp_->resize(T::length(), T::height());
    }   
  refresh();
}


template<class T> void
win_object<T>::set_auto_refresh(bool b)
{
  auto_refresh_ = b;
}



template<class T> void
win_object<T>::refresh()
{
  //put the T drawable object in image  
  if(T::length() && T::height())
    {
        img_tmp_->lock (); 
        img_tmp_->fillrect(0,0,T::length(),T::height(),screen::trans_col());
        img_tmp_->unlock (); 
      
      T::draw(0,0,NULL,img_tmp_);
      
      if(brightness_) 
	{
	  img_brightness_->brightness(*img_tmp_,WIN_BRIGHTNESS_LEVEL);
	  img_brightness_->set_mask(true);
	}
      
      if(trans_) {img_tmp_->set_alpha(130);img_brightness_->set_alpha(130);}
      else {img_tmp_->set_alpha(255);img_brightness_->set_alpha(255);}
    }
}
 

#endif
