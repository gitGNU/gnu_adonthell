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
 * @file dlg_circle_entry.h
 *
 * @author Kai Sterker
 * @brief The contents of a DlgCircle.
 */

#ifndef DLG_CIRCLE_ENTRY_H
#define DLG_CIRCLE_ENTRY_H

#include <string>

/**
 * The %DlgCircleEntry keeps the actual contents of circle, it's
 * actual text, annotions of the author, the NPC it is assigned to 
 * (if any), it's conditions and additional Python code.
 */
class DlgCircleEntry
{
public:
    DlgCircleEntry ();
    
    /**
     * Attribute access.
     */
    //@{
    /**
     * Get the text assigned to this circle.
     * @return the circle's text.
     */
    string text ();
    /**
     * Get the comment assigned to this circle.
     * @return the circle's comments.
     */
    string annotation ()            { return annotation_; }
    /**
     * Get the character this circle is assigned to.
     * @return the circle's character.
     */
    string npc ()                   { return npc_; }
    /**
     * Get the arbitrary code assigned to this circle.
     * @return the circle's code.
     */
    string code ()                  { return code_; }
    /**
     * Get the condition this circle is based upon.
     * @return the circle's condition.
     */
    string condition ()             { return condition_; }
    
    void setText (string t);
    void setAnnotation (string a)   { annotation_ = a; }
    void setNpc (string n)          { npc_ = n; }
    void setCondition (string c);
    void setCode (string c)         { code_ = c; }
    //@}
        
    bool hasCode ()                 { return code_ != "" || condition_ != ""; }
                
private:
    string text_;           // the node's actual text
    string annotation_;     // comments of the author
    string npc_;            // name of the NPC this node is assigned to
    string code_;           // arbitrary (Python) code
    string condition_;      // condition(s) for this node
};

#endif // DLG_CIRCLE_ENTRY_H
