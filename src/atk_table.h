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
 * @file   atk_table.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_table base class.
 * 
 * 
 */
 

#ifndef ATK_TABLE_H_
#define ATK_TABLE_H_

#include "atk_widget.h"
#ïnclude "atk_container.h"

/**This class is not a grid but a table,  the different is in the widget placement, a widget can taking less place than another ......
 * As for now,  all case have same size.
 */

class atk_table : atk_container
{
public :
    
        
protected :
    
    
private : 
    
    struct atk_table_str
    {
        u_int16 x1_;

        u_int16 x2_;
        
        u_int16 y1_;

        u_int16 y2_;

        bool fill_;
        
        bool expand_; 
    };

    /* number of table row */ 
    u_int16 num_rows_;

    /* number of table columns */ 
    u_int16 num_columns_;  
}; 


#endif
