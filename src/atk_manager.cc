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



#include "atk_manager.h"


atk_manager::atk_manager () : cur_window_ (NULL) 
{
    il_.connect_function (input_event::KEYBOARD_EVENT,
                          MemberTranslator1wRet<input_event *,int,atk_manager,int (atk_manager::*)(input_event *)>
                          (*this, &atk_manager::keyboard_event));  

    
    il_.connect_function (input_event::MOUSE_EVENT,
                          MemberTranslator1wRet<input_event *,int,atk_manager,int (atk_manager::*)(input_event *)>
                          (*this, &atk_manager::mouse_event));  

    input_manager::add(&il_);
}


void atk_manager::add (atk_window *w)
{
    v_window_.push_back (w); 
}



void atk_manager::remove (atk_window *w)
{

}

void atk_manager::clear () 
{

}



void atk_manager::draw () 
{

}



int atk_manager::keyboard_event (input_event * ev)
{
    cout << "key event\n"; 
    return 0; 
}


int atk_manager::mouse_event (input_event * ev)
{
    cout << "Mouse event\n"; 
    return 1; 
}



