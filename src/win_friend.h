/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_FRIEND_H_
#define WIN_FRIEND_H_

class win_font;
class drawing_area;

void draw_text(u_int16 sx,u_int16 sy,u_int16 ex,u_int16 ey,win_font * font,text_w & txt,drawing_area *,bool brightness );

#endif
