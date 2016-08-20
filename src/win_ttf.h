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

#ifndef WIN_TTF_H
#define WIN_TTF_H

#include <SDL_ttf.h>
#include "win_font.h"

class win_ttf : public win_font
{
public:
    win_ttf (const char *color, const string & file);
    ~win_ttf ();
    
    bool load (const string & file);
  	bool in_table (u_int16 tmp);
    image & operator[] (u_int16);
    s_int8 kerning(const u_int16 & char1, const u_int16 & char2);

private:
    SDL_Color Color;
    static std::string path_;
    static TTF_Font *ttf;
    static u_int32 refcount;
    hash_map<u_int16, s_int16> overflow;
};

#endif // WIN_TTF_H

