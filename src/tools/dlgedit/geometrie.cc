/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

#include <gtk/gtk.h>

#include "../../types.h"

/* is point inside rect */
u_int8 
point_in_rect (GdkRectangle rect, GdkPoint point)
{
    if (point.x < rect.x || point.x > rect.x + rect.width ||
        point.y < rect.y || point.y > rect.y + rect.height)
        return 0;

    return 1;
}

 /* return rectangles center */
GdkPoint 
center_point (GdkRectangle rect)
{
    GdkPoint point;

    point.x = rect.x + rect.width / 2;
    point.y = rect.y + rect.height / 2;

    return point;
}

/* add a offset to point */
GdkPoint 
offset_point (GdkPoint point, s_int32 x, s_int32 y)
{
    GdkPoint p;

    p.x = point.x + x;
    p.y = point.y + y;

    return p;
}

/* Create a rectangle from 2 points */
GdkRectangle 
create_rectangle (GdkPoint topleft, GdkPoint bottomright)
{
    GdkRectangle rect;

    rect.x = (topleft.x < bottomright.x) ? topleft.x : bottomright.x;
    rect.y = (topleft.y < bottomright.y) ? topleft.y : bottomright.y;

    rect.width = (rect.x == topleft.x) ? (bottomright.x - rect.x) : (topleft.x - rect.x);
    rect.height = (rect.y == topleft.y) ? (bottomright.y - rect.y) : (topleft.y - rect.y);

    return rect;
}

/* inflate a rectangle */
GdkRectangle 
inflate_rectangle (GdkRectangle rect, s_int32 x, s_int32 y)
{
    GdkRectangle r;

    r.x = rect.x - x;
    r.y = rect.y - y;
    r.width = rect.width + x + x;
    r.height = rect.height + y + y;

    return r;
}
