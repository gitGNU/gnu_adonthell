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
 * @file gui_edit.cc
 *
 * @author Kai Sterker
 * @brief Wrapper around the gtkeditor widget.
 */

#include <gtk/gtk.h>
#include "gui_edit.h"

extern "C" 
{
    /* these symbols we get from the (python) scanner */ 
    extern const char *get_name (void); 
    extern const char **get_token_names (void); 
    extern const char **get_block_names (void); 
    extern int get_token_no ();
    extern int get_block_no (); 
    extern void token_func (void (*func)(int id, int precedence, int length, int pos, int block_id, int nesting, int block_type)); 
    extern void char_func (int (*func)()); 
    extern int get_state (void); 
    extern void set_state (int state); 
    extern int lex ();
}

GuiEdit::GuiEdit (GtkWidget *container)
{
    GtkWidget *view;
    GtkTextTag *tag;
    
    // let the editor be scrollable
    GtkWidget *scrolled = gtk_scrolled_window_new (0, 0);
    gtk_container_add (GTK_CONTAINER (container), scrolled);
    gtk_container_set_border_width (GTK_CONTAINER (scrolled), 4);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_widget_show (scrolled);

    // create the editor
    entry = gtk_editor_new ();
    view = gtk_text_view_new_with_buffer(GTK_TEXT_BUFFER (entry));
    gtk_container_add (GTK_CONTAINER (scrolled), view);
    GTK_WIDGET_SET_FLAGS (view, GTK_CAN_DEFAULT);
    gtk_editable_set_editable (GTK_EDITABLE (entry), TRUE);
    // gtk_text_set_word_wrap (GTK_TEXT (entry), TRUE);
    gtk_widget_show (view);
    
    // access the statically linked scanner
    GtkEditorScanner *scanner = gtk_editor_static_scanner (get_name, 
            get_token_names, get_block_names, get_token_no, get_block_no, 
            token_func, char_func, get_state, set_state, lex);
    gtk_editor_install_scanner (entry, scanner);
    
    // define the fonts to use
    char *medium = "-*-lucidatypewriter-medium-*-*-*-12-*-*-*-*-*-*";
    char *bold = "-*-lucidatypewriter-bold-*-*-*-12-*-*-*-*-*-*";

    // set the highlighting
    tag = gtk_editor_create_tag (entry, "keyword");
    gtk_object_set (GTK_OBJECT (tag), "foreground", "#000000", "font", bold, NULL);

    tag = gtk_editor_create_tag (entry, "comment");
    gtk_object_set (GTK_OBJECT (tag), "foreground", "#008000", "font", medium, NULL);

    tag = gtk_editor_create_tag (entry, "string");
    gtk_object_set (GTK_OBJECT (tag), "foreground", "#000080", "font", medium, NULL);

    tag = gtk_editor_create_tag (entry, "type");
    gtk_object_set (GTK_OBJECT (tag), "foreground", "#800000", "font", medium, NULL);

    tag = gtk_editor_create_tag (entry, "function");
    gtk_object_set (GTK_OBJECT (tag), "foreground", "#0080ff", "font", medium, NULL);
    
    tag = gtk_editor_create_tag (entry, "include");
    gtk_object_set (GTK_OBJECT (tag), "foreground", "#0080ff", "font", medium, NULL);

    tag = gtk_editor_create_tag (entry, "constant");
    gtk_object_set (GTK_OBJECT (tag), "foreground", "#0080ff", "font", medium, NULL);
    
    tag = gtk_editor_create_tag (entry, "operator");
    gtk_object_set (GTK_OBJECT (tag), "foreground", "#000000", "font", bold, NULL);
}
