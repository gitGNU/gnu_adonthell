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
    name_ <<  fd; 
    
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
    name_ >> fd; 
    
    /* save number of border */
    u_int8 nb_border = border_.size ();
    nb_border >> fd;

    for (std::hash_map <std::string, border_template * >::iterator it = border_.begin (); it  != border_.end (); it++)
        it->second->save (fd); 
    
    fd.close (); 
}


std::string theme::get_name () const
{
    return name_; 
}


void theme::set_name (const std::string & name)
{
    name_ =  name; 
}


void theme::add_border (border_template * border)
{
    border_[border->get_name () ] =  border; 
}


void theme::remove_border (const std::string & name)
{
    std::hash_map <std::string, border_template * >::iterator it = border_.find (name);
    if ( it ==  border_.end ()) return;
    delete it->second;
    border_.erase (it); 
}


border_template * theme::get_border (const std::string & name)
{
  std::hash_map <std::string, border_template * >::iterator it = border_.find (name);
  if ( it ==  border_.end ()) return NULL;
  return it->second;
}

void theme::destroy ()
{
     // free border
    for (std::hash_map <std::string, border_template * >::iterator it = border_.begin (); it  != border_.end (); it++)
        delete it->second;
    border_.clear (); 
}


theme::~theme ()
{
    destroy (); 
}



void theme::display_info () 
{
    std::cout << "Theme name : " << name_ << std::endl;
    std::cout << "List of border : \n"; 

     
    for (std::hash_map <std::string, border_template * >::iterator it = border_.begin (); it  != border_.end (); it++)
        it->second->display_info (); 
}





