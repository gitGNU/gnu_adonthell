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
#include <gtk/gtk.h>

#include "../../types.h"
#include "../../array_tmpl.h"           // replace by STL vector one day
#include "project.h"

// Project Item stuff follows
// Save an item
void project_item::save (FILE *file)
{
    fwrite (&type, sizeof (type), 1, file);
    fwrite (&id, sizeof (id), 1, file);
    fwrite ((void*)attached.length (), sizeof (u_int32), 1, file);
    fwrite (attached.get_array (), sizeof (u_int32), attached.length (), file);
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
    fread (&i, sizeof (i), 1, file);

    for (i = 0; i < len; i++)
    {
        fread (&val, sizeof (val), 1, file);
        attached.add_element (val);
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
