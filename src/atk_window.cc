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

#include "mouse_event.h"

#include "atk_border.h"
#include "atk_window.h"


atk_window::atk_window () :  alive_ (true) 
{
    // modal by default
    set_modal (true);

    //sensible by default
    set_sensible (true);

    set_resizeable (true);

    set_moveable (true);

    set_deleted (true); 

    /* mouse action none at the beginning */
    mouse_action_ = ACTION_NONE; 
}


void atk_window::set_title (const std::string & t)
{
    title_ = t; 
}


void atk_window::set_type (const u_int8 t)
{
    type_ = t; 
}


bool atk_window::is_modal () const
{
    return modal_; 
}


void atk_window::set_modal (const bool m)
{
    modal_ = m; 
}



int atk_window::input_update (input_event * ev)
{
    if (!is_sensible () ) return 0; 
 
    mouse_event * me = (mouse_event *) ev; 
 
    // check mouse action
    if (mouse_action_ == ACTION_MOVE)
    {
        if (me->type () == mouse_event::MOUSE_MOTION)
        {
            set_position (me->x ()  - mouse_range_x_, me->y ()  - mouse_range_y_);
            return 1; 
        }
        else if (me->type () == mouse_event::BUTTON_RELEASED)
        {
            cout << "Button Release Move\n"; 
            mouse_action_ = ACTION_NONE; 
            return 1; 
        } 
    }
    
    
    if (mouse_action_ == ACTION_RESIZE)
    {
        if (me->type () == mouse_event::MOUSE_MOTION)
        {
            set_size (me->x ()  - get_x_real ()  , me->y ()  - get_y_real () );
            return 1; 
        }
        else if (me->type () == mouse_event::BUTTON_RELEASED)
        {
            cout << "Button Release Resize\n"; 
            mouse_action_ = ACTION_NONE; 
            return 1; 
        } 
    }
    
    

    
    if (point_belong (me->x (), me->y ())) 
    {         
        
        return 0; 
    }
    else
    {
        // maybe the mouse cursor is on border
        if (deleted_)
        { 
            // WARNING ADD CHECK ON BORDER 
            if (me->type () == mouse_event::BUTTON_RELEASED &&
                me->x ()  >= get_x_real () + get_length () - 10 && me->x ()  <= get_x_real () + get_length () &&
                me->y () >= get_y_real () - 10  && me->y ()  <= get_y_real ()
                )
            {
                on_delete (); 
                return 1; 
            } 
        }
        
        if (moveable_)
        {
            if (mouse_action_ == ACTION_NONE && me->type () == mouse_event::BUTTON_PUSHED &&
                me->x () >= get_x_real () && me->x () < get_x_real () + get_length () - 10 &&
                me->y () >= get_y_real () - 10  && me->y ()  <= get_y_real ()
                ) 
            {
                cout << "Button Pushed Move\n"; 
                mouse_action_ = ACTION_MOVE;
                mouse_range_x_ = me->x () - get_x_real ();
                mouse_range_y_ = me->y () - get_y_real (); 
                return 1; 
            } 
        }


        if (resizeable_)
        {
            if (mouse_action_ == ACTION_NONE && me->type () == mouse_event::BUTTON_PUSHED &&
                me->x () >= get_x_real () + get_length () - 10 && me->x () < get_x_real () + get_length () &&
                me->y () >= get_y_real () + get_height ()  && me->y ()  <= get_y_real () + get_height () + 5
                ) 
            {
                cout << "Button Pushed Resize\n"; 
                mouse_action_ = ACTION_RESIZE;
                return 1; 
            }  
        }
        
    }  
    return 0; 
}


bool atk_window::update ()
{
    return alive_; 
}


void atk_window::shutdown ()
{
    alive_ = false; 
}


void atk_window::set_resizeable (const bool b)
{
    resizeable_ = b; 
}

bool atk_window::is_resizeable () const
{
    return resizeable_; 
}


void atk_window::set_deleted (const bool b)
{
    deleted_ = b; 
}

bool atk_window::is_deleted () const
{
    return deleted_; 
}


void atk_window::set_moveable (const bool b)
{
    moveable_ = b; 
}

bool atk_window::is_moveable () const
{
    return moveable_; 
}


void atk_window::draw (drawing_area * da = NULL, surface * sf = NULL)
{
    /* first draw the background */
    
    
    /* draw the border */
    //WARNING must be move in atk_border or atk_border_template 
    if (border_->get_template () == NULL)
    {
        /* top */
        screen::display.draw_line (get_x_real (), get_y_real () - 10 ,get_x_real () + get_length (),get_y_real () - 10,  0x000000);
        screen::display.draw_line (get_x_real (), get_y_real (),get_x_real () + get_length (),get_y_real (),  0x000000); 

        /* bottom */
        screen::display.draw_line (get_x_real (), get_y_real () + get_height () ,get_x_real () + get_length (),get_y_real () + get_height (),  0x000000);
        screen::display.draw_line (get_x_real (), get_y_real () + get_height () + 5,get_x_real () + get_length (),get_y_real () + get_height () + 5,  0x000000);

        
        /* left */
        screen::display.draw_line (get_x_real (), get_y_real () - 10 ,get_x_real (),get_y_real ()+ get_height () + 5,  0x000000); 
        
        /* right */
        screen::display.draw_line (get_x_real () + get_length (), get_y_real () - 10 ,get_x_real () + get_length (),get_y_real () + get_height () + 5,  0x000000);

        
        if (deleted_) 
            screen::display.draw_line (get_x_real () + get_length () - 10, get_y_real () - 10 ,get_x_real () + get_length () - 10,get_y_real (),  0x000000);
        
        if (resizeable_)
        {
            /* last line */ 
           
            
            /* left */
            screen::display.draw_line (get_x_real () + 10, get_y_real () + get_height (),get_x_real () + 10,get_y_real () + get_height () + 5,  0x000000);
            /* right */
            screen::display.draw_line (get_x_real () + get_length ()  - 10, get_y_real () + get_height (),get_x_real () + get_length () - 10,get_y_real () + get_height () + 5,  0x000000);
        }
    }
    else border_->draw (da, sf); 
    
    
    /* draw the contain */ 
    if (child) child->draw (da, sf); 
    
}




void atk_window::realize ()
{
    if (!child) return;

    if (child->get_length ()  != get_length () || child->get_height ()  != get_height ())
    {
        // WARNING maybe change and put this system in atk_bin ? ? 
        if (resizeable_)
        {
            set_size (child->get_length () + (border_width_ << 1) , child->get_height () + (border_width_ << 1)); 
        }
        else
        {
            child->set_size (get_length () -(border_width_ << 1), get_height () - (border_width_ << 1));
            child->realize (); 
        }
    } 
}


void atk_window::set_size (u_int32 length, u_int32 height)
{
    atk_container::set_size (length, height);
    
    if (child)
    {
        child->set_size (get_length () -(border_width_ << 1), get_height () - (border_width_ << 1));
        child->realize ();
    }
}

