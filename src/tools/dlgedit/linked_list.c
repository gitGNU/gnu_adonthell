/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

#include <stdlib.h>

#include "../../types.h"
#include "linked_list.h"

/* Insert new Element at End of list */
void 
add_ptr_list_element (ptr_list * list, void *data)
{
    ptr_list_element *element = (ptr_list_element *) malloc (sizeof (ptr_list_element));

    /* first Element */
    if (list->head == NULL)
    {
        list->head = element;
        list->tail = element;
        element->prev = NULL;
    }

    /* second and later elements */
    else
    {
        list->tail->next = element;
        element->prev = list->tail;
        list->tail = element;
    }

    element->data = data;
    element->next = NULL;
    list->size++;
}

/* Returns data stored at nth position of list */
void *
get_ptr_list_element (ptr_list * list, u_int32 index)
{
    u_int32 i;
    ptr_list_element *element = list->head;

    if (index >= (u_int32) list->size)
        return NULL;

    for (i = 0; i < index; i++)
        element = element->next;

    return element->data;
}

/* Deletes node at nth position */
void 
remove_ptr_list_element (ptr_list * list, u_int32 index)
{
    u_int32 i;
    ptr_list_element *element = list->head;

    if (index >= (u_int32) list->size)
        return;

    for (i = 0; i < index; i++)
        element = element->next;

    /* first element? */
    if (element->prev == NULL)
        list->head = element->next;
    else
        element->prev->next = element->next;

    /* last element? */
    if (element->next == NULL)
        list->tail = element->prev;
    else
        element->next->prev = element->prev;

    /* delete element */
    free (element);

    list->size--;
}

/* delete the given pointer from the list */
u_int8
remove_ptr_list_pointer (ptr_list * list, void * data)
{
    u_int32 index = search_ptr_list_element (list, data);
    
    if (index == 0) 
        return index;
    else
        remove_ptr_list_element (list, index-1);
        
    return 1;
}

/* Searches for element and returns position + 1 if found */
u_int32 
search_ptr_list_element (ptr_list * list, void *data)
{
    u_int32 i;
    ptr_list_element *element = list->head;

    for (i = 1; i <= (u_int32) list->size; i++, element = element->next)
        if (element->data == data)
            return i;

    return 0;
}

/* Empty whole List */
void 
clear_ptr_list (ptr_list * list)
{
    u_int32 i;
    u_int32 num = list->size;

    for (i = 0; i < num; i++)
        remove_ptr_list_element (list, 0);

    list->head = NULL;
}


/* Insert new Element at End of list */
void 
add_int_list_element (int_list * list, s_int32 data)
{
    int_list_element *element = (int_list_element *) malloc (sizeof (int_list_element));

    /* first Element */
    if (list->head == NULL)
    {
        list->head = element;
        list->tail = element;
        element->prev = NULL;
    }
    /* second and later elements */
    else
    {
        list->tail->next = element;
        element->prev = list->tail;
        list->tail = element;
    }

    element->data = data;
    element->next = NULL;
    list->size++;
}

/* Returns data stored at nth position of list */
s_int32 
get_int_list_element (int_list * list, u_int32 index)
{
    u_int32 i;
    int_list_element *element = list->head;

    if (index >= (u_int32) list->size)
        return 0;

    for (i = 0; i < index; i++)
        element = element->next;

    return element->data;
}

/* Deletes node at nth position */
void 
remove_int_list_element (int_list * list, u_int32 index)
{
    u_int32 i;
    int_list_element *element = list->head;

    if (index >= (u_int32) list->size)
        return;

    for (i = 0; i < index; i++)
        element = element->next;

    /* first element? */
    if (element->prev == NULL)
        list->head = element->next;
    else
        element->prev->next = element->next;

    /* last element? */
    if (element->next == NULL)
        list->tail = element->prev;
    else
        element->next->prev = element->prev;

    /* delete everything */
    free (element);

    list->size--;
}

/* Searches for element and returns position + 1 if found */
u_int32 
search_int_list_element (int_list * list, s_int32 data)
{
    u_int32 i;
    int_list_element *element = list->head;

    for (i = 1; i <= (u_int32) list->size; i++, element = element->next)
        if (element->data == data)
            return i;

    return 0;
}

/* Empty whole List */
void 
clear_int_list (int_list * list)
{
    u_int32 i;
    u_int32 num = list->size;

    for (i = 0; i < num; i++)
        remove_int_list_element (list, 0);

    list->head = NULL;
}
