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
 * @file gui_dlgedit.cc
 *
 * @author Kai Sterker
 * @brief The Dialogue Editor's main window
 */

#include <algorithm>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "gui_dlgedit.h"
#include "gui_dlgedit_events.h"

// Icon of the main window
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

// Global pointer to the main window
GuiDlgedit *GuiDlgedit::window = NULL;

// Strings describing the various program states
char *GuiDlgedit::progState[NUM_MODES] = 
    { " IDLE", " SELECTED", " HIGHLIGHTED", " DRAGGED" };

// Create the main window
GuiDlgedit::GuiDlgedit ()
{
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *menu;
    GtkWidget *submenu;
    GtkWidget *menuitem;
    GtkWidget *paned;
    GdkPixmap *icon;
    GdkBitmap *mask;
    GtkAccelGroup *accel_group;
    
    window = this;

    number = 0;
    
    for (int i = 0; i < MAX_GC; i++)
        color[i] = NULL;
    
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

    // Load
    menuitem = gtk_menu_item_new_with_label ("Open");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (2));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_file_load_activate), (gpointer) this);
    gtk_widget_show (menuitem);

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
    
    // Close
    menuitem = gtk_menu_item_new_with_label ("Close");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_w, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (4));
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
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (5));
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
    menuitem = gtk_menu_item_new_with_label ("Settings");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_t, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (10));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_player_activate), (gpointer) NULL);
    gtk_widget_show (menuitem);
    menuItem[SETTINGS] = menuitem;

    // Custom Functions
    menuitem = gtk_menu_item_new_with_label ("Functions");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_f, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (11));
    gtk_signal_connect (GTK_OBJECT (menuitem), "enter-notify-event", GTK_SIGNAL_FUNC (on_display_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "leave-notify-event", GTK_SIGNAL_FUNC (on_clear_help), message);
    gtk_signal_connect (GTK_OBJECT (menuitem), "activate", GTK_SIGNAL_FUNC (on_dialogue_functions_activate), (gpointer) NULL);
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

    // Run
    menuitem = gtk_menu_item_new_with_label ("Run");
    gtk_container_add (GTK_CONTAINER (submenu), menuitem);
    gtk_widget_add_accelerator (menuitem, "activate", accel_group, GDK_r, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_object_set_data (GTK_OBJECT (menuitem), "help-id", GINT_TO_POINTER (13));
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

    paned = gtk_vpaned_new ();
    gtk_box_pack_start (GTK_BOX (vbox), paned, TRUE, TRUE, 2);
    gtk_paned_set_handle_size (GTK_PANED (paned), 5);
    gtk_paned_set_gutter_size (GTK_PANED (paned), 7);
    gtk_widget_show (paned);

    // Accelerators
    gtk_window_add_accel_group (GTK_WINDOW (wnd), accel_group);

    // Drawing Area
    graph_ = new GuiGraph (paned);
    
    // List
    list_ = new GuiList (paned);
    
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

    // font to use on the drawing area
    font_ = gdk_font_load ("-*-*-medium-r-normal-sans-12-*-*-*-*-*-iso8859-1");

    // init the colors we need for drawing
    initColors (graph_->drawingArea ());

    // get the current working directory
    directory_ = g_get_current_dir ();
    
    clear ();
}

// starts a new dialogue
void GuiDlgedit::newDialogue ()
{
    // the name of the dialogue
    gchar *name = g_strdup_printf ("%s/untitled-%i", directory_.c_str (), ++number);
            
    // the new dialogue
    DlgModule *module = initDialogue (name);

    // Display the dialogue
    showDialogue (module);

    g_free (name);
}

// load a new dialogue
void GuiDlgedit::loadDialogue (string file)
{
    // first, open the file
    loadlgin = fopen (file.c_str (), "rb");

    if (!loadlgin)
    {
        message->display (-2, g_basename (file.c_str ()));
        return;
    }

    // get the name to use for the dialogue
    file += g_strdup_printf ("-%i", ++number);

    // remember the current directory for later use
    directory_ = g_dirname (file.c_str ());
    
    // the new dialogue
    DlgModule *module = initDialogue (file);

    // try to load from file
    if (!module->load ())
    {
        message->display (-2, g_basename (filename ().c_str ()));
        closeDialogue ();
    }
    
    // Display the dialogue
    else 
    {
        message->display (200);        
        showDialogue (module);
    }
}

// save a dialogue
void GuiDlgedit::saveDialogue (string file)
{
    DlgModule *module = graph_->dialogue ();
    if (module == NULL) return;
    
    // remember the current directory for later use
    directory_ = g_dirname (file.c_str ());
    
    // append the ".adlg" extension if necessary
    if (file.rfind (".adlg") == file.npos) file += ".adlg";

    // try to save file
    if (!module->save (file)) 
        message->display (-4, g_basename (file.c_str ()));
    else
    {
        message->display (201);
        
        // update the dialogue's name in case it has changed
        module->setName (file);
        initTitle ();
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
    
    // delete the dialogue
    delete module;
}

// display a certain dialogue
void GuiDlgedit::showDialogue (DlgModule *module)
{
    // remove the current module from the view
    graph_->detachModule ();
    
    // attach the dialogue to the view
    graph_->attachModule (module);
    
    // update the window title
    initTitle ();
}

DlgModule *GuiDlgedit::initDialogue (string name)
{
    // the new dialogue
    DlgModule *dlg = new DlgModule (name, "New Dialogue");

    // insert into the list of open dialogues
    dialogues_.push_back (dlg);

    // rebuild the 'Window' menu
    initMenu ();

    // activate all dialogue related menu-items
    gtk_widget_set_sensitive (menuItem[SAVE], TRUE);
    gtk_widget_set_sensitive (menuItem[CLOSE], TRUE);
    gtk_widget_set_sensitive (menuItem[SETTINGS], TRUE);
    gtk_widget_set_sensitive (menuItem[FUNCTIONS], TRUE);
    gtk_widget_set_sensitive (menuItem[COMPILE], TRUE);

    return dlg;
}

// sets the window title
void GuiDlgedit::initTitle ()
{
    gchar *title = "Adonthell Dialogue Editor v"_VERSION_;
    DlgModule *module = graph_->dialogue ();
  
    if (module != NULL)
        title = g_strjoin (NULL, title, " - [", 
                g_basename (module->name().c_str ()), "]", NULL);

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
    vector<DlgModule*>::iterator i;
    GtkAccelGroup *accel_group = gtk_accel_group_get_default ();
    GtkWidget *menuitem;
    gchar *name;
    
    for (i = dialogues_.begin (); i != dialogues_.end (); i++, position++)
    {
        name = (char*) (*i)->name().c_str ();
        menuitem = gtk_menu_item_new_with_label (g_basename (name));
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

// Prepare some colors for later use
void GuiDlgedit::initColors (GtkWidget *graph)
{
    GdkColor c;

    // Yellow
    color[GC_YELLOW] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 65535;
    c.green = 32700;
    c.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (wnd), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_YELLOW], &c);

    // Orange
    color[GC_RED] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 65535;
    c.green = 16350;
    c.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (graph), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_RED], &c);

    // Dark Red
    color[GC_DARK_RED] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 65535;
    c.green = 0;
    c.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (wnd), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_DARK_RED], &c);

    // Dark Blue
    color[GC_DARK_BLUE] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 0;
    c.green = 0;
    c.blue = 35000;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (wnd), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_DARK_BLUE], &c);

    // Green
    color[GC_GREEN] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 16000;
    c.green = 50000;
    c.blue = 5000;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (wnd), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_GREEN], &c);

    // Dark Green
    color[GC_DARK_GREEN] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 0;
    c.green = 27300;
    c.blue = 15600;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (wnd), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_DARK_GREEN], &c);

    // Grey
    color[GC_GREY] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 40000;
    c.green = 40000;
    c.blue = 40000;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (wnd), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_GREY], &c);

    // Black
    color[GC_BLACK] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 0;
    c.green = 0;
    c.blue = 0;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (wnd), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_BLACK], &c);
    
    // White
    color[GC_WHITE] = gdk_gc_new (gtk_widget_get_parent_window (graph));
    c.red = 65535;
    c.green = 65535;
    c.blue = 65535;
    gdk_colormap_alloc_color (gtk_widget_get_colormap (wnd), &c, TRUE, TRUE);
    gdk_gc_set_foreground (color[GC_WHITE], &c);
}

void GuiDlgedit::clear ()
{
    // update the window title
    initTitle ();

    // make the various menu-items insensitive
    gtk_widget_set_sensitive (menuItem[SAVE], FALSE);
    gtk_widget_set_sensitive (menuItem[CLOSE], FALSE);
    gtk_widget_set_sensitive (menuItem[SETTINGS], FALSE);
    gtk_widget_set_sensitive (menuItem[FUNCTIONS], FALSE);
    gtk_widget_set_sensitive (menuItem[COMPILE], FALSE);
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
        text = " INVALID MODE";
    else
        text = progState[mode];
    
    // some context id the statusbar needs for some reason
    int id = gtk_statusbar_get_context_id (GTK_STATUSBAR (status_mode), "Mode");
    
    // remove the previous message
    gtk_statusbar_pop (GTK_STATUSBAR (status_mode), id);
    
    // add the new status
    gtk_statusbar_push (GTK_STATUSBAR (status_mode), id, text);
}


// Get a certain predefined color
GdkGC *GuiDlgedit::getColor (int col)
{
    if (col < 0 || col > MAX_GC) return NULL;
    else return color[col];
}

// Get the right color for a (mode, type) combination
GdkGC *GuiDlgedit::getColor (mode_type mode, node_type type)
{    
    switch (mode)
    {
        // not selected    
        case IDLE:
        {
            if (type == NPC || type == LINK) return color[GC_BLACK];
            else if (type == NARRATOR) return color[GC_DARK_GREEN];
            else return color[GC_DARK_BLUE];
            break;
        }

        // selected
        case NODE_SELECTED:
        {
            if (type == NPC || type == LINK) return color[GC_DARK_RED];
            else if (type == NARRATOR) return color[GC_YELLOW];
            else return color[GC_RED];
            break;
        }

        // highlighted
        case NODE_HILIGHTED:
        {
            return color[GC_GREEN];
            break;
        }

        // unknown
        default:
            break;
    }

    return NULL;
}

// get the full path/name of a dialogue
string GuiDlgedit::filename ()
{
    DlgModule *module = graph_->dialogue ();
    
    // if there is no module, return the current working directory
    if (module == NULL) return directory_;
    string file = module->name ();
    
    // remove the serial number from the name
    unsigned int pos = file.rfind ("-");
    if (pos != file.npos) file.erase (pos);
    
    return file;
}
