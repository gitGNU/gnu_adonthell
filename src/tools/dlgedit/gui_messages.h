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
 * @file gui_messages.h
 *
 * @author Kai Sterker
 * @brief Defines the messages to display in the status bar.
 */

#ifndef GUI_MESSAGES_H
#define GUI_MESSAGES_H

#include <map>
#include <gtk/gtkstyle.h>

/**
 * operator< for use with the map
 */
struct ltint
{
    bool operator() (const int &a, const int &b) const
    {
        return a < b;
    }
};

/**
 * Contains a map that holds (number, string) pairs, so the messages
 * can be easily accessed by their ID.
 */
class GuiMessages
{
public:
    GuiMessages (GtkWidget *sb);
    
    void display (int id);
    void display (int id, const char* text);
    
    void clear ();
    
private:
    std::map<int, const char*, ltint> messages;  // List of message strings
    int current;                                 // String currently displayed
    GtkWidget *statusbar;                        // Target for the messages
};

#endif // GUI_MESSAGES_H
