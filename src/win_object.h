/*
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
#include "label.h"
#include "label_input.h"
#include "image.h"



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


template class win_object<label>;
template class win_object<label_input>;
template class win_object<image>;


#endif











