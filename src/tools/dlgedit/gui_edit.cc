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

// ctor
GuiEdit::GuiEdit (GtkWidget *container)
{
    // let the editor be scrollable
    GtkWidget *scrolled = gtk_scrolled_window_new (0, 0);
    gtk_container_add (GTK_CONTAINER (container), scrolled);
    gtk_container_set_border_width (GTK_CONTAINER (scrolled), 4);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_widget_show (scrolled);

    // create the editor
    entry = gtk_text_new (0, 0);
    gtk_container_add (GTK_CONTAINER (scrolled), entry);
    GTK_WIDGET_SET_FLAGS (entry, GTK_CAN_DEFAULT);
    gtk_text_set_editable (GTK_TEXT (entry), TRUE);
    gtk_text_set_word_wrap (GTK_TEXT (entry), TRUE);
    gtk_widget_show (entry);

    // use a fixed size font
    GtkStyle *style = gtk_style_copy (gtk_widget_get_default_style ());
    style->font = gdk_font_load ("-*-lucidatypewriter-medium-*-*-*-12-*-*-*-*-*-iso8859-1");
    gtk_widget_set_style (entry, style);
}

// dtor
GuiEdit::~GuiEdit ()
{
}

// set the entry's text
void GuiEdit::setText (const std::string &text)
{
    gtk_text_insert (GTK_TEXT (entry), entry->style->font, 
        &entry->style->black, &entry->style->white, text.c_str (), -1);
}

// return the entry's text
std::string GuiEdit::getText ()
{
    gchar *tmp = gtk_editable_get_chars (GTK_EDITABLE (entry), 0, -1);
    std::string text (tmp);
    g_free (tmp);

    return text;
}
