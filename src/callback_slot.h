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
 
#include "callback.h"

/**
 * Callback slot store callback and data parameters of the callback. With this,  callback is very powerfull. <BR>
 * Callback slot is created to be used with %callback_sig. <BR>  
 * Important callback slot doesn't support callback with a return value.
 * As now, there are only callback with 0 and 1 argument supported.
 *
 * Exemple :
 *   If you want create a callback slot with no parameters :
 *       callback_slot cb_slot (makeFunctor (classinstance, &classname::method));
 *
 *   With 1 parameters :
 *       callback_slot1 <type> (makeFunctor (classinstance, &classname::method), parameter);
 *
 *  For a better exemple see %callback_sig
 */

class callback_slot
{
public :
    callback_slot () {}
    
    callback_slot (const Functor0 & f) : F(f) {}
    
    virtual void operator () () { F (); }
    
private :
    Functor0 F; 
}; 


template <class T >
class callback_slot1 : public callback_slot
{
public : 
    
    callback_slot1 () {}
    
    callback_slot1 (const Functor1 <T > & f, T & p1) : F (f), P1 (p1) {}
    
    void operator () () { F (P1); }
    
private :
    Functor1 <T > F;  
    
    T & P1; 
}; 


#endif


