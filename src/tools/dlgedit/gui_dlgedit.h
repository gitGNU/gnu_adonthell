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
 * @file gui_dlgedit.h
 *
 * @author Kai Sterker
 * @brief The Dialogue Editor's main window
 */

#ifndef GUI_DLGEDIT_H
#define GUI_DLGEDIT_H

#include <vector>
#include <string>
#include <gtk/gtkstyle.h>
#include "gui_messages.h"
#include "gui_graph.h"
#include "gui_list.h"

/**
 * The application's main window. Contains the menu- and statusbar, as
 * well as pointers to the other widgets. The main window is created at
 * program startup, and closed on termination of the program. There can
 * be only one main window at a time, but several dialogues may be open
 * simultanously.
 */
class GuiDlgedit
{
public:
    /** 
     * Constructor. Creates the main window with all the sub-widgets,
     * such as the dialogue view or the instant preview.
     */
    GuiDlgedit ();
    /**
     * Destructor.
     */
    ~GuiDlgedit ();
    
    /**
     * Global pointer to the main window, to allow easy access from anywhere.
     */
    static GuiDlgedit *window;

    /**
     * Widget access methods
     */
    //@{
    /**
     * Retrieve the dialogue view.
     * @return the widget containing the graphical representation of the
     *         dialogue graph.
     */
    GuiGraph *graph ()      { return graph_; }
    /**
     * Retriecve the instant preview widget.
     * @return the widget containing the text of the selected DlgNode and
     *         that of it's parents and children.  
     */
    GuiList *list ()        { return list_; }
    
    /**
     * Retrieve the font used to draw text onto a drawing area.
     * @return a GdkFont.
     */
    GdkFont *font ()        { return font_; }

    /**
     * Retrieve the main window of the applictaion.
     * @return a GtkWidget.
     */
    GtkWidget *getWindow () { return wnd; }

    /**
     * Retrieve a colored 'pen' to draw onto a drawing area.
     * @param col the color to retrieve
     * @return a GdkGC.
     */
    GdkGC *getColor (int col);
    /**
     * Retrieve a colored 'pen' to draw onto a drawing area, depending
     * on the given mode and type.
     * @param mode the mode of a DlgNode
     * @param type the type of a DlgNode
     * @return a GdkGC.
     */
    GdkGC *getColor (mode_type mode, node_type type);
    //@}
    
    /**
     * Set the program state and display it in the status bar
     * @param mode The new program state.
     */
    void setMode (mode_type mode);
    /**
     * Get the current program state
     * @return The program state.
     */
    mode_type mode ()   { return mode_; }
    
    /**
     * Create a new dialogue.
     */
    void newDialogue ();
    /**
     * Load a dialogue from a file
     * @param file Filename (and path) of the dialogue to load.
     */
    void loadDialogue (std::string file);
    /**
     * Save a dialogue to file
     * @param file Filename (and path) of the dialogue to load.
     */
    void saveDialogue (std::string file);
    /**
     * Close a dialogue
     */
    void closeDialogue ();
    /**
     * Display a dialogue
     * @param module The dialogue to display.
     */
    void showDialogue (DlgModule *module);
    /**
     * Compile a dialogue
     */
    void compileDialogue ();
    /**
     * Edit the custom code of the current module.
     */
    void customCode ();
    /**
     * preview translation for a dialogue
     * @param catalogue full path to a gnu gettext compliant binary catalogue
     *        (.mo) file
     */
    void previewTranslation (std::string catalogue);
    /**
     * stop translation preview
     */
    void exitPreview ();
    
    /**
     * Get the directory where the last fileselection took place.
     * @return Path to the directory last opened.
     */
    std::string directory ()  { return directory_+"/"; }
    /**
     * Get the full path and name of the dialogue currently in the view.
     * @return Filename of the current dialogue.
     */
    std::string filename ();
                        
private:
    /**
     * Prepare some colored pens for later use
     * @param widget the widget we later draw to 
     */
    void initColors (GtkWidget *widget);
    /**
     * (Re)build the 'windows' menu 
     */
    void initMenu ();
    /**
     * Display the correct window title
     */
    void initTitle ();
    /**
     * Add a dialogue to the GUI
     */
    DlgModule *initDialogue (std::string);
    /**
     * Set the GUI back to it's initial state.
     */
    void clear ();

    int number;                     // serial number of open dialogues
    mode_type mode_;                // the program mode
    GuiList *list_;                 // instant preview widget
    GuiGraph *graph_;               // dialogue view
    GuiMessages *message;           // statusbar for displaying help/error texts
    GtkWidget *wnd;                 // actual main window
    GtkWidget *menuItem[MAX_ITEM];  // pointers to a few menu-items
    GtkWidget *windowMenu;          // the 'Windows' dropdown menu
    GtkWidget *status_mode;         // statusbar displaying the program state
    GdkFont *font_;                 // font for text-output
    GdkGC *color[MAX_GC];           // custom Pens
    std::vector<DlgModule*> dialogues_;// dialogues currently loaded
    std::string directory_;            // directory used in last file-selection
    static char *progState[NUM_MODES]; // Textual program states
};

#endif // GUI_DLGEDIT_H
