/*
  
   $Id$

   Copyright (C) 2000 Jayson S. Baird
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.


  this is the main class for the inventory subsystem.
  it uses a hash table based on the objects name to 
  allocate storage in the inventory. It supports inventory
  growth through prime sequences. A 101 element array can
  easily store hundreds more using linked lists inside the
  element. I felt this was optimal since a person isn't 
  carry that many objects, but allows for growth if they
  do. Searching is fast. A simple matter of grabbing the key
  and doing a linear search down a short list...
  
  methods to add: remove, get, etc.
*/

#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <math.h>

class inventory
{
  public:
    const static int base = 37;
    const static int HASH_SIZE = 101;
    
    inventory ()
    {
        for (int i = 0; i < HASH_SIZE; i++) 
         inven[i] = NULL; 
        total_weight = 0;
    }

    void insert (item * itm)
    {
        int index = 0;
        index = hashcode (itm->getName());
        total_weight += itm->getWeight();

        // if (((inven + index)->getId ()) == 0)
        if (inven[index] == NULL)
        {
            inven[index] = itm;
        }
        else
        {
            item *current = inven[index];

            while (current->next != NULL)
                current = current->next;
            current->next = itm;
        }
    }


    void display ()
    {
        int i = 0;
        cout << "id" << "   " << "name" << "                " << "weight" << endl;
        cout << "--" << "   " << "-----------------" << "   " << "------" << endl;
        while (i < HASH_SIZE)
        {
            item *t = inven[i]; 
            if (t != NULL)
            {
                cout << t->getId () << "    " << t->getName () << "                 " << t->getWeight () << endl;
                if (t->next != NULL)
                {
                    item *current = t->next;
                    while (current != 0)
                    {
                        cout << current->getId () << "    " << current->getName () << "                 " << current->getWeight () << endl;
                        current = current->next;
                    }
                }
            }
            i++;
        }
        cout << "total weight: " << total_weight << endl;
    }

    int hashcode (char name[])
    {
        int index = 0;
        int code = 0;
        int length = strlen (name), max = strlen (name);
        while (index < length)
        {
            code += ((int)name[index] * ((int)pow (base, max)));
            index++;
            max--;
        }
        int var = abs (code % HASH_SIZE);

       /** program segfaults without a cout here.. any ideas? **/
        //cout << "weirdness.." << endl;     
        return var;
    }

  private:
    /* the main item inventory */
    item *inven[HASH_SIZE];
    /* the total weight in this inventory */
    int total_weight;

    // the number of items stored in this inventory
    int num_items;

    // the maximum amount of items that can be stored by this inventory(good for containers...)
    int max_items;
};

#endif
