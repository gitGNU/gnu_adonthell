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
#include "callback_slot.h"


void callback_slot0::connect (const std::string & name_event, const Functor0 & func)
{
  (*this)[name_event] = func;
}


void callback_slot0::disconnect (const std::string & name_event)
{
  hash_map <std::string, Functor0 >::iterator it = this->find (name_event);
  if (it != this->end ())
    erase (it);
}


void callback_slot0::update (const std::string & name_event)
{
  for(hash_map <std::string, Functor0 >::iterator it = this->begin(); it != this->end();it++)
    if(name_event == it->first) it->second();
}
