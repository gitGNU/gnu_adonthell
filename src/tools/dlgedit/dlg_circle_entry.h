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
    std::string text ();
    /**
     * Get the comment assigned to this circle.
     * @return the circle's comments.
     */
    std::string annotation ()       { return annotation_; }
    /**
     * Get the character this circle is assigned to.
     * @return the circle's character.
     */
    std::string npc ()              { return npc_; }
    /**
     * Get the arbitrary code assigned to this circle.
     * @return the circle's code.
     */
    std::string code ()             { return code_; }
    /**
     * Get the condition this circle is based upon.
     * @return the circle's condition.
     */
    std::string condition ()        { return condition_; }
    /**
     * Get whether the circle may loop.
     * @return <b>true</b> if that's the case, <b>false</b> otherwise.
     */
    bool loop ()                    { return loop_; }
    
    void setText (std::string t);
    void setAnnotation (std::string a)   { annotation_ = a; }
    void setNpc (std::string n)          { npc_ = n; }
    void setCondition (std::string c);
    void setCode (std::string c)         { code_ = c; }
    void setLoop (bool l)                { loop_ = l; }
    //@}
        
    bool hasCode ()                 { return code_ != "" || condition_ != ""; }
                
private:
    bool loop_;                  // whether the text may loop
    std::string text_;           // the node's actual text
    std::string annotation_;     // comments of the author
    std::string npc_;            // name of the NPC this node is assigned to
    std::string code_;           // arbitrary (Python) code
    std::string condition_;      // condition(s) for this node
};

#endif // DLG_CIRCLE_ENTRY_H
