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
 * @file cfg_data.h
 *
 * @author Kai Sterker
 * @brief dlgedit configuration data
 */

#ifndef CFG_DATA_H
#define CFG_DATA_H

#include <vector>
#include <deque>
#include "cfg_project.h"

/**
 * This class keeps track of the actual configuration settings.
 * It also checks those settings for correctnesss as far as that
 * is possible.
 *
 * Settings will be added or changed in two ways: at program
 * startup through reading ~/.adonthell/dlgeditrc and at run-time
 * through changes made by the user.
 */
class CfgData
{
public:
    /**
     * Destructor. Delete all Project entries.
     */
    ~CfgData ();
    
    /**
     * Adds an entry to the list of previously opened files. Only
     * existing files will be added. There's also a maximum of 15
     * files - when adding more than that, the oldest entries will
     * be discarded.
     * @param file full path/name of the file to add.
     */
    void addFile (std::string & file);
    /**
     * Adds a project to the list of projects and loads all project
     * specific data. The number of projects isn't limited.
     * @param project name of the project to add.
     */
    void addProject (std::string & project);
    /**
     * Save data to disk.
     * @param file file to save data to.
     */
    void save (ofstream & file);
    
private:
    std::deque <std::string> Files;         // list of previously opened files
    std::vector <CfgProject*> Projects;     // list of defined projects
};

#endif // CFG_DATA_H
