/*
   $Id$

   (C) Copyright 2001 Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

/**
 * @file   str_hash.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the hash<string> type, to be able to declare
 *         hash_maps with strings as keys.
 */ 

#ifndef STR_HASH_
#define STR_HASH_
 
#if __GNUG__ > 2
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include <string>

namespace std 
{
	
template<> struct hash<string> {
    size_t operator()(const string& s) const {
	return __stl_hash_string(s.c_str());
}};

}; // namespace std
#endif
