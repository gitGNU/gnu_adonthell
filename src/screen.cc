/*
   $Id$

   Copyright (C) 1999/2000/2001  Alexandre Courbot
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

#include "screen.h"
#include <iostream>
#ifndef __BEOS__
#include <sstream> 
#endif

using namespace std; 


surface screen::display; 
u_int8 screen::bytes_per_pixel_ = 0;
u_int32 screen::trans = 0;
bool screen::fullscreen_ = false; 
bool screen::dblmode;

void screen::set_video_mode (u_int16 nl, u_int16 nh, u_int8 depth, bool dbl, bool fscreen)
{
    u_int8 bpp;
    u_int32 SDL_flags = SDL_SWSURFACE;
    u_int8 emulated = depth; 
    
    if (fscreen) 
    {
        SDL_flags |= SDL_FULLSCREEN;
        fullscreen_ = true;
    }

    dblmode = dbl;

    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        fprintf (stderr, "couldn't init display: %s\n", SDL_GetError ());
        exit (1);
    }

    // Default video depth if none chosen.
    if (!depth) depth = 16; 
    
    if (dblmode)
        bpp = SDL_VideoModeOK (nl << 1, nh << 1, depth, SDL_flags);
    else
        bpp = SDL_VideoModeOK (nl, nh, depth, SDL_flags);

    if ((emulated) && (bpp) && (bpp != depth)) bpp = depth; 

    switch (bpp)
    {
        case 0:
            fprintf (stderr, "Video mode %dx%d unavailable. Exiting.. \n", nl,
                     nh);
            exit (1);
            break;
        default:
            bytes_per_pixel_ = bpp / 8; 
            break;
    }
    display.set_length (nl);
    display.set_height (nh);
    
    // surface destructor musn't free the screen surface
    display.not_screen = false; 

    if (dblmode) 
    {
        nl = nl << 1;
        nh = nh << 1;
    }

    display.vis = SDL_SetVideoMode (nl, nh, bpp, SDL_flags);
    if (display.vis == NULL)
    {
        fprintf (stderr, "error: %s\n", SDL_GetError ());
        exit (1);
    }
    
    // Setting up transparency color
    trans = SDL_MapRGB (display.vis->format, 0xFF, 0x00, 0xFF);

    // Setting up the window title
    SDL_WM_SetCaption ("Adonthell", NULL);

    // Turn off SDL cursor
    SDL_ShowCursor (0);
}

void screen::show () 
{
    SDL_Flip (display.vis); 
}

string screen::info ()
{
#ifndef __BEOS__
    const SDL_VideoInfo * vi = SDL_GetVideoInfo ();
    ostringstream temp; 

    const int driver_name_length = 500;
    char drv_name[driver_name_length];

    temp << "Video information: \n"
         << "Video driver used:                   " << SDL_VideoDriverName(drv_name, driver_name_length) << endl
         << "Internal game depth:                 " << bytes_per_pixel_ * 8 << endl
         << "Can create hardware surfaces:        " << (vi->hw_available ? "Yes" : "No") << endl
         << "Window manager available:            " << (vi->wm_available ? "Yes" : "No") << endl
         << "Hardware blits accelerated:          " << (vi->blit_hw ? "Yes" : "No") << endl
         << "Colorkey hardware blits accelerated: " << (vi->blit_hw_CC ? "Yes" : "No") << endl
         << "Alpha hardware blits accelerated:    " << (vi->blit_hw_A ? "Yes" : "No") << endl
         << "Software blits accelerated:          " << (vi->blit_sw ? "Yes" : "No") << endl
         << "Colorkey software blits accelerated: " << (vi->blit_sw_CC ? "Yes" : "No") << endl
         << "Alpha software blits accelerated:    " << (vi->blit_sw_A ? "Yes" : "No") << endl
         << "Color fill blits accelerated:        " << (vi->blit_fill ? "Yes" : "No") << endl
         << "Total video memory available:        " << vi->video_mem << " Kb" << endl 
         << "Using double size:                   " << (dblmode ? "Yes" : "No") << endl
         << "Fullscreen:                          " << (fullscreen_ ? "Yes" : "No") << endl
         << ends;

    string ret = temp.str ();
#else
    string ret = "Sorry, not available under BeOS\n";
#endif // __BEOS__

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
