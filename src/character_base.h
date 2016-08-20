/*
   Copyright (C) 2000/2001 Kai Sterker <kai.sterker@gmail.com>
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * @file   character_base.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the character_base class.
 * 
 * 
 */



#ifndef CHARACTER_BASE_H_
#define CHARACTER_BASE_H_


/**
 * Where dialogs are located in the data tree.
 * 
 */
#define DIALOG_DIR "dialogues/"

#include "storage.h"
#include "fileops.h"

/**
 * Race enumeration.
 * 
 */ 
enum
{
    DWARF = 0,
    ELF = 1,
    HALFELF = 2,
    HUMAN = 3
};

/**
 * Gender enumeration.
 * 
 */
enum
{
    FEMALE = 0,
    MALE = 1
};

/**
 * Type enumeration.
 *
 */
enum
{
    NPC = 0,
    PLAYER = 1,
    PARTY = 2
};

/**
 * Base character class containing attributes and dialog stuff.
 * 
 */ 
class character_base : public storage
{
 public:
    /**
     * Default constructor.
     * 
     */ 
    character_base ();

    /**
     * Destructor.
     * 
     */ 
    ~character_base ();

    /** 
     * Returns the name of the %character.
     * 
     * @return the name of the %character.
     */
    string get_name () const { return name; }

    /**
     * Returns an unique identifier of the %character. 
     *
     * @return
     *      @li <b>Player</b> for the player controlled %character
     *      @li the %character's name otherwise.
     */
    string get_id ()
    {
        if (get_val ("type") == PLAYER) return "Player";
        else return name;
    }
    
    /** 
     * Sets the name of the %character.
     * 
     * @param newname name of the %character.
     */
    void set_name (string newname);

    /**
     * Returns the color representing the %character.
     *
     * @return the color representing the %character.
     */
    u_int32 get_color() const { return color; } 

    /** 
     * Sets the color representing the %character.
     * 
     * @param c new color representing the %character.
     */
    void set_color (int c) { color = c; } 
    
    /** 
     * Returns the current portrait of the %character.
     * 
     * @return the current portrait of the %character.
     */
    string get_portrait() const { return portrait; } 
    
    /** 
     * Sets the current portrait of the %character.
     * 
     * @param fname filename of the new portrait to use.
     */ 
    void set_portrait (string fname) { portrait = fname; } 
    
    /** 
     * Return the file name of the current %character's dialog.
     * 
     * @return file name of the dialog currently assigned to this %character.
     */
    string get_dialogue () const { return dialogue; }

    /** 
     * Sets the dialogue of the %character.
     * 
     * @param dialogue new %character's dialog.
     */
    void set_dialogue (string dialogue);

    /** 
     * Loads the state (attributes) of the %character from an opened file.
     * 
     * @param in file from which to read.
     */
    
    void get_state (igzstream& in);

    /** 
     * Saves the state (ttributes) of the %character into an opened file.
     * 
     * @param out file where to save.
     */
    void put_state (ogzstream& out);
         
private:
    string name;
    string dialogue;
    string portrait; 
    u_int32 color;
};

#endif
