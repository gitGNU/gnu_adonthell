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
 * @file   atk_label.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_misc base class.
 * 
 * 
 */

#ifndef ATK_LABEL_H_
#define ATK_LABEL_H_

#include "atk_font.h"
#include "atk_misc.h"

#include <string>  
#include <vector> 

class atk_label : public atk_misc
{
public : 

    /**Constructor initialise some parameters
     */
    atk_label (); 


    /**Set text
     * @param text
     */
    void set_text (const std::string & text); 

    
    /** get text
     * @return text of this label
     */
    string & get_text ();  


    /** set font
     * @param the font to used
     */
    void set_font (atk_font & font);


    /**get font used by this label
     * @return the font used
     */
    atk_font * get_font () const;

    
    /* clear the vector*/
    ~atk_label (); 


    /**Realize this object,  not draw but adapt this object with his contains
     */
    void realize (); 


    /**Draw the label
     * @param the target where draw the label, if null draw on the screen
     */
    void draw (surface * target = NULL); 
    
protected : 

    
    
private : 
    
    /** transform text in line, if the text contain some line
     */
    void transform_in_line (); 
      
    /* a line vector */
    std::vector <std::string> line_;   
    
    /* text of the label */
    std::string text_; 
    
    /* font used */
    atk_font * font_;  
}; 


#endif

