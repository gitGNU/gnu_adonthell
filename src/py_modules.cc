/*
   $Id$

   Copyright (C) 2000 The Adonthell Team
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "py_inc.h"


/**
 * @file   py_modules.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the Python initialisation function.
 * 
 * 
 */


/*
 * SWIG init prototypes. Should we use dynamic linking??? 
 */
extern "C"
{
    /** 
     * SWIG init prototype.
     * 
     */
    void initadonthellc (void);
}

/*
 * Start Python
 */
bool init_python (void)
{
    Py_Initialize();
    
    insert_path("scripts");
    insert_path("scripts/modules");
    
    /* Initialise SWIG modules. This should go if we ever switch to dynamic 
       link */
    initadonthellc();
    return true;
}

