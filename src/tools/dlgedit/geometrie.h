/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

u_int8 point_in_rect (GdkRectangle, GdkPoint);
u_int8 rect_in_rect (GdkRectangle a, GdkRectangle b);
GdkPoint offset_point (GdkPoint, s_int32, s_int32);
GdkPoint center_point (GdkRectangle);
GdkRectangle create_rectangle (GdkPoint, GdkPoint);
GdkRectangle inflate_rectangle (GdkRectangle, s_int32, s_int32);
