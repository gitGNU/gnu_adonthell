/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/* A List-Element for pointer-data */
typedef struct ptr_element
{
    void *data;                 /* Actual Data */
    struct ptr_element *next;   /* Next Element */
    struct ptr_element *prev;   /* Previous Element */
}
ptr_list_element;

/* Double Linked List for storing pointers */
typedef struct
{
    ptr_list_element *head;     /* First Element */
    ptr_list_element *tail;     /* Last Element */
    s_int32 size;               /* Number of Elements */
}
ptr_list;

/* A List-Element for integer-data */
typedef struct int_element
{
    s_int32 data;               /* Actual Data */
    struct int_element *next;   /* Next Element */
    struct int_element *prev;   /* Previous Element */
}
int_list_element;

/* Double Linked List for storing integers */
typedef struct
{
    int_list_element *head;     /* First Element */
    int_list_element *tail;     /* Last Element */
    s_int32 size;               /* Number of Elements */
}
int_list;

/* Double Linked List functions */
void add_ptr_list_element (ptr_list *, void *);
void remove_ptr_list_element (ptr_list *, u_int32);
u_int8 remove_ptr_list_pointer (ptr_list *, void *);
void clear_ptr_list (ptr_list *);
void *get_ptr_list_element (ptr_list *, u_int32);
u_int32 search_ptr_list_element (ptr_list *, void *);
void add_int_list_element (int_list *, s_int32);
void remove_int_list_element (int_list *, u_int32);
void clear_int_list (int_list *);
u_int32 search_int_list_element (int_list *, s_int32);
s_int32 get_int_list_element (int_list *, u_int32);
