/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


#ifndef _WIN_MANAGER_
#define _WIN_MANAGER_

#include <list>

class win_container;

class win_manager
{
 public:
  static void add(win_container *);
  
  static bool exist(win_container *);
  
  static void remove(win_container *);
  
  static void update();
  
  static void input_update();
  
  static void draw();
  
  static void set_focus(win_container*);
  
  static void destroy();


 private:
  
  static list<win_container *> lmanage;
  //static list<win_container *> ::iterator ilm;
  static win_container * wc;
};

#endif



