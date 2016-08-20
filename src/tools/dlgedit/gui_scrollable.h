/*
   Copyright (C) 2009 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project  <http://adonthell.nongnu.org>

   Dlgedit is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Dlgedit is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Dlgedit.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file common/gui_scrollable.h
 *
 * @author Kai Sterker
 * @brief Basic functionality for automated scolling.
 */

#ifndef COMMON_SCROLLING_H
#define COMMON_SCROLLING_H

#include <gdk/gdk.h>

/**
 * Implements scrolling that is triggered by the mouse pointer
 * near the border of the widget.
 */
class Scrollable
{
public:
    /**
     * Constructor.
     */
    Scrollable();
    
    /**
     * Destructor.
     */
    virtual ~Scrollable();
    
    /**
     * @name Auto-Scrolling (TM) ;) functionality.
     */
    //@{
    /**
     * Method deciding whether to start with scrolling. The cursor has to
     * remain in the outer 20 pixel of the view to enable scrolling.
     * @param point The current cursor position.
     */
    void prepareScrolling (const GdkPoint & point);
    /**
     * Moves the view in the desired direction.
     */
    virtual void scroll () = 0;
    /**
     * Check whether we are currently scrolling
     * @return <b>true</b> if that is the case, <b>false</b> otherwise.
     */
    bool isScrolling () const;
    /**
     * Finish scrolling.
     */
    void stopScrolling ();
    //@}

    /**
     * @name Program-Initiated Scrolling
     */
    void setScrollOffset (const int & x, const int & y)
    {
    	scroll_offset.x = x;
    	scroll_offset.y = y;
    }
    
    /**
     * @name Hooks into users of auto-scrolling.
     */
    //@{
    /**
     * Return a pointer to the drawing area
     * @return the GtkDrawingArea widget
     */
    virtual GtkWidget *drawingArea () const = 0;
    /**
     * Check whether it is allowed to initiate
     * scrolling.
     * @return false if scrolling is forbidden, true otherwise.
     */
    virtual bool scrollingAllowed () const = 0;
    //@}

protected:
    /// Offset by which the view moves during scrolling
    GdkPoint scroll_offset; 

private:
    /// create the cursor to display when scrolling is active
    void createScrollCursor ();
    /// Cursor to indicate scrolling
    GdkCursor *scrollCursor;

    /// Indicates whether autoscrolling is active
    bool scrolling;
};

#endif // COMMON_SCROLLING_H
