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
#if __GNUG__ > 2
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include "win_theme.h"
#include "win_font.h"
#include "win_base.h"
#include "str_hash.h"

using namespace std;   

class win_manager
{
 public:
    static void init ();
    
    static void cleanup ();
    
    static void add(win_base *);
  
    static void add_after (win_base * toadd, win_base * after);
    
    static bool exist(win_base *);
    
    static void remove(win_base *);
    
    static void update();
    
    static void input_update();
    
    static void draw();
    
    static void set_focus(win_base*);
    
    static void destroy();


    static void add_theme (string name); 
    static bool remove_theme (string name); 
    static win_theme * get_theme (string name); 

    static void add_font (string name); 
    static bool remove_font (string name); 
    static win_font * get_font (string name); 
 private:
  
    static list<win_base *> lmanage;
    //static list<win_container *> ::iterator ilm; 
    
    static hash_map<string, win_theme *> theme; 
    static hash_map<string, win_font *> font; 
    
    static win_base * wc;
};

#endif
 
