/*
   $Id$

   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


/**
 * @file   atk_misc.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_misc base class.
 * 
 * 
 */

#ifndef ATK_MISC_H_
#define ATK_MISC_H_


#include "atk_widget.h"

/**
 * this is an abstract class 
 */

class atk_misc : public atk_widget
{
public :
    
    /** Initialize pad values to 0 and alignment in upper left
     */
    atk_misc (); 
    
    
    /** Set padding value
     * @param xp x pad
     * @param yp y pad
     */
    void set_padding (const u_int32 xp, const u_int32 yp); 
    

    /** get x padding
     *  @return x padding
     */
    u_int32 get_x_padding () const; 
    

    /** get x padding
     *  @return y padding
     */
    u_int32 get_y_padding () const;


    /** Set alignment
     * @param xa LEFT, CENTER, RIGHT
     * @param ya TOP, CENTER, BOTTOM
     */
    void set_alignment (const u_int8 xa, const u_int8 ya); 
    


    /** get x alignment
     *  @return x alignement (LEFT, CENTER, RIGHT) 
     */
    u_int8 get_x_alignment () const; 

    
    /** get y alignment
     *  @return y alignement (TOP, CENTER, BOTTOM)
     */
    u_int8 get_y_alignment () const; 


    static const u_int8 LEFT = 0;
    static const u_int8 CENTER = 1;
    static const u_int8 RIGHT = 2;
    static const u_int8 TOP = 0;
    static const u_int8 BOTTOM = 2; 
    
protected :

    
private : 

    /* padding value */
    u_int32 x_pad_;
    u_int32 y_pad_;

    /* align value */
    u_int8 x_align_;
    u_int8 y_align_; 
    
};


#endif
