/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#if defined(USE_MAP)
#include "mapcharacter.h"
#else
#include "character_base.h"
#endif

class character
#if defined(USE_MAP)
 : public mapcharacter
#else
 : public character_base
#endif
{
 public:
  character();
  ~character();
};

#endif // __CHARACTER_H__
