/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file dlg_module_entry.cc
 *
 * @author Kai Sterker
 * @brief The textual contents of a DlgModule.
 */

#include "dlg_module_entry.h"

DlgModuleEntry::DlgModuleEntry ()
{
    imports_ = "";
    ctor_ = "";
    dtor_ = "";
    methods_ = "";
}
