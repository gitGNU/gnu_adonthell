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
 * @file   theme.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the theme class.
 * 
 * 
 */


#ifndef THEME_H_
#define THEME_H_

#if __GNUG__ > 2
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include "str_hash.h"

#include <string> 

#include "border_template.h"
#include "button_template.h"

namespace gui
{

  class theme
    {
      public :
	/**
	 * Initialize this object
	 */
	theme ();

    
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
      void add_border (border_template * border);


      /**
       * remove a border
       * @param name :  name of the border to remove
       */
      void remove_border (const std::string & name);  


      /**
       * get border 
       * @param name : name of the border
       * @return the border associated with the name
       */
      border_template * get_border( const std::string & name);

      
      /**
       * add a button
       * @param button :  button to add
       */
      void add_button (button_template * button);
      
      
      /**
       * remove a button
       * @param name :  name of the button to remove
       */
      void remove_button (const std::string & name);  


      /**
       * get button 
       * @param name : name of the button
       * @return the button associated with the name
       */
      button_template * get_button( const std::string & name);




      /**
       * write in standart ouput theme information 
       */
      void display_info (); 
    
    
      /**
       * Free memory
       */
      ~theme (); 
    
      private :
	/**
	 * free memory delete all object from the memory
	 */
	void destroy (); 
    

      /* name of the theme */
      std::string my_name;

      /* a hash_map for border */
      std::hash_map <std::string, border_template* > my_border;  
      
      /* a hash_map for button */
      std::hash_map <std::string, button_template* > my_button;  
    }; 
};



#endif
