/*
   $Id$
    
   Copyright (C) 2000 Jayson S. Baird
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.

   This is the main item class for Adonthell
   of course this is version 0.1 super beta 
   extrodinaire. :)
   this was written by me, Jayson Baird
   
   item holds attributes, links and weights
   more stuff to come.
 */

#ifndef _ITEM_H_
#define _ITEM_H_

#include <stdio.h>

class item
{
  public:

    /* the link to the next item in the "bucket", if any. */
    item * next;

    /* the default constructor */
    item ()
    {
        id = 0;
        weight = 0;
        next = NULL;
    }

    /* the more useful common constructor */
    item (int i, char n[], int atrib[], int w, char sg[], item * nxt)
    {
        id = i;
        name = n;
        attribute_array = atrib;
        weight = w;
        source_gfx = sg;
        next = nxt;
    }

    char *getName ()
    {
        return name;
    }

    int getId ()
    {
        return id;
    }

    int getWeight ()
    {
        return weight;
    }

  private:
    /* the id of the item, based on the item database(what gets loaded from file?) */
    int id;

    /* the name of the item */
    char *name;

    /*
       this will be changing to static ints... (thanks kai :) )
       the following array holds the states of the item.
       if the integer at state n == 1 then it has that state
       if not, then it does not have that attribute.
       0 -- moveable field
       1 -- container
       2 -- light
       3 -- locked   this is different than #4 because 4 means it is *never* openable
       4 -- openable
       5 -- combinable 
       add more when necessary...
     */
    int *attribute_array;

    /* this is the items weight */
    int weight;

    /* this is the item's model gfx source */
    char *source_gfx;
};

#endif
