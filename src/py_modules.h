/*
   $Id$

   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   py_modules.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Defines the Python initialisation function.
 * 
 * 
 */
 

#ifndef __PY_MODULES_H__
#define __PY_MODULES_H__

/** 
 * Initialise Python and inserts the Adonthell include paths.
 * 
 * 
 * @return true in case of success, false otherwise.
 */
bool init_python (void);

#endif // __PY_MODULES_H__
