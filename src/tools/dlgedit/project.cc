/*
   $Id$
   
   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <vector>

#include "../../types.h"
#include "project.h"

// Project Item stuff follows
// Save an item
void project_item::save (FILE *file)
{
    u_int32 i;

    fwrite (&type, sizeof (type), 1, file);
    fwrite (&id, sizeof (id), 1, file);
    fwrite ((void*)attached.size (), sizeof (u_int32), 1, file);

    for (i = 0; i < attached.size (); i++)
        fwrite (&attached[i], sizeof (u_int32), 1, file);

    fwrite ((void*)strlen (label), sizeof (u_int32), 1, file);
    fwrite (label, sizeof (char), strlen (label), file);
    fwrite ((void*)(strlen (this->file)+1), sizeof (u_int32), 1, file);
    fwrite (this->file, sizeof (char), strlen (this->file)+1, file);
    fwrite ((void*)(strlen (note)+1), sizeof (u_int32), 1, file);
    fwrite (note, sizeof (char), strlen (note)+1, file);
}

// Load an item
void project_item::load (FILE *file)
{
    u_int32 i, len, val;
    
    fread (&type, sizeof (type), 1, file);
    fread (&id, sizeof (id), 1, file);
    fread (&len, sizeof (len), 1, file);

    attached.reserve (len);

    for (i = 0; i < len; i++)
    {
        fread (&val, sizeof (val), 1, file);
        attached.push_back (val);
    }
    
    fread (&len, sizeof (len), 1, file);
    label = new char[len];
    fread (label, sizeof (char), len, file);

    fread (&len, sizeof (len), 1, file);
    this->file = new char[len];
    fread (this->file, sizeof (char), len, file);

    fread (&len, sizeof (len), 1, file);
    note = new char[len];
    fread (note, sizeof (char), len, file);
}


// Project stuff follows 
// Save the project
void project::save (const char *name)
{
    vector<project_item*>::iterator it;
    FILE *file = fopen (name, "w");

    if (!file) return;
    
    // Adonthell Dialogue Project file version 1
    fputs ("ADP1", file);

    // save number of items
    fwrite ((void *) items.size (), sizeof (int), 1, file);
    
    // Save all items
    for (it = items.begin (); it < items.end (); it++)
        (*it)->save (file);
}

// Load a project
void project::load (const char *name)
{
    FILE *file = fopen (name, "r");
    project_item *pi;    
    char str[5];
    u_int32 size, i;

    // Was file opend correctly?
    if (!file) return;

    // Check if its a correct file
    fgets (str, 5, file);

    if (g_strcasecmp ("ADP1", str))
    {
        g_message ("%s is no valid project file", name);
        return;
    }

    // Load number of items
    fread (&size, sizeof (size), 1, file);

    // Set size of the vector 
    items.reserve (size);

    // and load all items
    for (i = 0; i < size; i++)
    {
        pi = new project_item;
        pi->load (file); 
        items.push_back (pi);
    }

    // Clean up
    fclose (file);
}
