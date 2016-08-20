/*
   (C) Copyright 2004/2016 Kai Sterker
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

#include <config.h>
#include <ctype.h>
#include "win_ttf.h"

// number of references to font file
u_int32 win_ttf::refcount = 0;
// path of the font file
std::string win_ttf::path_;
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
        int real_height_ =  TTF_FontHeight(ttf) + screen::scale() - 1;
        height_ = real_height_ / screen::scale();
        cursor = &operator[]('_');
        length_ = cursor->length ();
    }
    // fallback to old win_font implementation
    else win_font::load ((char *) color);
}

win_ttf::~win_ttf ()
{
    refcount--;
    if (refcount == 0 && ttf != NULL)
        TTF_CloseFont (ttf);
}

static int glyph_height (TTF_Font *ttf, const u_int16 & chr)
{
    int miny, maxy;
    if (TTF_GlyphIsProvided(ttf, chr))
    {
    	TTF_GlyphMetrics(ttf, chr, NULL, NULL, &miny, &maxy, NULL);
    	return maxy - miny;
    }
    return TTF_FontHeight(ttf)/2;
}

bool win_ttf::load (const string & file)
{
    string path;
    int avg_size = 0;

    // size of font
    u_int32 size = screen::scale () * 12;
    
    // load font only once 
    if (ttf != NULL) return true;
    
    // absolute or relative font file from config?
    if (file.size() > 1 && (file[0] == '/' || file[1] == ':'))
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
        path += file == "" ? "Aclonica.ttf" : file;
    }
    
    // try to load font specified in cfg file
    ttf = TTF_OpenFont (path.c_str (), size);
    
    if (ttf == NULL)
    {
        printf ("*** error loading font '%s':\n %s\n", path.c_str (), TTF_GetError ());
        return false;
    }

    TTF_SetFontHinting(ttf, TTF_HINTING_LIGHT);

    // determine average pixel height of font, based on a number of glyphs
    avg_size += glyph_height(ttf, 'c');
    avg_size += glyph_height(ttf, 'C');
    avg_size += glyph_height(ttf, '0');
    avg_size += glyph_height(ttf, ':');
    
    // make sure our font doesn't exceed a certain height
    // since every font is different, this really only is an
    // approximation and results are not perfect for each and
    // every font out there
    while (avg_size > (22.5 * screen::scale())) {
    	avg_size = 0;
    	TTF_CloseFont (ttf);
        ttf = TTF_OpenFont (path.c_str (), --size);
        TTF_SetFontHinting(ttf, TTF_HINTING_LIGHT);

        avg_size += glyph_height(ttf, 'c');
        avg_size += glyph_height(ttf, 'C');
        avg_size += glyph_height(ttf, '0');
        avg_size += glyph_height(ttf, ':');
    }

    path_ = file;
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
    u_int16 unichar[2] = { 0, 0 };
    unichar[0] = glyph;

    static SDL_Color bg = { 0x00, 0x00, 0x00, 0 };
    static SDL_Color white = { 0xff, 0xff, 0xff, 0 };

    if (ttf && cursor && cursor->scale() != screen::scale())
    {
    	// if screen scale changed since creating the glyph,
    	// it needs to be recreated with the new size
    	TTF_CloseFont (ttf);
    	ttf = NULL;
    	erase();

        if (load (path_))
        {
            int real_height_ =  TTF_FontHeight(ttf) + screen::scale() - 1;
            height_ = real_height_ / screen::scale();
            cursor = &operator[]('_');
            length_ = cursor->length ();
        }
    }

    else if (win_font::in_table (glyph))
	{
    	return *(glyphs[glyph]);
	}

    if (ttf == NULL)
    {
    	return *(glyphs[' ']);
    }
    
    SDL_Surface *s = TTF_RenderUNICODE_Shaded (ttf, unichar, Color, bg);
    if (s == NULL)
	{
    	return *(glyphs[' ']);
	}
    
    u_int16 width = s->w;
    image tmp (s, bg);
    overflow[glyph] = width - tmp.length() * screen::scale();

    image *glph = new image (tmp.length(), height_, screen::scale());
    glph->fillrect (0, 0, tmp.length(), height_, screen::trans_col(), NULL);

    s = TTF_RenderUNICODE_Solid (ttf, unichar, bg);
    if (s != NULL)
    {
    	image shadow (s, white);
	    shadow.draw (1, 1+height_-shadow.height(), 0, 0, shadow.length(), shadow.height(), NULL, glph);
    }
    else
    {
    	fprintf (stderr, "%s\n", TTF_GetError ());
    }

    tmp.draw (0, height_-tmp.height(), 0, 0, tmp.length(), tmp.height(), NULL, glph);
    if (cursor != NULL)
    {
    	glyphs[glyph] = glph;
    }

    return *glph;
}

s_int8 win_ttf::kerning(const u_int16 & char1, const u_int16 & char2)
{
#ifdef HAVE_TTF_GETFONTKERNINGSIZEGLYPHS
	if (ttf)
	{
		return TTF_GetFontKerningSizeGlyphs(ttf, char1, char2) + overflow[char1];
	}
#endif
	return overflow[char1];
}
