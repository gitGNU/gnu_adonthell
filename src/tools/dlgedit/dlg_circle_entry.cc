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
 * @file dlg_circle_entry.cc
 *
 * @author Kai Sterker
 * @brief The contents of a DlgCircle.
 */

#include <algorithm>
#include "dlg_circle_entry.h"

// Default Constructor
DlgCircleEntry::DlgCircleEntry ()
{
    text_ = "";
    annotation_ = "";
    npc_ = "Default";
    code_ = "";
    condition_ = "";
}

// set the dialogue text
void DlgCircleEntry::setText (string t)
{
    unsigned int pos = 0;
    text_ = t;

    // Make text safe:
    // replace linebreaks with space
    replace (text_.begin (), text_.end (), '\n', ' ');

    // escape quotes
    while ((pos = text_.find ("\"", pos)) != text_.npos)
    {
        if (pos > 0) {
            if (text_[pos-1] != '\\') text_.insert (pos, "\\");
        }
        else text_.insert (pos, "\\");

        pos++;
    }
}

// set the node's condition
void DlgCircleEntry::setCondition (string c)
{
    // remove all whitespace from the edges of the condition
    condition_ = c.substr (c.find_first_not_of (" \n\t"), c.find_last_not_of (" \n\t") + 1);

    // replace 'else if' by the shorter 'elif'
    if (strncmp ("else if", condition_.c_str (), 7) == 0)
        condition_.replace (0, 7, "elif");
}
