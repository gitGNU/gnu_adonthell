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


#include "gfx/screen.h"
#include "atk_manager.h"


//  atk_window *  atk_manager::cur_window_ = NULL;
//  bool atk_manager::alive_ = true; 


atk_manager::atk_manager ()
{
    init (); 
}



void atk_manager::init ()   
{
    
    cur_window_ = NULL;
    alive_ = true; 
    
    
    // connect keyboard function callback
    il_.connect_function (input::event::KEYBOARD_EVENT,
                          MemberTranslator1wRet<input::event *,int,atk_manager,int (atk_manager::*)(input::event *)>
                          (*this, &atk_manager::input_update));  
    

    
    //connect mouse function callback 
    il_.connect_function (input::event::MOUSE_EVENT,
                          MemberTranslator1wRet<input::event *,int,atk_manager,int (atk_manager::*)(input::event *)>
                          (*this, &atk_manager::input_update));  
   
    
       
    input::manager::add(&il_);
}


void atk_manager::add (atk_window *w)
{ 
    v_window_.push_back (w); 
}



void atk_manager::remove (atk_window *w)
{
    std::deque <atk_window * >::iterator it = v_window_.begin ();
    
    while (it != v_window_.end () && * it != w)it++;
    
    if (it != v_window_.end ()) 
        v_window_.erase (it);     
}


void atk_manager::clear () 
{
    for (std::deque <atk_window* >::iterator it = v_window_.begin (); it != v_window_.end (); it++)
        delete *it;
    
    v_window_.clear (); 
}



void atk_manager::draw () 
{
    for (std::deque <atk_window* >::iterator it = v_window_.begin (); it != v_window_.end (); it++)
        (*it)->draw ();


    /* draw cursor */
    // WARNING IMPROVE IT !
    if (mouse_x_ <=  0 || mouse_y_ <=  0 || mouse_x_ >=  gfx::screen::length () - 5 || mouse_y_ >=  gfx::screen::height () - 10  ) return; 
    gfx::screen::display.draw_line (mouse_x_, mouse_y_, mouse_x_ + 5, mouse_y_, 0x000000); 
    gfx::screen::display.draw_line (mouse_x_, mouse_y_, mouse_x_, mouse_y_ + 5, 0x000000);
    gfx::screen::display.draw_line (mouse_x_, mouse_y_, mouse_x_ + 10, mouse_y_ + 10, 0x000000); 
    
}


void atk_manager::shutdown ()
{
    alive_ = false; 
}

/*
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
*/

int atk_manager::input_update (input::event * ev)
{

    // get mouse position for the cursor
    input::mouse_event * mev = (input::mouse_event*) ev; 
    mouse_x_ = mev->x ();
    mouse_y_ = mev->y (); 
    
    
    // if the window is a modal window,  only modal window receive input event.
    // if it is not a modal window all the event are dispatch for all window in the deque.
    // In this way if a window catch an event, the event is not longer dispatched 
    if (cur_window_ && cur_window_->is_modal ())
    {
        cur_window_->input_update (ev);  
    }
    else
    {
        // std::deque <atk_window* >::iterator it = v_window_.rend ();
        // while (it != v_window_.rbegin () && (*it)->input_update (ev) == 0)it--;
        int i = v_window_.size () - 1;
        while ( i > -1 && v_window_[i]->input_update (ev) == 0) i--;  
        
    } 
    return 1; 
}


void atk_manager::set_focus (atk_window * w)
{
    // WARNING :  call lost focus for the window
    cur_window_ = w; 
    remove (w);
    add (w); 
}


bool atk_manager::update ()
{
    if (cur_window_ && cur_window_->is_modal ())
    {
        if (cur_window_->update () == false) 
        {
            remove (cur_window_); 
            delete cur_window_;
            cur_window_ = NULL;
        }  
    }
    else
    {
        int i = v_window_.size () - 1;
        atk_window * tmp;
        
        while ( i > -1)
        {
            if (v_window_[i]->update () == false)
            {
                remove (tmp = v_window_[i]);
                delete tmp; 
            } 
            i--; 
        } 
    } 
    return alive_; 
}

atk_manager::~atk_manager ()
{
    clear (); 
}

