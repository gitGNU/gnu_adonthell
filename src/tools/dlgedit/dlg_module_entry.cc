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

#include <algorithm>
#include "dlg_module_entry.h"

DlgModuleEntry::DlgModuleEntry ()
{
    imports_ = "";
    ctor_ = "";
    dtor_ = "";
    methods_ = "";
}

// add a character
void DlgModuleEntry::addCharacter (std::string character)
{
    std::vector<std::string>::iterator i;
    
    for (i = characters.begin (); i != characters.end (); i++)
        if (character < *i) break;
    
    characters.insert (i, character);
}

// add a quest
void DlgModuleEntry::addQuest (std::string quest)
{
    std::vector<std::string>::iterator i;
    
    for (i = quests.begin (); i != quests.end (); i++)
        if (quest < *i) break;
    
    quests.insert (i, quest);
}

// check whether a certain character exists
bool DlgModuleEntry::isCharacter (const std::string &character)
{
    if (find (characters.begin (), characters.end (), character) == characters.end ())
        return false;
    
    return true;
}

// check whether a certain quest exists
bool DlgModuleEntry::isQuest (const std::string &quest)
{
    if (find (quests.begin (), quests.end (), quest) == quests.end ())
        return false;
    
    return true;
}

// iterate over the existing characters
std::string DlgModuleEntry::character ()
{
    static std::vector<std::string>::iterator i = characters.begin ();
    std::string character = "";
    
    if (i != characters.end ())
    {
        character = *i;
        i++;
    }
    else i = characters.begin ();
    
    return character;
}

// iterate over the existing quests
std::string DlgModuleEntry::quest ()
{
    static std::vector<std::string>::iterator i = quests.begin ();
    std::string quest = "";
    
    if (i != quests.end ())
    {
        quest = *i;
        i++;
    }
    else i = quests.begin ();
    
    return quest;
}
