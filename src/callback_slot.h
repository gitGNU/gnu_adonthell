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
 * @file   callback_slot.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the callback_slots class.
 * 
 * 
 */
#ifndef CALLBACK_SLOT_H_
#define CALLBACK_SLOT_H_
 
#if __GNUG__ > 2
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include <string>
#include "str_hash.h"
#include "callback.h"




/**
 * Callback manipulation class
 * This class is currently designed to work with callback with 0 arguments and 0 return
 * You can connect and disconnect several event at this object. Next you connect events
 * you can call update method with the name of event, If the name of event is connected
 * on slot so the callback is executed.  
 *
 * Just a little example
 * class A{
 * 
 * public:
 *  void  print(){
 *   cout << "Adonthell\n";
 *  }
 * };
 *
 *
 * void main()
 * {
 * callback_slot0 cb;
 * 
 * A a;
 * 
 * cb.connect ("essai", makeFunctor (a, &A::print));
 * 
 *
 * cb.update("essai");
 * cb.update("essai2"); //nothing, because event "essai2" is not define
 * 
 * }
 */

class callback_slot0 : private std::hash_map<std::string, Functor0 >
{
 public:
  

  /** Connect a callback for a specific event.
   * @param name_event The name of the specific event
   * @param func The callback function
   */
  void connect (const std::string & name_event, const Functor0 & func);

  /** Disconnect a callback for a specific event.
   * @param name_event The name of the specific event to remove
   */
  void disconnect (const std::string & name_event);

  
  
  /** Update, try to execute callback associated at the event_name.
   * @param name_event The name of the specific event to execute some callback.
   */
  void update (const std::string & name_event);
};

#endif
