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
 * @file dlg_point.h
 *
 * @author Kai Sterker
 * @brief A wrapper around GdkPoint.
 */

#ifndef DLG_POINT_H
#define DLG_POINT_H

#include <gdk/gdktypes.h>

/**
 * The representation of a point, defined by its x and y coordinate. 
 * 
 * This class provides some abstraction from the GdkPoint type 
 * defined in GTK+, to remain as independent from the actual GUI 
 * toolkit as possible.
 */
class DlgPoint
{
public:
    DlgPoint () : x_(0), y_(0) { }
    DlgPoint (int x, int y) : x_(x), y_(y) { }
    DlgPoint (GdkPoint &point) : x_(point.x), y_(point.y) { }
    
    int x () const                  { return x_; }
    int y () const                  { return y_; }
    
    /**
     * Creates a new point, which is moved by the values of the given point.
     * @param p the distance to offset this point
     * @return this point, offset by the point p
     */
    DlgPoint offset (DlgPoint &p) const;
    /**
     * Creates a new point, which is moved by the given values.
     * @param x the offset in x direction
     * @param y the offset in y direction
     * @return this point, offset by the given values.
     */
    DlgPoint offset (int x, int y)  { return DlgPoint (x_ + x, y_ + y); }
    /**
     * Offset this point by the given values.
     * @param x the offset in x direction
     * @param y the offset in y direction
     */
    void move (DlgPoint &p)         { x_ += p.x (); y_ += p.y (); }
    /**
     * Offsets this point by the given values.
     * @param x the offset in x direction
     * @param y the offset in y direction
     */
    void move (int x, int y)        { x_ += x; y_ += y; }

    /**
     * Convert a DlgPoint into a GdkPoint.
     */
    operator GdkPoint();
    
    /**
     * Assign another point to this one
     */
    DlgPoint &operator= (const DlgPoint &p)  
    { 
        x_ = p.x (); 
        y_ = p.y (); 
        return *this;
    }
    
    /**
     * Less Than operator
     */
    bool operator< (const DlgPoint &p)
    {
        if (x_ == p.x ()) return y_ < p.y ();
        else return x_ < p.x ();
    }
    
private:
    int x_;
    int y_;
};
 
#endif // DLG_POINT_H
