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
 * @file   callback_sig.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the callback sig management.
 * 
 * 
 */

/** Callback_sig is a powerfull class for callback management, you can connect lots of %callback_slot at this object.
 * Important when an instance of this object is destroyed, this instance delete all %callback_slot of the memory. So when you use callback_slot
 * and callback_sig you DON'T need to free callback_slot. <BR> 
 * It's very to use, see the exemple : 
 *
 * class A <BR> 
 * {<BR> 
 *  public : <BR> 
 *    void toto () { cout << "toto" <<  endl; }<BR> 
 *<BR> 
 *   void aff (char * c) { cout <<  c <<  endl;} <BR> 
 * }; <BR> 
 *<BR> 
 * int main () <BR> 
 * {<BR> 
 *   A a;<BR> 
 *   callback_sig cb_sig; <BR> 
 *<BR> 
 *  char *ch = "Kewl it's working"; <BR> 
 *  // it's very important to connect callback slot with "new" <BR> 
 *  cb_sig.connect (new  callback_slot (makeFunctor (a, &A::toto)));  <BR>   
 *  cb_sig.connect (new  callback_slot1 <char* > (makeFunctor (a, &A::aff),ch));<BR> 
 *  // execute all callback connected at this callback sig <BR> 
 *  cb_sig ();<BR> 
 * }<BR> 
 *
 */

#ifndef CALLBACK_SIG_H_
#define CALLBACK_SIG_H_

#include <vector>  
#include "callback_slot.h"
 
class callback_sig
{
public : 

    /** Call all %callback_slot connected at this object.
     */
    void operator () (); 
    
    
    /** Connect a %callback_slot at this object
     * @param s connect this callback_slot
     */
    void connect (callback_slot * s); 
    
    
    /** Delete in memory all %callback_slot connected at this object 
     */
    void clear (); 
    
    
    /** Destructor,  delete his stuff
     */
    ~callback_sig (); 
    
private :
    
    // list of callback slot
    std::vector <callback_slot *> lslot;  
}; 

#endif
