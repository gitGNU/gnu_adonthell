/*
  $Id$

  (C) Copyright 2002 Joel Vennin
  Part of the Adonthell Project http://adonthell.linuxgames.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License.
  This program is distributed in the hope that it will be useful,
  bbut WITHOUT ANY WARRANTY.

  See the COPYING file for more details
*/


/**
 * @file   box.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the container base class.
 * 
 * 
 */

#ifndef BOX_H_
#define BOX_H_

#include <deque>  
#include "input/event.h"
#include "widget.h"
#include "container.h"

namespace gui
{

  /**
   * This container can contains lots of widget, you can choice widget disposition, horizontal or vertical.
   */
  class box : public container
  {
  public :
	
    /**
     * Constructor,  initilase to  HORIZONTAL dispoition, homogene is false.
     */
    box ();
      
      
    /**
     * Add a widget in the list,  at the end of the list. It's same than add_end () 
     * on_add event is executed
     * @param a widget to add
     */
    void add (widget * w); 
      
    
    /** Remove a widget,  don't delete the object
     * on_remove event is executed
     * @param remove the widget
     */
    void remove (widget *  w); 
      

    /** clear all widget of the vector and child,  they are deleted
     * No event is called
     */
    void clear (); 
      
      
    /** 
     * It's used to build the widget.
     */
    void realize ();

    
    /** add widget at the beginning
     * @param widget to add,  on_add event is executed
     */
    void add_start (widget * w, bool expand = true); 
      
      
    /** add widget at the end
     * @param widget to add at the end ,  on_add event is executed
     */
    void add_end (widget * w, bool expand = true); 
    

    /** set space between each child
     * @param space in pixel
     */
    void set_spacing (const u_int16 s); 

    
    /** get  space between each object
     * @return space
     */
    u_int16 get_spacing () const; 
      
      
    /**draw the widget
     */
    virtual bool draw (gfx::drawing_area * da = NULL, gfx::surface * sf = NULL); 
      
      
    /**set_geometry
     * @param g gemotry is VERTICAL or HORIZONTAL (default) .
     */
    void set_geometry (const u_int8 g); 
    

    /**
     * update position
     */
    void update_position (); 

    void update_size ();
      
      
    /**
     * input update function
     * @return 1 if this object use the event,  else return 0
     */
    virtual int input_update (input::event *); 
      
      
    ~box (); 
      
    static const u_int8 VERTICAL = 0;
    static const u_int8 HORIZONTAL = 1; 
      
  protected : 

    
  private : 
	
	
    /** build the box in horizontal geometry
     */
    void realize_horizontal ();
      
    
    /** build the box in vertical geometry
     */
    void realize_vertical ();
    

    struct box_struct
    {
      /* the widget */
      widget * widget_;
        
      /* if true widget is streching to the container size, else container is stretch to widget */
      bool expand_;
    }; 
      
      
    /* HORIZONTAL OR VERTICAL */
    u_int8 geometry_; 
      

    /* space between each widget */
    u_int16 spacing_; 
      
      
    /* contains widget */
    std::deque <box_struct* > v_widget_;   
  }; 
};


#endif
