/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef CHARACTER_BASE_H_
#define CHARACTER_BASE_H_

#define DIALOG_DIR "dialogues/"

#include <string>
#include "storage.h"
#include "fileops.h"

#include "py_inc.h"

enum
{
    DWARF = 0,
    ELF = 1,
    HALFELF = 2,
    HUMAN = 3
};

enum
{
    FEMALE = 0,
    MALE = 1
};

class character_base : public storage
{
 public:
  character_base();
  ~character_base();
  
  string get_name() { return name; }        // Get the character's name
  void set_name(const string& newname);      // Set the character's name
  u_int32 get_color() { return color; }     // Return the character's text color 
  void set_color (int c) { color = c; }     // Set the character's text color 
  string get_dialogue () { return dialogue; }// Gets the character's active dialogue
  void set_dialogue (const string& dialogue);// Set the active dialogue

#ifndef SWIG
  void save (ogzstream& out);                   // Save the character to file
  void load (igzstream& in);                    // Load the character from file
#endif

 protected:
  string name;
  string dialogue;
  u_int32 color;
  PyObject * locals;         // Locals that belong to that character
};

#endif // __CHARACTER_BASE_H__
