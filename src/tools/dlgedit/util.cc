/*
   Copyright (C) 2009/2010 Kai Sterker <kai.sterker@gmail.com>
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
 * @file common/util.cc
 *
 * @author Kai Sterker
 * @brief Helper methods.
 */

#include <sys/param.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include "game.h"

#ifdef WIN32
#define realpath(N,R) _fullpath((R),(N),_MAX_PATH)
#undef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

#include "util.h"

// get path relative to data directory
std::string util::get_relative_path (const std::string & path, const std::string & target_dir)
{
    std::string base_path = MK_UNIX_PATH (game::user_data_dir());
    std::string rel_path = path;
    
    // make sure to use path relative to (user defined) data directory
    if (base_path == "" || !remove_common_path (rel_path, base_path))
    {
        // fallback to builtin data dir if that doesn't seem to work
        base_path = MK_UNIX_PATH (game::game_data_dir());
        if (!remove_common_path (rel_path, base_path))
        {
            // if everything fails, try locating target_dir in the path and use
            // that as relative path
            size_t pos = rel_path.rfind (target_dir);
            if (pos != std::string::npos)
            {
                rel_path = rel_path.substr (pos);
                if (rel_path[0] == '/')
                {
                    rel_path = rel_path.substr (1);
                }
            }
        }
    }
    
    return rel_path;
}

// try to make path relative to base path
bool util::remove_common_path (std::string & path, const std::string & base_path)
{
    // make canonical base path
    char canonical_path[PATH_MAX];
    if (realpath(base_path.c_str(), canonical_path))
    {
        std::string c_base_path = canonical_path;
        if (realpath(path.c_str(), canonical_path))
        {
            path = MK_UNIX_PATH (canonical_path);
            if (path.compare (0, c_base_path.size(), c_base_path) == 0)
            {
                path = path.substr (c_base_path.length());
                if (path[0] == '/')
                {
                    path = path.substr (1);
                }
                return true;
            }
        }
    }
    
    return false;
}

// convert windows directory separators to unix style
std::string util::to_unix_path (const std::string & path)
{
	std::string result = path;
	for (std::string::iterator i = result.begin(); i != result.end(); i++)
	{
		if (*i == '\\') *i = '/';
	}
	return result;
}

// convert relative path to absolute path
std::string util::get_absolute_path (const std::string & path)
{
    std::string result = path;

    if (result[0] != '/')
    {
        char *oldwd = getcwd (NULL, 0);
        if (chdir (dirname ((char*) result.c_str())))
        {
            return path;
        }

        // get absolute pathname
        char *cpath = getcwd (NULL, 0);
        char *bname = strdup (path.c_str());
        result = std::string(cpath) + "/" + basename (bname);

        // restore working directory
        chdir (oldwd);

        // cleanup
        free (oldwd);
        free (cpath);
        free (bname);
    }

    return result;
}
