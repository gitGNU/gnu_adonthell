/*
   $Id$
   (C) Copyright 2001 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   (C) Copyright 2004 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com
   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with Adonthell; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 * @file   str_hash.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the hash<string> type, to be able to declare
 *         hash_maps with strings as keys.
 */ 

#ifndef STR_HASH_H
#define STR_HASH_H

#ifdef __clang__
#if MAC_OS_X_VERSION_MIN_REQUIRED > 1060
#include <unordered_map>
#include <unordered_set>

#define hash_map unordered_map
#define hash_set unordered_set
#else
#include <tr1/unordered_map>
#include <tr1/unordered_set>

#define hash_map tr1::unordered_map
#define hash_set tr1::unordered_set
#endif

#else

// gcc >= 4.4
#if __GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 4
#include <tr1/unordered_map>
#include <tr1/unordered_set>

#define hash_map tr1::unordered_map
#define hash_set tr1::unordered_set

#else
// gcc < 4.4
#if __GNUG__ > 2
#include <ext/hash_map>
#include <ext/hash_set>
#else
#include <hash_map>
#include <hash_set>
#endif
#include <string>

#if __GNUG__ > 2
namespace __gnu_cxx
#else
namespace std
#endif
{
    /**
     * Hash function for strings.
     */
    template<> struct hash<std::string>
    {
        size_t operator() (const std::string & __s) const
        {
            return __stl_hash_string (__s.c_str());
        }
    };
}

#if __GNUG__ > 2
namespace std { using namespace __gnu_cxx; }
#endif
#endif
#endif // CLANG

#endif // STR_HASH_H
