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
 * @file   atk_theme.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the atk_theme class.
 * 
 * 
 */


#ifndef ATK_THEME_H_
#define ATK_THEME_H_

#if __GNUG__ > 2
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include "str_hash.h"

#include <string> 
#include "atk_border_template.h"


class atk_theme
{
public :
    /**
     * Initialize this object
     */
    atk_theme ();

    
    /**
     *Load a theme
     * @param filename :  filename of the theme
     */
    void load (const std::string & filename); 


    /**
     *Save a theme
     * @param filename : filename used to save
     */
    void save (const std::string & filename); 

    
    /**
     * get name of the theme
     * @return name of the theme
     */
    std::string get_name () const;


    /**
     * set name of the theme
     * @param name :  name of the theme
     */
    void set_name (const std::string & name);  


    /**
     * add a border
     * @param border :  border to add
     */
    void add_border (atk_border_template * border);


    /**
     * remove a border
     * @param name :  name of the border to remove
     */
    void remove_border (const std::string & name);  


    /**
     * write in standart ouput theme information 
     */
    void display_info (); 
    
    
    /**
     * Free memory
     */
    ~atk_theme (); 
    
private :
    /**
     * free memory delete all border from the memory
     */
    void destroy (); 
    

    /* name of the theme */
    std::string name_;

    /* a hash_map for border */
    std::hash_map <std::string, atk_border_template* > border_;  
}; 




#endif