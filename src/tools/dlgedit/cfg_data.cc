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
 * @file cfg_data.cc
 *
 * @author Kai Sterker
 * @brief dlgedit configuration data
 */

#include <stdio.h>
#include <algorithm>
#include "cfg_data.h"

/**
 * Global access to the dlgedit configuration
 */
CfgData* CfgData::data = NULL;

// ctor
CfgData::CfgData ()
{
    // make this configuration globally available
    data = this;
}

// dtor
CfgData::~CfgData ()
{
    data = NULL;
    
    // delete all projects
    for (std::vector<CfgProject*>::iterator i = Projects.begin (); i != Projects.end (); i++)
        delete *i;
}

// add entry to list of recently opened files
void CfgData::addFile (std::string &file)
{
    // check whether the file already exists
    std::deque<std::string>::iterator i = find (Files.begin (), Files.end (), file);

    // if that's the case, remove it
    if (i != Files.end ()) Files.erase (i);
    
    // otherwise make sure that we can add it 
    else 
    {
        // check whether the file exists at all
        FILE* test = fopen (file.c_str (), "r");
        if (!test) return;
        else fclose (test);
        
        // check whether there's enough room
        if (Files.size () == 15) Files.pop_front ();
    }
    
    // add file
    Files.push_back (file);
}

// get list of previously opened files
std::list<std::string> CfgData::getFiles ()
{
    // copy contents of Files into the list
    std::list<std::string> files (Files.begin (), Files.end ());

    // sort the list
    files.sort ();

    return files;
}

// add entry to list of projects
void CfgData::addProject (std::string &project)
{
    std::vector<CfgProject*>::iterator i;
    
    // check whether the project already exists
    for (i = Projects.begin (); i != Projects.end (); i++)
        if ((*i)->name () == project) break;
    
    // if exists -> overwrite
    if (i != Projects.end ()) (*i)->load ();
    
    // otherwise create a new one
    else
    {
        CfgProject *p = new CfgProject (project);
        
        // try to load it
        if (p->load ()) Projects.push_back (p);
        else delete p;
    }
}

// get basedir associated with a certain project
std::string CfgData::getBasedir (const std::string & project)
{
    std::vector<CfgProject*>::iterator i;

    // try to locate project
    for (i = Projects.begin (); i != Projects.end (); i++)
        if ((*i)->name () == project)
            return (*i)->basedir ();

    return "";
}

// set basedir of a certain project
void CfgData::setBasedir (const std::string & project, const std::string & basedir)
{
    std::vector<CfgProject*>::iterator i;

    // try to locate project
    for (i = Projects.begin (); i != Projects.end (); i++)
        if ((*i)->name () == project)
        {
            (*i)->setBasedir (basedir);
            return;
        }

    // project does not exist, so create it
    CfgProject *p = new CfgProject (project);
    p->setBasedir (basedir);
    Projects.push_back (p);
}

// save configuration data
void CfgData::save (std::ofstream &out)
{
    // save list of files
    for (std::deque<std::string>::iterator i = Files.begin (); i != Files.end (); i++)
        out << "File [" << *i << "]\n";

    // save list of projects
    for (std::vector<CfgProject*>::iterator i = Projects.begin (); i != Projects.end (); i++)
        (*i)->save (out);    
}
