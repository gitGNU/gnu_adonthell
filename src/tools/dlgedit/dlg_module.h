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
 * @file dlg_module.h
 *
 * @author Kai Sterker
 * @brief One complete dialogue or sub-dialogue.
 */

#ifndef DLG_MODULE_H
#define DLG_MODULE_H

#include "dlg_module_entry.h"
#include "kb_traverse.h"

/**
 * This class contains a dialogue, made up of elementary nodes and
 * (optionally) sub-dialogues. It is different from the other 
 * nodes as it contains a complete dialogue, but can also be part
 * of another dialogue.
 */
class DlgModule : public DlgNode
{
public:
    /**
     * Create a new, empty dialogue module.
     * @param p The full path to the file containing the module.
     * @param n The actual name without file extension.
     * @param s Unique id of the module.
     * @param d Description of the module.
     */
    DlgModule (std::string p, std::string n, std::string s, std::string d);

    /**
     * Reset the dialogue to its state after construction. Deletes all
     * nodes and clears the DlgModuleEntry.
     */
    void clear ();
    
    /**
     * @name Module drawing
     */
    //@{
    /**
     * Calculate this module's shape. The shape is a rectangle centered
     * around the given DlgPoint. It is large enough to display the
     * module's name.
     * @param center The point to center the module shape around.
     */
    void initShape (DlgPoint &center);
    
    /**
     * Draw this node to the given surface with the specified offset. 
     * The node's current mode determines the color thw node is drawn with. 
     * @param surface the GdkPixmap to draw to
     * @param offset the DlgPoint to use as offset
     */
    void draw (GdkPixmap *surface, DlgPoint &offset);
    //@}
    
    /**
     * @name Node handling
     */
    //@{
    /**
     * Add the given node to the dialogue.
     * @param node the DlgNode to add.
     */    
    void addNode (DlgNode *node);
    
    /**
     * Delete the currently selected node from the dialogue. If the node 
     * is a circle, also deletes all attached arrows.
     * @return \b true> if the node has been deleted, \b false otherwise.
     */
    bool deleteNode ();
    
    /**
     * Delete the given node from the dialogue.
     * @param node The node to delete.
     */
    void deleteNode (DlgNode *node);
    
    /**
     * Select a node from the list of nodes.
     * @param node The DlgNode to select.
     * @return \b true if the node has been selected, \b false otherwise.
     */
    bool selectNode (DlgNode *node);

    /**
     * Mark the given node as highlighted, to show that is under the cursor.
     * @param node the DlgNode to be highlighted
     * @return the the previously highlighted node, or <i>NULL</i> if no 
     *         node has been highlighted
     */
    DlgNode* highlightNode (DlgNode *node);
    
    /**
     * Select the root node of the dialogue.
     * @return \b true if the node has been selected, \b false otherwise.
     */
    bool selectRoot ();

    /** 
     * Deselect a previously selected node.
     * @return the previously selected DlgNode, or <i>NULL</i> if no 
     *         node has been deselected
     */
    DlgNode* deselectNode ();

    /**
     * Get the node at the given position.
     * @return the DlgNode at the positon, or NULL if there is none
     */
    DlgNode* getNode (DlgPoint &point);
    
    /**
     * Get the node that is currently selected.
     * @return the DlgNode currently selected
     */
    DlgNode* selected ()                { return selected_; }
    //@}
    
    /**
     * Get the extension of the module for centering in view.
     *
     * @param min_x will contain position of leftmost node
     * @param min_y will contain position of rightmost node
     * @param y will contain position of topmost node
     */
    void extension (int &min_x, int &max_x, int &y);

    /**
     * @name Loading / Saving
     */
    //@{
    /**
     * Init the Dialogue from a file
     * @return \b true if loading was successful, \b false otherwise.
     */
    bool load ();
    
    /**
     * Save the Dialogue to a file
     * @param path full path of the dialogue.
     * @param name file name without extension.
     * @return \b true if saving was successful, \b false otherwise.
     */
    bool save (std::string &path, std::string &name);
    
    /**
     * Save module as sub-dialogue. In contrast to the method above,
     * this only saves a 'reference' to the module. It will have to be
     * restored from the actual source file. 
     *
     * In order to allow different users to work on the same dialogues,
     * the reference will be relative to CfgProject::BaseDir. That way,
     * a dialogue referencing sub-dialogues can be loaded even if it is
     * moved to a different location, provided the relative paths of
     * included modules have not changed.
     *
     * @param file an opened file.
     */
    void save (std::ofstream &file)     { }
    //@}
   
    /**
     * @name Member access
     */
    //@{
    /**
     * Get the list of nodes in this dialogue.
     * @return a reference to the list of nodes.
     */
    std::vector<DlgNode*> &getNodes ()  { return nodes; }
    
    /**
     * Get the current offset of this dialogue.
     * @return a reference to the dialogue's offset.
     */
    DlgPoint &offset ()                 { return offset_; }
    
    /**
     * Get the name of this dialogue.
     * @return a reference to the dialogue's name.
     */
    std::string &name ()                { return name_; }

    /**
     * Get the name and id of this dialogue. To be used for window
     * captions and window menu.
     * @return string composed of name and unique id.
     */
    std::string shortName ()            { return name_ + serial_; }

    /**
     * Get the full path and filename of this dialogue. To be used when
     * saving the dialogue.
     * @return string composed of path, filename and file extension.
     */
    std::string fullName ()             { return path_ + name_ + FILE_EXT; }

    /**
     * Check whether this dialogue has been changed since it's been
     * opened or saved.
     * @return \b true if that is the case, \b false otherwise.
     */
    bool changed ()                     { return changed_; }
    /**
     * Mark this dialogue as changed.
     * @param c Set to \b true to mark the dialogue as changed.
     */
    void setChanged (bool c = true)     { changed_ = c; }
    
    /**
     * Retrieve the parent dialogue of this module. If the module is
     * a sub-dialogue, this will point to the dialogue that contains
     * the module. If it is the top-level dialogue, the parent will be
     * NULL.
     * @return parent of this dialogue, or \b NULL if it's a top-level dialogue 
     */
    DlgModule *parent ()                { return parent_; }
    /**
     * Set the parent of this dialogue.
     * @param parent The module containing this sub-dialogue
     */
    void setParent (DlgModule *parent)  { parent_ = parent; }
    
    /**
     * Get a pointer to the module entry
     * @return the DlgModuleEntry of this module
     */
    DlgModuleEntry *entry ()            { return &entry_; }
    
    /**
     * Get a pointer to the keyboard graph traversal helper. This keeps track
     * of the current selection's siblings. Users can navigate through them
     * via the arrow keys.
     * @return the KBTraverse of this module
     */
    KBTraverse *traverse ()             { return &traverse_; }
    //@}
    
protected:
    std::vector<DlgNode*> nodes;// all the nodes in this dialogue
    DlgNode *selected_;         // the node currently selected
    DlgNode *highlighted_;      // the node currently under the cursor
    DlgModule *parent_;         // parent of sub-dialogue
    
    DlgPoint offset_;           // The current offset in the graph view
    bool changed_;              // Whether there were changes since saving
    int nid_;                   // Id to use for the next new node

    std::string name_;          // Short (file-) name of the dialogue 
    std::string path_;          // Path of the dialogue
    std::string serial_;        // Unique number of the dialogue
    
    DlgModuleEntry entry_;      // further content of the dialogue

    KBTraverse traverse_;       // Keyboard graph traversal   helper
private:
    void init ();               // initialize a newly constructed DlgModule
};

#endif // DLG_MODULE_H
