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

class win_base;


using namespace std; 

class win_manager
{
 public:
    static void add(win_base *);
  
    static void add_after (win_base * toadd, win_base * after);
    
    static bool exist(win_base *);
    
    static void remove(win_base *);
    
    static void update();
    
    static void input_update();
    
    static void draw();
    
    static void set_focus(win_base*);
    
    static void destroy();


 private:
  
  static list<win_base *> lmanage;
  //static list<win_container *> ::iterator ilm;
  static win_base * wc;
};

#endif



