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
 * @file dlg_cmdline.cc
 *
 * @author Kai Sterker
 * @brief Methods to parse the dlgedit commandline.
 */
 
#include <iostream> 
#include <dirent.h>
#include <unistd.h>
#include "dlg_cmdline.h"

// flag indicating whether to compile the given scripts
bool DlgCmdline::compile = false;

// the directory to look for project files
std::string DlgCmdline::datadir = DATA_DIR"/games";

// the default project
std::string DlgCmdline::project = "";

// index of the first dialgoue source in argv[]
int DlgCmdline::sources;

// examine the parameters passed to dlgedit
bool DlgCmdline::parse (int argc, char* argv[])
{
    int c;
    
    // Check for options
    while ((c = getopt (argc, argv, "cdhvp:j:")) != -1)
    {
        switch (c)
        {
            case 'd':
            {
                std::cout << datadir << std::endl;
                return false;
            }
                
            case 'v':
            {
                std::cout << _VERSION_ << std::endl;
                return false; 
            }
                
            case 'c':
            {
                compile = true;
                break;
            }
            
            case 'j':
            {
                project = optarg;
                break;
            }
            
            case 'p':
            {
                datadir = optarg;
                
                if (datadir[datadir.size () - 1] == '/')
                    datadir.erase (datadir.size () - 1);
                
                // Check whether the requested game directory exists
                DIR * mydir = opendir (datadir.c_str ()); 
                
                if (!mydir) 
                {
                    std::cerr << "No such directory " << datadir << "!" << std::endl;
                    return false;
                }
                closedir (mydir);
                
                break;
            }
            
            case '?':
            case 'h':
            {
                help (argv[0]);
                return false;
            }             
        }
    }
    
    sources = optind;
    return true;
}

// prints the help message
void DlgCmdline::help (const std::string &program)
{
    std::cout << "Usage: " << program << " [OPTIONS] [SOURCES]" << std::endl;
    std::cout << std::endl;
    std::cout << "Where [OPTIONS] can be:\n";
    std::cout << "-h         print this help message and exit" << std::endl; 
    std::cout << "-d         print the project directory and exit" << std::endl; 
    std::cout << "-v         print version and exit" << std::endl; 
    std::cout << "-c         compile all SOURCES and exit" << std::endl;
    std::cout << "-p dir     specify a custom project directory" << std::endl;
    std::cout << "-j project specify a default project" << std::endl;
}
