/*
   $Id$

   Copyright (C) 1999/2000/2001/2002  Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file   screen.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the screen class.
 * 
 * 
 */

#include <iostream>
#include <sstream> 

#include "screen.h"

using namespace gfx;

surface screen::display; 
u_int8 screen::bytes_per_pixel_ = 0;
u_int32 screen::trans = 0;
bool screen::fullscreen_ = false; 

void screen::init () 
{
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
        fprintf (stderr, "couldn't init display: %s\n", SDL_GetError ());
        exit (1);
    }
}

void screen::set_video_mode (u_int16 nl, u_int16 nh, u_int8 depth)
{
    u_int8 bpp;
    u_int32 SDL_flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    bool emulated = false;

    bpp = SDL_VideoModeOK (nl, nh, 16, SDL_flags);

    // Default video depth if none chosen.
    if (!depth) depth = bpp; 
    else if (depth != 16 && depth != 24) depth = 16;
    else if (bpp != depth) emulated = true;

    switch (bpp)
    {
        case 0:
            fprintf (stderr, "Video mode %dx%d unavailable. Exiting.. \n", nl,
                     nh);
            exit (1);
            break;
        default:
            if (emulated) std::cout << "Emulating " << (u_int16) depth << "bpp depth in "
                                    << (u_int16) bpp << "bpp mode: " << nl << "x" << nh << ".\n";
            else std::cout << "Using " << (u_int16) depth << "bpp depth: " << nl << "x" << nh << ".\n"; 
            bytes_per_pixel_ = bpp / 8; 
            break;
    }
    display.set_length (nl);
    display.set_height (nh);
    
    // surface destructor musn't free the screen surface
    display.not_screen = false; 

    display.vis = SDL_SetVideoMode (nl, nh, depth, SDL_flags);
    if (display.vis == NULL)
    {
        fprintf (stderr, "error: %s\n", SDL_GetError ());
        exit (1);
    }

    // Setting up transparency color
    trans = SDL_MapRGB (display.vis->format, 0xF8, 0x00, 0xF8);

    // Setting up the window title
    SDL_WM_SetCaption ("Adonthell", NULL);

    // Turn off SDL cursor
    SDL_ShowCursor (0);
}

void screen::show () 
{
    SDL_Flip (display.vis); 
}

std::string screen::info ()
{
    const SDL_VideoInfo * vi = SDL_GetVideoInfo ();
    std::ostringstream temp; 

    temp << "Video information: \n"
         << "Can create hardware surfaces:        " << (vi->hw_available ? "Yes" : "No") << std::endl
         << "Window manager available:            " << (vi->wm_available ? "Yes" : "No") << std::endl
         << "Hardware blits accelerated:          " << (vi->blit_hw ? "Yes" : "No") << std::endl
         << "Colorkey hardware blits accelerated: " << (vi->blit_hw_CC ? "Yes" : "No") << std::endl
         << "Alpha hardware blits accelerated:    " << (vi->blit_hw_A ? "Yes" : "No") << std::endl
         << "Software blits accelerated:          " << (vi->blit_sw ? "Yes" : "No") << std::endl
         << "Colorkey software blits accelerated: " << (vi->blit_sw_CC ? "Yes" : "No") << std::endl
         << "Alpha software blits accelerated:    " << (vi->blit_sw_A ? "Yes" : "No") << std::endl
         << "Color fill blits accelerated:        " << (vi->blit_fill ? "Yes" : "No") << std::endl
         << "Total video memory available:        " << vi->video_mem << " Kb" << std::endl 
         << std::ends;

    std::string ret = temp.str ();

    return ret; 
}

bool screen::set_fullscreen (bool m) 
{
    if (fullscreen_ != m)
    {
        int r = SDL_WM_ToggleFullScreen(display.vis);
        if (r) fullscreen_ = m;
        return r; 
    }
    return 0; 
}

void screen::transition (u_int16 i) 
{
    display.fillrect (0, 0, i, screen::height (), 0); 
    display.fillrect (screen::length () - i, 0, i, screen::height (), 0); 
    display.fillrect (0, 0, screen::length (), i, 0); 
    display.fillrect (0, screen::height () - i, screen::length (), i, 0); 
}