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

#include "character.h"

character::character()
{
#if defined(USE_PYTHON)
  locals=PyDict_New();
  PyDict_SetItemString(locals,"myself",pass_instance(this,"character"));
#endif
}

character::~character()
{
#if defined(USE_PYTHON)
  Py_DECREF(locals);
#endif
}
