/*
   Copyright (C) 2001 Kai Sterker <kai.sterker@gmail.com>
   Copyright (C) 2001 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * @file   text_bubble.h
 * @author Kai Sterker <kai.sterker@gmail.com>
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
