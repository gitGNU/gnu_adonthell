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

#include "effect.h"


// CONSTRUCTORS AND DESTRUCTORS
// =======================================================================

effect::effect() // default constructor
{
  debug_mode = false;
  setName( "" );
}

// "full" constructor
// "sparse" constructor

effect::effect( const string fn ) // from file constructor
{
}


// =======================================================================


// OVERLOADED OPERATORS
// =======================================================================


effect effect::operator=( const effect& ef )
{
  debug_mode = false;
  if( *this != ef )
  {
    setName( ef.getName() );
  }
  return *this;
}

bool effect::operator==( const effect& ef ) const
{
  if( getName() == ef.getName() )
    return true;
  else
    return false;
}

bool effect::operator!=( const effect& ef ) const
{
  if( !( *this == ef ) )
    return true;
  else
    return false;
}

// =======================================================================


// OVERLOADED CONSTRUCTORS
// =======================================================================

bool effect::debug( bool val = true ) // turns debug on or off
{
  debug_mode = val;
  return true;
}

string effect::getName() const // returns name
{
  if( debug_mode )
    cout << "getName(): name is '" << name << "'\n";
  return name;
}

bool effect::setName( const string val ) // sets name
{
  if( debug_mode )
    cout << "setName(): name set to '" << val << "'\n";
  name = val;
  return true;
}

// =======================================================================
