/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file dlg_point.cc
 *
 * @author Kai Sterker
 * @brief A wrapper around GdkPoint.
 */

#include "dlg_point.h"

// Cast DlgRect to GdkRectangle
DlgPoint::operator GdkPoint ()
{
    GdkPoint point;
    
    point.x = x ();
    point.y = y ();
    
    return point;
}

// move the point by a certain offset
DlgPoint DlgPoint::offset (DlgPoint &p) const
{
    return DlgPoint (x_ + p.x (), y_ + p.y ());
}
