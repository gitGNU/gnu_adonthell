/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <functional>
#include <map.h>
#include <String.h>
using namespace std;

#ifndef _keyword_class
#define _keyword_class


struct keyword_struct
{
  String name;
  int nb;
  int argc;

  public :
  keyword_struct();
  keyword_struct(String n, int a, int b);
};

class keyword_class : map<String, keyword_struct, less<String> > 
{
  public:

  keyword_class(const int&,const keyword_struct []);
  int nb_args(const String &) const;
  int operator() (const String & ) const;
};

#endif
