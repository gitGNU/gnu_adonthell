/*
   Copyright (C) 2002 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project  <http://adonthell.nongnu.org>

   Dlgedit is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Dlgedit is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Dlgedit.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file cfg_io.h
 *
 * @author Kai Sterker
 * @brief I/O Routines for dlgedit configuration file
 */

#ifndef CFG_IO_H
#define CFG_IO_H

#include "cfg_data.h"

/**
 * This class implements loading and saving of the dlgedit configuration
 * file. On Unix systems, the configuration is $HOME/.adonthell/dlgeditrc.
 *
 * The configuration contains a global section and project specific
 * sections. The global section stores infos like recently opened files,
 * while the project sections contains the base source directory of that
 * project.
 *
 * Only games that are located in DATA_DIR/games are valid projects.
 */
class CfgIO
{
public:
    /**
     * Constructor. Computes the path to the config file and loads it.
     */
    CfgIO ();

    /**
     * Destructor. Saves all changes to the config file.
     */
    ~CfgIO ();

private:
    /**
     * Load configuration from ~/.adonthell/dlgeditrc.
     */
    void load ();
    /**
     * Save configuration to ~/.adonthell/dlgeditrc.
     */
    void save ();

    CfgData     Data;           // Global configuration data
    std::string Dlgeditrc;      // Path to configuration file  
};

#endif // CFG_IO_H
