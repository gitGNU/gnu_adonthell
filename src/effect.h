/*
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.

   effect.h is the definition for the effect class
   effect.cc is the implementation for this class
   effect.txt is the documentation for this class
*/

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <fstream>
#include <string>

class effect
{
 public:
  
  // CONSTRUCTORS AND DESTRUCTORS
  // =======================================================================
  
  effect(); // default constructor
  // "full" constructor
  // "sparse" constructor
  effect( const string fn ); // from file constructor

  // =======================================================================


  // OVERLOADED OPERATORS
  // =======================================================================

  effect operator=( const effect& ef ); // overloaded = operator
  bool operator==( const effect& ef ) const; // overloaded == operator
  bool operator!=( const effect& ef ) const; // overloaded != operator

  // =======================================================================


  // PUBLIC FUNCTIONS
  // =======================================================================

  bool debug( bool val = true ); // turns debug on or off
  string getName() const; // returns name
  bool setName( const string val ); // sets name

  // =======================================================================

 protected:
  
  // PUBLIC FUNCTIONS
  // =======================================================================

  bool debug_mode; // is debug mode on or off?
  string name; // name

  // =======================================================================

};

#endif
