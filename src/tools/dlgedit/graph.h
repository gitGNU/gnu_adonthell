/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

int select_object (MainFrame *, GdkPoint);
void select_object_index (MainFrame *, int);
int deselect_object (MainFrame *);
void center_object (MainFrame *, DlgNode *);
int new_circle (MainFrame *, GdkPoint, int);
void draw_circle (MainFrame *, DlgNode *, int);
int new_arrow (MainFrame *, GdkPoint);
void draw_arrow (MainFrame *, DlgNode *, int);
void redraw_arrow (MainFrame *, DlgNode *);
int new_link (MainFrame *, GdkPoint);
int remove_link (MainFrame *, GdkPoint);
DlgNode *get_cur_selection (MainFrame *, GdkPoint);
void show_sel (MainFrame *, DlgNode *);
void show_preview (MainFrame *);
void show_tooltip (MainFrame *, DlgNode *);
void mouse_over (MainFrame *, GdkPoint);
void redraw_graph (MainFrame *);
void new_dialogue (MainFrame *);
void save_dialogue (MainFrame *);
void save_circle (DlgNode *, FILE *, int);
void save_arrow (DlgNode *, FILE *, int, int *);
void load_dialogue (MainFrame *);
void load_circle (MainFrame *, FILE *, int);
void load_arrow (MainFrame *, FILE *);
void new_mover (MainFrame *, GdkPoint);
void move_node (MainFrame *, GdkPoint);
void end_moving (MainFrame *, GdkPoint);
void delete_links (MainFrame *, DlgNode *);
void delete_arrow (MainFrame *, DlgNode *);
void delete_node (MainFrame *);
void edit_node (MainFrame *);
void sort_nodes (MainFrame *);
void free_node (DlgNode *);
