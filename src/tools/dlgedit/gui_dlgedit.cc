/*
   $Id$

   Copyright (C) 2002/2003 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/** 
 * @file gui_dlgedit.cc
 *
 * @author Kai Sterker
 * @brief The Dialogue Editor's main window
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <algorithm>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "gettext.h"
#include "cfg_data.h"
#include "dlg_cmdline.h"
#include "dlg_compiler.h"
#include "gui_code.h"
#include "gui_settings.h"
#include "gui_resources.h"
#include "gui_dlgedit.h"
#include "gui_dlgedit_events.h"

/**
 * Point parser to another file to load sub-dialogue.
 * Defined in loadlg.l
 */
extern void parser_switch_input ();

/**
 * Return to previous input file after loading sub-dialogue - or 
 * after failing to do so.
 * Defined in loadlg.l
 */
extern void parser_restore_input ();

/**
 * Icon of the main window
 */
static char * icon_xpm[] = {
"16 16 13 1",
" 	c None",
".	c #888888",
"+	c #777777",
"@	c #999999",
"#	c #111111",
"$	c #222222",
"%	c #000000",
"&	c #333333",
"*	c #555555",
"=	c #666666",
"-	c #444444",
";	c #070707",
">	c #AAAAAA",
"     .+++.@     ",
"    .#$%&**@    ",
"    =%-#=  $*   ",
"    #*+%+  .%-  ",
"   .# .;+   *%= ",
"   =- =;+   .## ",
"   #= =#=    $%.",
"  .%%##%$*==.&%+",
"  +-  =%-@   *%*",
"  #%%$#%$**=.&%+",
"  %-++*#=    $%.",
" =#+  =%+   .%& ",
" -$   =%+   $%. ",
"+%+   +%+  *#+  ",
"-%.   =%-*=$.   ",
"=*.   @=+..>    "};

/**
 * Global pointer to the main window
 */
GuiDlgedit *GuiDlgedit::window = NULL;

/**
 * Strings describing the various program states
 */
char *GuiDlgedit::progState[NUM_MODES] = 
    { " IDLE", " SELECTED", " HIGHLIGHTED", " DRAGGED", " PREVIEW" };

// Create the main window
GuiDlgedit::GuiDlgedit ()
{
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *menu;
    GtkWidget *submenu;
    GtkWidget *menuitem;
    GtkWidget *hpaned, *vpaned;
    GdkPixmap *icon;
    GdkBitmap *mask;
    GtkAccelGroup *accel_group;
    
    window = this;

    number = 0;
    
    // Statusbar for displaying help and error messages
    GtkWidget *status_help = gtk_statusbar_new ();
    message = new GuiMessages (status_help);
        
    // Main Window
    wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize (GTK_WIDGET (wnd), 800, 600);
    gtk_signal_connect (GTK_OBJECT (wnd), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);
            
    // Menu Accelerators
    accel_group = gtk_accel_group_get_default ();

    // Main Windows Menu
    menu = gtk_menu_bar_new ();

    // Attach Menubar
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (wnd), vbox);
    gtk_widget_show (vbox);

    gtk_box_pack_start (GTK_BOX (vbox), menu, FALSE, FALSE, 2);

    // File Menu
    submenu = gtk_menu_new ();

    // New
    menuitem = gtk_menu_item_new_with_label ("New");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (1));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_new_activate), (gpointer) this);
    gtk_widget_show (menuitem);

    // Open
    menuitem = gtk_menu_item_new_with_label ("Open ...");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (2));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_load_activate), (gpointer) this);
    gtk_widget_show (menuitem);

    // Open Previous >
    menuitem = gtk_menu_item_new_with_label ("Open Previous");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (2));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_widget_show (menuitem);
    menuItem[OPEN_RECENT] = menuitem;
    
    // Save
    menuitem = gtk_menu_item_new_with_label ("Save");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (3));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_save_activate), (gpointer) this);
    gtk_widget_show (menuitem);
    menuItem[SAVE] = menuitem;
     
    // Save As
    menuitem = gtk_menu_item_new_with_label ("Save As ...");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (4));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_save_as_activate), (gpointer) this);
    gtk_widget_show (menuitem);
    menuItem[SAVE_AS] = menuitem;

    // Seperator
    menuitem = gtk_menu_item_new ();
    gtk_menu_append (GTK_MENU (submenu), menuitem);
    gtk_widget_set_sensitive (menuitem, FALSE);
    gtk_widget_show (menuitem);
    
    // Revert to Saved
    menuitem = gtk_menu_item_new_with_label ("Revert to Saved");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_r, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (7));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_revert_activate), (gpointer) NULL);
    gtk_widget_show (menuitem);
    menuItem[REVERT] = menuitem;
    
    // Close
    menuitem = gtk_menu_item_new_with_label ("Close");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_w, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (5));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_close_activate), (gpointer) NULL);
    gtk_widget_show (menuitem);
    menuItem[CLOSE] = menuitem;
    
    // Seperator
    menuitem = gtk_menu_item_new ();
    gtk_menu_append (GTK_MENU (submenu), menuitem);
    gtk_widget_set_sensitive (menuitem, FALSE);
    gtk_widget_show (menuitem);

    // Quit
    menuitem = gtk_menu_item_new_with_label ("Quit");
    gtk_menu_append (GTK_MENU (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (6));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_widget_destroy), (gpointer) NULL);
    gtk_widget_show (menuitem);

    // Attach File Menu
    menuitem = gtk_menu_item_new_with_label ("File");
    gtk_widget_show (menuitem);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), submenu);
    gtk_menu_bar_append (GTK_MENU_BAR (menu), menuitem);

    // Dialogue Menu
    submenu = gtk_menu_new ();
    
    // Settings
    menuitem = gtk_menu_item_new_with_label ("Settings ...");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_t, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (10));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_player_activate), (gpointer) NULL);
    gtk_widget_show (menuitem);
    menuItem[SETTINGS] = menuitem;

    // Custom Functions
    menuitem = gtk_menu_item_new_with_label ("Python Code ...");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_p, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (11));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_functions_activate), (gpointer) this);
    gtk_widget_show (menuitem);
    menuItem[FUNCTIONS] = menuitem;

    // Seperator
    menuitem = gtk_menu_item_new ();
    gtk_menu_append (GTK_MENU (submenu), menuitem);
    gtk_widget_set_sensitive (menuitem, FALSE);
    gtk_widget_show (menuitem);

    // Compile
    menuitem = gtk_menu_item_new_with_label ("Compile");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_c, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (12));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_compile_activate), (gpointer) NULL);
    gtk_widget_show (menuitem);
    menuItem[COMPILE] = menuitem;

    // Preview i18n
#ifdef ENABLE_NLS 
    menuitem = gtk_menu_item_new_with_label ("Preview L10n ...");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_l, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (13));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_preview_activate), (gpointer) this);
    gtk_widget_show (menuitem);
    menuItem[PREVIEW] = menuitem;
#endif
    
    // Run
    menuitem = gtk_menu_item_new_with_label ("Execute");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_e, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (14));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_run_activate), (gpointer) NULL);
    gtk_widget_show (menuitem);
    menuItem[RUN] = menuitem;

    // Attach Dialogue Menu
    menuitem = gtk_menu_item_new_with_label ("Dialogue");
    gtk_widget_show (menuitem);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), submenu);
    gtk_menu_bar_append (GTK_MENU_BAR (menu), menuitem);

    // Window Menu
    windowMenu = gtk_menu_new ();

    // Attach Window Menu
    menuitem = gtk_menu_item_new_with_label ("Windows");
    gtk_widget_show (menuitem);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), windowMenu);
    gtk_menu_bar_append (GTK_MENU_BAR (menu), menuitem);

    gtk_widget_show (menu);

    vpaned = gtk_vpaned_new ();
    gtk_box_pack_start (GTK_BOX (vbox), vpaned, TRUE, TRUE, 2);
    gtk_paned_set_handle_size (GTK_PANED (vpaned), 5);
    gtk_paned_set_gutter_size (GTK_PANED (vpaned), 7);
    gtk_widget_show (vpaned);

    hpaned = gtk_hpaned_new ();
    gtk_paned_add1 (GTK_PANED (vpaned), hpaned);
    gtk_paned_set_handle_size (GTK_PANED (hpaned), 5);
    gtk_paned_set_gutter_size (GTK_PANED (hpaned), 7);
    gtk_widget_show (hpaned);
    
    // Accelerators
    gtk_window_add_accel_group (GTK_WINDOW (wnd), accel_group);

    gtk_widget_realize (wnd);
    
    // Tree
    tree_ = new GuiTree (hpaned);
    
    // Drawing Area
    graph_ = new GuiGraph (hpaned);
    
    // List
    list_ = new GuiList (vpaned);
    
    // Status bars
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_widget_ref (hbox);
    gtk_object_set_data_full (GTK_OBJECT (wnd), "hbox", hbox, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
    gtk_widget_set_usize (hbox, -2, 20);

    // help message
    gtk_widget_ref (status_help);
    gtk_object_set_data_full (GTK_OBJECT (wnd), "status_help", status_help, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (status_help);
    gtk_box_pack_start (GTK_BOX (hbox), status_help, TRUE, TRUE, 0);
    gtk_widget_set_usize (status_help, -2, 20);

    // program mode
    status_mode = gtk_statusbar_new ();
    gtk_widget_ref (status_mode);
    gtk_object_set_data_full (GTK_OBJECT (wnd), "status_mode", status_mode, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (status_mode);
    gtk_box_pack_start (GTK_BOX (hbox), status_mode, FALSE, TRUE, 0);
    gtk_widget_set_usize (status_mode, 150, -2);
    
    // set the editor's icon    
    gtk_widget_realize (wnd);
    icon = gdk_pixmap_create_from_xpm_d (wnd->window, &mask, NULL, icon_xpm);
    gdk_window_set_icon (wnd->window, wnd->window, icon, mask);

    // Display MainWindow
    gtk_widget_show (wnd);

    // set the programm mode
    setMode (IDLE);

    // Display welcome message
    message->display (1000);

    // init list of previously opened files
    initRecentFiles ();
    
    // get the current working directory
    directory_ = g_get_current_dir ();
    
    // init the resources we will need
    GuiResources::init (graph_->drawingArea ());

    clear ();
}

// dtor
GuiDlgedit::~GuiDlgedit ()
{
    // cleanup if in preview mode
    if (mode_ == L10N_PREVIEW)
    {
        unlink ("/tmp/locale/xy/LC_MESSAGES/preview.mo");
        rmdir ("/tmp/locale/xy/LC_MESSAGES");
        rmdir ("/tmp/locale/xy");
        rmdir ("/tmp/locale/");    
    }
}

// starts a new dialogue
void GuiDlgedit::newDialogue ()
{
    // the new dialogue
    DlgModule *module = initDialogue ("untitled");

    // Display the dialogue
    showDialogue (module);
}

// load a new dialogue
void GuiDlgedit::loadDialogue (const std::string &f)
{
    // make sure that file has an absolute path
    std::string file = (f[0] == '/' ? f : directory_ + std::string ("/") + f);
    
    // test if we have a valid dialogue
    if (!checkDialogue (file))
    {        
        // update list of previously opened files
        CfgData::data->removeFile (file);
        initRecentFiles ();
    
        message->display (-2, g_basename (file.c_str ()));
        return;
    }

    // remember the current directory for later use
    directory_ = g_dirname (file.c_str ());
    
    // get the name to use for the dialogue
    std::string filename = g_basename (file.c_str ());
    
    // remove file extension
    unsigned int pos = filename.rfind (FILE_EXT);
    if (pos != filename.npos) filename.erase (pos);

    // the new dialogue
    DlgModule *module = initDialogue (filename);

    // try to load from file
    if (!module->load ())
    {
        message->display (-3, filename.c_str ());
        closeDialogue ();
    }
    
    // Display the dialogue
    else 
    {
        // update list of previously opened files
        CfgData::data->addFile (file);
        initRecentFiles ();

        message->display (200);     
        showDialogue (module, true);
    }
}

// load a sub-dialogue
DlgModule* GuiDlgedit::loadSubdialogue (const std::string &file)
{
    // test if we have a valid dialogue
    if (!checkDialogue (file)) return NULL;

    // remember the current directory for later use
    directory_ = g_dirname (file.c_str ());

    // get the name to use for the dialogue
    std::string filename = g_basename (file.c_str ());

    // remove file extension
    unsigned int pos = filename.rfind (FILE_EXT);
    if (pos != filename.npos) filename.erase (pos);

    // the sub-dialogue
    DlgModule *module = new DlgModule (directory_, filename, "", "");

    // parser needs to read from sub-dialogue source file
    parser_switch_input ();

    // try to load from file
    if (!module->load ())
    {
        delete module;
        parser_restore_input ();
        return NULL;
    }

    // return the sub-dialogue
    return module;    
}

// revert Dialogue to state on disk
void GuiDlgedit::revertDialogue ()
{
    DlgModule *module = graph_->dialogue ();
    if (module == NULL) return;

    // check whether dialogue (still) exists on disk
    if (!checkDialogue (module->fullName ()))
    {
        gtk_widget_set_sensitive (menuItem[REVERT], FALSE);
        message->display (-2, module->name ().c_str ());
        return;
    }

    // cleanup
    module->clear ();
    
    // reload
    if (!module->load ())
    {
        message->display (-3, module->name ().c_str ());
        closeDialogue ();
        return;
    }
    
    // redisplay
    graph_->detachModule ();
    tree_->updateModule (module);
    graph_->attachModule (module);
}

// save a dialogue
void GuiDlgedit::saveDialogue (const std::string &file)
{
    DlgModule *module = graph_->dialogue ();
    if (module == NULL) return;
    
    // remember the current directory for later use
    directory_ = g_dirname (file.c_str ());
    
    // get the filename
    std::string filename = g_basename (file.c_str ());
    
    // remove file extension
    unsigned int pos = filename.rfind (FILE_EXT);
    if (pos != filename.npos) filename.erase (pos);

    // try to save file
    if (!module->save (directory_, filename)) 
        message->display (-4, filename.c_str ());
    else
    {
        message->display (201);
        
        // update list of previously opened files
        CfgData::data->addFile (file);
        initRecentFiles ();

        // update 'Revert to Saved' menu item
        gtk_widget_set_sensitive (menuItem[REVERT], TRUE);
             
        // update the dialogue's name in case it has changed
        tree_->setName (module);
        initMenu ();
    }  
}

// close the dialogue being displayed
void GuiDlgedit::closeDialogue ()
{
    DlgModule *module = graph_->dialogue ();
    if (module == NULL) return;
    
    // check whether dialogue has been saved
    // if (module->hasChanged ())
    
    // remove the dialogue from the list of open dialogues
    dialogues_.erase (remove (dialogues_.begin (), dialogues_.end (), module), dialogues_.end ());
    
    // detach module
    graph_->detachModule ();
    
    // if another dialogue is open, display that one
    if (dialogues_.size () > 0) showDialogue (dialogues_.front ());
    // otherwise just clear the GUI
    else clear ();
    
    // rebuild the 'windows' menu
    initMenu ();
    
    // update list of previously opened files
    initRecentFiles ();
    
    // delete the dialogue
    delete module;
}

// display a certain dialogue
void GuiDlgedit::showDialogue (DlgModule *module, bool center)
{
    // remove the current module from the view
    graph_->detachModule ();
    
    // update the tree view
    // NOTE that this method does some magic: it will select and attach
    // the sub-dialogue of 'module' that has been viewed before. In that
    // case, 'module' must not be attached, as it is the toplevel dialogue.
    tree_->addModule (module);
    
    // attach the dialogue to the view
    // In case of a newly created or (re)loaded 'module', none of it's
    // sub-dialogues will have been in the view. Therefore, the call
    // above can't attach anything. Therefore we can attach 'module'.
    if (!graph_->getAttached ()) graph_->attachModule (module, center);
    
    // update the custom code entry if neccessary
    if (GuiCode::dialog != NULL)
        GuiCode::dialog->display (module->entry (), module->shortName ());
    
    // update the settings if neccessary
    if (GuiSettings::dialog != NULL)
        GuiSettings::dialog->display (module->entry (), module->shortName ());

    // update 'Revert to Saved' menu item
    if (!checkDialogue (module->fullName ()))
        gtk_widget_set_sensitive (menuItem[REVERT], FALSE);
    else 
        gtk_widget_set_sensitive (menuItem[REVERT], TRUE);
             
    // update the window title
    initTitle ();
}

// compile a dialogue
void GuiDlgedit::compileDialogue ()
{
    DlgModule *module = graph_->dialogue ();
    if (module == NULL) return;

    // init the compiler
    DlgCompiler compiler (module);
    
    // compile
    compiler.run ();
    
    // enable the 'run' menuitem after successful compilation
    gtk_widget_set_sensitive (menuItem[RUN], TRUE);
    
    // report success
    message->display (212);
}

// edit the genral dialogu settings
void GuiDlgedit::settings ()
{
    DlgModule *module = graph_->dialogue ();
    if (module == NULL) return;

    // if the dialog isn't already open ...
    if (GuiSettings::dialog == NULL) 
        GuiSettings::dialog = new GuiSettings ();
    
    // otherwise just show it
    std::string project = module->entry ()->project ();
    GuiSettings::dialog->display (module->entry (), module->shortName ());
}

// edit custom code of current module
void GuiDlgedit::customCode ()
{
    DlgModule *module = graph_->dialogue ();
    if (module == NULL) return;
    
    // if the dialog isn't already open ...
    if (GuiCode::dialog == NULL) 
        GuiCode::dialog = new GuiCode ();
    
    // otherwise just show it
    GuiCode::dialog->display (module->entry (), module->shortName ());
}

// preview the translated dialogue
void GuiDlgedit::previewTranslation (const std::string &catalogue)
{
    DlgModule *module = graph_->dialogue ();
    if (module == NULL) return;

    // check if we have a proper catalogue
    if (strncmp (catalogue.substr (catalogue.length ()-3).c_str (), ".mo", 3))
    {
        message->display (-130, g_basename (catalogue.c_str ()));
        return;
    }
    
    // see if the file exists at all
    FILE *exists = fopen (catalogue.c_str (), "rb");

    if (!exists)
    {
        message->display (-2, g_basename (catalogue.c_str ()));
        return;
    }
    // if it does, check magic number of catalogue file
    else
    {
        unsigned int magic;
        
        // read magic number
        fread (&magic, 4, 1, exists);
        fclose (exists);
    
        if (magic != 0x950412de)
        {
            message->display (-130, g_basename (catalogue.c_str ()));
            return;
        }
    }
    
    // create temporary locale directory
    if (mkdir ("/tmp/locale/", 0750) ||
        mkdir ("/tmp/locale/xy", 0750) ||
        mkdir ("/tmp/locale/xy/LC_MESSAGES", 0750))
    {
        message->display (-131);
        return;
    }
            
    // create a symlink to the given catalogue
    symlink (catalogue.c_str (), "/tmp/locale/xy/LC_MESSAGES/preview.mo");
    
    // set the language to use  
    setenv ("LANGUAGE", "xy", 1);
    
#ifdef ENABLE_NLS
    {
        // tell gettext that the language has changed
        extern int _nl_msg_cat_cntr;
        ++_nl_msg_cat_cntr;
    }
#endif
    
    // open the catalogue
    bindtextdomain ("preview", "/tmp/locale");
    textdomain ("preview");
    
    // deselect selected node, if any
    DlgNode *node = module->deselectNode ();
    
    // update menuitem
    gtk_label_set_text (GTK_LABEL (GTK_BIN (menuItem[PREVIEW])->child), "Exit Preview mode");
    
    // set program mode
    setMode (L10N_PREVIEW);
    message->display (130);
    
    // reselect node with proper translation
    if (node != NULL) module->selectNode (node);
}

// stop preview mode
void GuiDlgedit::exitPreview ()
{
    DlgModule *module = graph_->dialogue ();

    // clear the program mode
    setMode (NUM_MODES);
    
    // restore the program mode
    if (module != NULL) 
    {
        setMode (module->state ());
        
        // deselect selected node, if any
        DlgNode *node = module->deselectNode ();
    
        // reselect node without translation
        if (node != NULL) module->selectNode (node);
    }
    
    // update menuitem
    gtk_label_set_text (GTK_LABEL (GTK_BIN (menuItem[PREVIEW])->child), "Preview Translation");

    // cleanup
    unlink ("/tmp/locale/xy/LC_MESSAGES/preview.mo");
    rmdir ("/tmp/locale/xy/LC_MESSAGES");
    rmdir ("/tmp/locale/xy");
    rmdir ("/tmp/locale/");

    // clear the statusbar
    message->clear ();
}

void GuiDlgedit::setChanged ()
{
    // update tree
    tree_->setChanged (graph_->dialogue ());
    
    // update title bar
    initTitle ();
}

void GuiDlgedit::updateProject ()
{
    // update tree
    tree_->updateProject (graph_->dialogue ());
}

bool GuiDlgedit::checkDialogue (const std::string &file)
{
    // first, open the file
    FILE *test = fopen (file.c_str (), "rb");

    if (!test)
        return false;
    
    // check if it's a regular file
    struct stat statbuf;
    fstat (fileno (test), &statbuf);
    if (!S_ISREG (statbuf.st_mode))
    {
        fclose (test);
        return false;
    }
    
    loadlgin = test;
    return true;
}

DlgModule *GuiDlgedit::initDialogue (std::string name)
{
    // serial number
    std::string serial = g_strdup_printf ("-%i", ++number);

    // the new dialogue
    DlgModule *dlg = new DlgModule (directory_, name, serial, "New Dialogue");

    // set project if dlgedit started with '-j' option
    dlg->entry()->setProject (DlgCmdline::project);
    
    // insert into the list of open dialogues
    dialogues_.push_back (dlg);

    // rebuild the 'Window' menu
    initMenu ();

    // activate all dialogue related menu-items
    gtk_widget_set_sensitive (menuItem[SAVE], TRUE);
    gtk_widget_set_sensitive (menuItem[SAVE_AS], TRUE);
    gtk_widget_set_sensitive (menuItem[CLOSE], TRUE);
    gtk_widget_set_sensitive (menuItem[SETTINGS], TRUE);
    gtk_widget_set_sensitive (menuItem[FUNCTIONS], TRUE);
    gtk_widget_set_sensitive (menuItem[COMPILE], TRUE);
#ifdef ENABLE_NLS
    gtk_widget_set_sensitive (menuItem[PREVIEW], TRUE);
#endif
    
    return dlg;
}

// sets the window title
void GuiDlgedit::initTitle ()
{
    gchar *title = "Adonthell Dialogue Editor v"_VERSION_;
    DlgModule *module = graph_->dialogue ();
  
    if (module != NULL)
    {
        if (module->changed ())
            title = g_strjoin (NULL, title, " - [", 
                module->shortName ().c_str (), " (modified)]", NULL);
        else
            title = g_strjoin (NULL, title, " - [", 
                module->shortName ().c_str (), "]", NULL);
    }
    gtk_window_set_title (GTK_WINDOW (wnd), title);
}

// init the 'windows' menu
void GuiDlgedit::initMenu ()
{
    // first, remove everything from the menu
    gtk_container_foreach (GTK_CONTAINER (windowMenu), (GtkCallback)gtk_widget_destroy, NULL);

    // Now rebuild the menu if any dialogues remain
    if (dialogues_.size () == 0) return;

    int position = 0;   
    std::vector<DlgModule*>::iterator i;
    GtkAccelGroup *accel_group = gtk_accel_group_get_default ();
    GtkWidget *menuitem;
    
    for (i = dialogues_.begin (); i != dialogues_.end (); i++, position++)
    {
        menuitem = gtk_menu_item_new_with_label ((*i)->shortName ().c_str ());
        gtk_container_add (GTK_CONTAINER (windowMenu), menuitem);
        gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_window_activate), (gpointer) *i);
        gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (20));
        gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
        gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
        gtk_widget_show (menuitem);
    
        // Menu Accelerators
        if (position < 9)
            gtk_widget_add_accelerator (menuitem, "activate", accel_group, 
                GDK_1 + position, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    }
}

// initialize the list of recently opened files
void GuiDlgedit::initRecentFiles ()
{
    GtkWidget *submenu = GTK_MENU_ITEM(menuItem[OPEN_RECENT])->submenu;
    GtkWidget *menuitem;
    
    // first, remove everything from the submenu
    if (submenu != NULL)
        gtk_container_foreach (GTK_CONTAINER (submenu), (GtkCallback) gtk_widget_destroy, NULL);
    else
        submenu = gtk_menu_new ();

    // get list of files
    std::list<std::string> files = CfgData::data->getFiles ();
    
    // now recreate the recent files list
    for (std::list<std::string>::iterator i = files.begin (); i != files.end (); i++)
    {
        menuitem = gtk_menu_item_new_with_label ((*i).c_str ());
        gtk_container_add (GTK_CONTAINER (submenu), menuitem);
        gtk_object_set_user_data (GTK_OBJECT (menuitem), (void *) (*i).c_str ());
        gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_load_recent_activate), (gpointer) this);
        gtk_widget_show (menuitem);          
    }

    // append submenu
    gtk_menu_item_set_submenu (GTK_MENU_ITEM(menuItem[OPEN_RECENT]), submenu);
}

void GuiDlgedit::clear ()
{
    // update the window title
    initTitle ();

    // make the various menu-items insensitive
    gtk_widget_set_sensitive (menuItem[SAVE], FALSE);
    gtk_widget_set_sensitive (menuItem[SAVE_AS], FALSE);
    gtk_widget_set_sensitive (menuItem[REVERT], FALSE);
    gtk_widget_set_sensitive (menuItem[CLOSE], FALSE);
    gtk_widget_set_sensitive (menuItem[SETTINGS], FALSE);
    gtk_widget_set_sensitive (menuItem[FUNCTIONS], FALSE);
    gtk_widget_set_sensitive (menuItem[COMPILE], FALSE);
#ifdef ENABLE_NLS
    gtk_widget_set_sensitive (menuItem[PREVIEW], FALSE);
#endif
    gtk_widget_set_sensitive (menuItem[RUN], FALSE);
    
    // empty the graph and list widget
    graph_->clear ();
    list_->clear ();
}

void GuiDlgedit::setMode (mode_type mode)
{
    char *text;
    
    // get the string representing the current program state   
    if (mode < IDLE || mode >= NUM_MODES)
    {
        text = " INVALID MODE";
        mode_ = IDLE;
    }
    else
    {
        // ignore everything else as long as we are in preview mode
        if (mode_ == L10N_PREVIEW) return;
        
        text = progState[mode];
        mode_ = mode;
    }
    
    // some context id the statusbar needs for some reason
    int id = gtk_statusbar_get_context_id (GTK_STATUSBAR (status_mode), "Mode");
    
    // remove the previous message
    gtk_statusbar_pop (GTK_STATUSBAR (status_mode), id);
    
    // add the new status
    gtk_statusbar_push (GTK_STATUSBAR (status_mode), id, text);
}

// get the full path/name/extension of a dialogue
std::string GuiDlgedit::filename ()
{
    DlgModule *module = graph_->dialogue ();
    
    return module->fullName ();
}
