// $Id$
/*
  Copyright (C) 1999   The Adonthell Project
  Part of the Adonthell Project http://adonthell.linuxgames.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY.

  See the COPYING file for more details.
*/

#include "drawing_area.h"

drawing_area::drawing_area (s_int16 px, s_int16 py, s_int16 pw, s_int16 ph)
{
    move (px, py);
    resize (pw, ph);
    draw_to = NULL;
}

drawing_area::drawing_area ()
{
    move (0, 0);
    resize (0, 0);
    draw_to = NULL;
}

bool drawing_area::is_x_in (s_int16 px)
{
    return ((px - x () > 0) && (px - (x () + length ()) < 0));
}

bool drawing_area::is_y_in (s_int16 py)
{
    return ((py - y () > 0) && (py - (y () + height ()) < 0));
}

bool drawing_area::is_point_in (s_int16 px, s_int16 py)
{
    return (is_x_in (px) && is_y_in (py));
}

rect drawing_area::get_rects ()
{
    if (draw_to)
    {
        rect ret;
        rect temp = draw_to->get_rects ();

        ret.move (temp.x () > x ()? temp.x () : x (),
                  temp.y () > y ()? temp.y () : y ());
        ret.resize ((temp.x () + temp.length ()) <
                    (x () + length ())? temp.x () + temp.length () - ret.x () >
                    0 ? temp.x () + temp.length () - ret.x () : 0 : x () + length () -
                    ret.x () > 0 ? x () + length () - ret.x () : 0,
                    (temp.y () + temp.height ()) <
                    (y () + height ())? temp.y () + temp.height () - ret.y () >
                    0 ? temp.y () + temp.height () - ret.y () : 0 : y () + height () -
                    ret.y () > 0 ? y () + height () - ret.y () : 0);
        return ret;
    }
    else
        return *this;
}

drawing_area & drawing_area::operator = (const rect & r)
{
    rect & t = (rect &) r;
    move (t.x (), t.y ());
    resize (t.length (), t.height ());
    return *this;
}

drawing_area drawing_area::operator + (drawing_area & da)
{
    drawing_area ret, temp = (*this);
    rect t;

    temp.assign_drawing_area (&da);
    t = temp.get_rects ();
    ret = t;
    ret.draw_to = NULL;
    return ret;
}
