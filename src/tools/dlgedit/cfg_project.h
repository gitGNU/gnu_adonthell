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
 * @file cfg_project.h
 *
 * @author Kai Sterker
 * @brief Project specific configuration data
 */

#ifndef CFG_PROJECT_H
#define CFG_PROJECT_H

#include <fstream>
#include <string>

/**
 * Data that belongs to an individual project. Usually, a project
 * contains all dialogues that belong to a certain game. The project's
 * name will be the name of the game, e.g. 'wastesedge'.
 */
class CfgProject
{
public:
    /**
     * Constructor. Creates an empty project with the given name.
     * @param name The name, or id, of the project.
     */
    CfgProject (std::string name);

    /**
     * Load project specific data from disk.
     * @return <b>true</b> if successful, <b>false</b> otherwise.
     */
    bool load ();
    
    /**
     * Save project specific data to disk.
     * @param file file to save data to.
     */
    void save (std::ofstream & file);
    
    /**
     * Get the project's name.
     * @return name (id) of the project
     */
    std::string name ()                 { return Name; }

    /**
     * Get the project's basedir
     * @return basedir of the project.
     */
    std::string basedir ()              { return BaseDir; }

    /**
     * Assign a base directory to the project. This is the
     * root of the dialogue sources belonging to this project.
     * When working with subdialogues, they will be adressed
     * relative to the base directory.
     * @param d the new base directory for this project.
     */
    void setBasedir (std::string d)     { BaseDir = d; }
    
private:
    std::string Name;               // Project Id
    std::string BaseDir;            // Path to dialogue source directory root.
};

#endif // CFG_PROJECT_H
