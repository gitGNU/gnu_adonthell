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


#ifndef TEXT_BUBBLE_H_
#define TEXT_BUBBLE_H_

#include "win_label.h"
#include "win_theme.h"

class text_bubble : public win_label
{
public:
    text_bubble (const string & text, const string & textcolor, const string & themename,
                 u_int16 len = 110); 
    ~text_bubble ();

    bool update (); 
    
private:
    u_int16 remain; 
}; 

#endif
