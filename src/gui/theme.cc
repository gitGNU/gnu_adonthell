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

#include <iostream>
#include "theme.h"

using namespace gui;


theme::theme ()
{
    
}


void theme::load (const std::string & filename)
{
  /* init hash_map border */
  destroy (); 
  
  /* open file with gz stream */
  igzstream fd;
  
  if (!fd.open (filename))
    {
      std::cerr << "error theme::load " <<  filename << std::endl;
        return; 
    }
  
  /* load name */
  my_name <<  fd; 
  
  /* start to read number of border */
  u_int8 nb_border;
  nb_border << fd; 
  
  // std::cout << nb_border << std::endl; 
  
  border_template * tmp; 
  for (u_int8 i = 0; i < nb_border; i++)
    {
      tmp =  new border_template ();
      tmp->load (fd);
      add_border (tmp); 
    }
  
  /* close the fd */
  fd.close (); 
}



void theme::save (const std::string & filename)
{
  /* open a gz output stream */
  ogzstream fd;
  if (!fd.open (filename))
    {
      std::cerr << "error theme::save " <<  filename << std::endl;
      return;  
    }
  
  /* save name */
  my_name >> fd; 
  
  /* save number of border */
  u_int8 nb_border = my_border.size ();
  nb_border >> fd;
  
  /* save the border */
  for (std::hash_map <std::string, border_template * >::iterator it = my_border.begin (); it  != my_border.end (); it++)
    it->second->save (fd); 
  

  /* save number of button */
  u_int8 nb_button = my_button.size ();
  nb_button >> fd; 
  
  /* save the button */
  for (std::hash_map <std::string, button_template * >::iterator ot = my_button.begin (); ot  != my_button.end (); ot++)
    ot->second->save (fd); 

  fd.close (); 
}


std::string theme::get_name () const
{
    return my_name; 
}


void theme::set_name (const std::string & name)
{
    my_name =  name; 
}


void theme::add_border (border_template * border)
{
    my_border[border->get_name () ] =  border; 
}


void theme::remove_border (const std::string & name)
{
    std::hash_map <std::string, border_template * >::iterator it = my_border.find (name);
    if ( it ==  my_border.end ()) return;
    delete it->second;
    my_border.erase (it); 
}


border_template * theme::get_border (const std::string & name)
{
  std::hash_map <std::string, border_template * >::iterator it = my_border.find (name);
  if ( it ==  my_border.end ()) return NULL;
  return it->second;
}


void theme::add_button (button_template * button)
{
    my_button[button->get_name () ] =  button; 
}


void theme::remove_button (const std::string & name)
{
    std::hash_map <std::string, button_template * >::iterator it = my_button.find (name);
    if ( it ==  my_button.end ()) return;
    delete it->second;
    my_button.erase (it); 
}


button_template * theme::get_button (const std::string & name)
{
  std::hash_map <std::string, button_template * >::iterator it = my_button.find (name);
  if ( it ==  my_button.end ()) return NULL;
  return it->second;
}



void theme::destroy ()
{
     // free border
    for (std::hash_map <std::string, border_template * >::iterator it = my_border.begin (); it  != my_border.end (); it++)
        delete it->second;
    my_border.clear (); 

    // free button 
    for (std::hash_map <std::string, button_template * >::iterator it = my_button.begin (); it  != my_button.end (); it++)
        delete it->second;
    my_button.clear (); 

}


theme::~theme ()
{
    destroy (); 
}



void theme::display_info () 
{
    std::cout << "Theme name : " << my_name << std::endl;
    std::cout << "List of border : \n"; 

     
    for (std::hash_map <std::string, border_template * >::iterator it = my_border.begin (); it  != my_border.end (); it++)
        it->second->display_info (); 

    std::cout << "List of button : \n";
    
    for (std::hash_map <std::string, button_template * >::iterator it = my_button.begin (); it  != my_button.end (); it++)
      it->second->display_info (); 
}





