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
#include "game.h"
#include "quest.h"
#include "character_base.h"
#include "fileops.h"
#include "dlg_module_entry.h"

DlgModuleEntry::DlgModuleEntry ()
{
    project_ = "";
    imports_ = "";
    ctor_ = "";
    dtor_ = "";
    methods_ = "";
}

// set the project
bool DlgModuleEntry::setProject (std::string p)
{
    bool retval = true;
    
    if (project_ != p)
    {
        project_ = p;
        
        // empty character and quest array
        characters.clear ();
        quests.clear ();
        
        retval &= loadCharacters ();
        retval &= loadQuests ();
    }
    
    return retval;
}

// load the character names
bool DlgModuleEntry::loadCharacters ()
{
    // look for a character file
    std::string file = game::find_file (project_ + "/character.data");
    if (file == "") return false;
    
    // open the file
    igzstream in;
    in.open (file);
    
    // version check
    if (!fileops::get_version (in, 3, 3, file))
        return false;
    
    // load characters
    character_base *mychar; 
    char ctemp;

    do
    {
        mychar = new character_base;
        mychar->get_state (in);
 
        // save the name of the NPC's
        if (mychar->get_id () != "Player") 
            addCharacter (mychar->get_name ());
        
        delete mychar;
    }
    while (ctemp << in);
    
    // close file
    in.close ();

    return true; 
}

// load the quest names
bool DlgModuleEntry::loadQuests ()
{
    // look for a character file
    std::string file = game::find_file (project_ + "/quest.data");
    if (file == "") return false;
    
    // open the file
    igzstream in;
    in.open (file);
    
    // version check
    if (!fileops::get_version (in, 1, 1, file))
        return false;

    ::quest myquest;
    char ctemp;

    // save the name of each Quest
    while (ctemp << in)
    {
        myquest.load (in);
        addQuest (myquest.name);
    }
    
    // close file
    in.close ();

    return true; 
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
