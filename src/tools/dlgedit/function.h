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

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <gtk/gtkstyle.h>
#include "main.h"

// Add custom functions to the Dialogue
class function_dlg : public preset_dlg
{
public:
    function_dlg (MainFrame*);

    int on_ok (char*);          // Accepted changes    
};

#endif // __FUNCTION_H__