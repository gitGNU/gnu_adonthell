/*
   $Id$
   
   (C) Copyright 2004 Kai Sterker
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <ctype.h>
#include "win_ttf.h"

// number of references to font file
u_int32 win_ttf::refcount = 0;
// font file used
TTF_Font *win_ttf::ttf = NULL;

// ctor
win_ttf::win_ttf (const char *color, const string & file) : win_font ()
{
    switch (color[0])
    {
        case 'r':		// red
            Color.r = 255; Color.g = 173; Color.b = 123;
            break;
        case 'b':		// blue
            Color.r = 139; Color.g = 185; Color.b = 238;
            break;
        case 'g':		// green
            Color.r = 205; Color.g = 254; Color.b = 148;
            break;
        case 'y':		// yellow
            Color.r = 255; Color.g = 238; Color.b = 123;
            break;
        case 'v':		// violet
            Color.r = 222; Color.g = 133; Color.b = 230;
            break;
        default:		// white and all the rest
            Color.r = 255; Color.g = 255; Color.b = 255;
            break;
    }
        
    refcount++;
    
    if (load (file)) 
    {
        u_int16 real_height_ = TTF_FontAscent (ttf);
        height_ = screen::dbl_mode () ? (real_height_+3) >> 1 : real_height_+1;
        cursor = &operator[]('_');
        length_ = cursor->length ();
    }
    // fallback to old win_font implementation
    else win_font::load ((char *) color);
}

win_ttf::~win_ttf ()
{
    refcount--;
    cursor = NULL;
    if (refcount == 0 && ttf != NULL)
        TTF_CloseFont (ttf);
}
    
bool win_ttf::load (const string & file)
{
    string path;

    // size of font
    u_int32 size = screen::dbl_mode () ? 22 : 12;
    
    // load font only once 
    if (ttf != NULL) return true;
    
    // absolute or relative font file from config?
    if (file != "" && file[0] == '/')
    {
        path = file;
    }
    else 
    {
        // path where is the file
        path = WIN_DIRECTORY; 
  
        // add win font directory path
        path += WIN_FONT_DIRECTORY; 

        // font name from config file
        path += file == "" ? "avatar.ttf" : file;
    }
    
    // try to load font specified in cfg file
    ttf = TTF_OpenFont (path.c_str (), size);
    
    if (ttf == NULL)
    {
        printf ("*** error loading font '%s':\n %s\n", path.c_str (), TTF_GetError ());
        return false;
    }
    
    // make sure our font doesn't exceed a pixel size of 24/13
    while (TTF_FontAscent (ttf) > (screen::dbl_mode () ? 24 : 13)) {
        TTF_CloseFont (ttf);
        TTF_OpenFont (path.c_str (), --size);
    }
    
    return true;
}

bool win_ttf::in_table(u_int16 tmp)
{
    if (win_font::in_table (tmp) == true) return true;
    
    // try to create font
    if (tmp > 0x80 || isprint (tmp)) {
        operator[](tmp);
        return true;
    }
    return false;
}

image & win_ttf::operator[](u_int16 glyph)
{
    static u_int16 unichar[2] = { 0, 0 };
    unichar[0] = glyph;

    static SDL_Color bg = { 0x00, 0x00, 0x00, 0 };
    static SDL_Color white = { 0xff, 0xff, 0xff, 0 };
    if (win_font::in_table (glyph)) return *(glyphs[glyph]);
    if (ttf == NULL) return *(glyphs[' ']);
    
    SDL_Surface *s = TTF_RenderUNICODE_Shaded (ttf, unichar, Color, bg);
    if (s == NULL) return *(glyphs[' ']);
    
    image tmp (s, bg);
    image shadow (TTF_RenderUNICODE_Solid (ttf, unichar, bg), white);
    image *glph = new image (tmp.length(), height_, false);
    glph->fillrect (0, 0, tmp.length()+1, height_+1, screen::trans_col(), NULL);
    shadow.draw (1, 1+height_-shadow.height(), 0, 0, shadow.length(), shadow.height(), NULL, glph);
    tmp.draw (0, height_-tmp.height(), 0, 0, tmp.length(), tmp.height(), NULL, glph);
    glyphs[glyph] = glph;

    return *glph;
}

