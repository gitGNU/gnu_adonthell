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

#ifndef __CHARACTER_BASE_H__
#define __CHARACTER_BASE_H__

#include "types.h"
#include "storage.h"
#include "py_inc.h"
#include <zlib.h>

struct PyCodeObject;

enum
{
    DWARF = 0,
    ELF = 1,
    HALFELF = 2,
    HUMAN = 3
};

enum
{
    FEMALE = 0,
    MALE = 1
};

class character_base : public storage
{
 public:
  character_base();
  ~character_base();
  
  char * get_name() { return name; }
  void set_name(const char * newname);
  u_int32 get_color() { return color; }

#ifndef SWIG
  void save (gzFile out);                   // Save the character to file
  void load (gzFile in);                    // Load the character from file
#endif

 protected:
  char * name;
  u_int32 color;
#if defined(USE_PYTHON)
  PyObject * locals;         // Locals that belong to that character
#endif
};

#endif // __CHARACTER_BASE_H__
