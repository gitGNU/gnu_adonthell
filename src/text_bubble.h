/*
   $Id$
   
   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>
   Copyright (C) 2001 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   text_bubble.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the text_bubble class.
 * 
 * 
 */


#ifndef TEXT_BUBBLE_H_
#define TEXT_BUBBLE_H_

#include "win_label.h"
#include "win_theme.h"


/**
 * Displays neat little text speech bubbles.
 *
 * This class makes text bubbles designed to stay visible
 * a short amount of time (just enough to be read).
 *
 */ 
class text_bubble : public win_label
{
public:
    /** 
     * Constructor.
     * 
     * @param text Text to display in that bubble.
     * @param textcolor Font to use with this bubble (directly taken from the
     *                  win_manager's font collection).
     * @param themename Theme to use with this bubble (directly taken from the
     *                  win_manager's theme collection).
     * @param len Basic duration (in %game cycles) of this bubble. Another amount
     *            of time will be added, depending on it's length.
     * 
     */
    text_bubble (const string & text, const string & textcolor, const string & themename,
                 u_int16 len = 110); 

    /**
     * Destructor.
     * 
     */ 
    ~text_bubble ();

    /**  
     * Update function.
     * 
     * @return \e false if the bubble should be destroyed now, \e true otherwise. 
     */
    bool update (); 

    /** 
     * Returns the number of game cycles remaining before the death of this
     * bubble. 
     * 
     * @return Number of game cycles remaining before the death of this bubble.
     */
    u_int16 remaining () 
    {
        return remain; 
    }
    
private:
    /**
     * Game cycles remaining before the death of this bubble.
     * 
     */ 
    u_int16 remain; 
}; 

#endif
