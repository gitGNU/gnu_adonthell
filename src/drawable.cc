// $Id$
/*
   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file drawable.cc
 * Defines the drawable class.
 */


#include "drawable.h"
#include "image.h"
#include "screen.h"

drawable::drawable () 
{
    set_length (0);
    set_height (0); 
}

drawable::~drawable ()
{ 
}

void drawable::update ()
{
}

void drawable::snapshot (image& r)
{
    //  Save the screen surface...
    SDL_Surface * ssave = screen::vis;

    //  Resize correctly the image surface...
    r.resize (length (), height ());
    
    // ...and put it in replacement of the screen's one.
    screen::vis = r.data;
    
    //  Performs the blitting operation which will actually occurs
    //  on the image
    draw (0, 0, NULL); 
    
    //  Restore the screen's surface.
    screen::vis = ssave; 
}
