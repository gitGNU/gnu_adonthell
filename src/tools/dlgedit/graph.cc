/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

class dialog;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "../../types.h"
#include "../../dlg_io.h"
#include "../../interpreter.h"
#include "linked_list.h"
#include "dlgnode.h"
#include "main.h"
#include "geometrie.h"
#include "compile.h"
#include "dlgrun.h"
#include "interface.h"
#include "graph.h"

/* Create a new circle */
int 
new_circle (MainFrame * wnd, GdkPoint point, int type)
{
    NodeData *cbd = (NodeData *) g_malloc (sizeof (NodeData));
    DlgNode *circle = (DlgNode *) g_malloc (sizeof (DlgNode));
    int retval;

    /* Init Circle */
    circle->number = wnd->number;
    circle->position.x = point.x - 10;
    circle->position.y = point.y - 10;
    circle->position.width = 21;
    circle->position.height = 21;
    circle->text = NULL;
    circle->type = type;

    /* Init Dialog-Callback-Data */
    cbd->wnd = wnd;
    cbd->node = circle;
    cbd->retval = 0;

    /* Create and display dialog for user-input */
    wnd->text_dlg = create_text_dialog (cbd);
    gtk_widget_show (wnd->text_dlg);
    gtk_window_set_modal ((GTK_WINDOW (wnd->text_dlg)), TRUE);

    /* Enter Dialog - Event - Loop */
    gtk_main ();

    retval = cbd->retval;

    /* Circle created */
    if (retval == 1)
    {
        /* Further initialization */
        circle->prev = g_ptr_array_new ();
        circle->next = g_ptr_array_new ();
        circle->link = g_ptr_array_new ();

        /* One Node more */
        wnd->number++;

        /* Add to Array */
        add_ptr_list_element (wnd->nodes, circle);

        /* Display Node */
        draw_circle (wnd, circle, 0);
    }
    else
        g_free (circle);

    g_free (cbd);

    return retval;
}

/* create new arrow */
int 
new_arrow (MainFrame * wnd, GdkPoint point)
{
    DlgNode *arrow = (DlgNode *) g_malloc (sizeof (DlgNode));
    DlgNode *end = get_cur_selection (wnd, point);
    int type;

    /* Exit function if  
       - no node is marked
       - the marked node is no circle
       - the clicked node is no circle
       - the clicked node is the marked node */
    if (wnd->selected_node == NULL)
        return 0;
    if (wnd->selected_node->type == LINK)
        return 0;
    if (end != NULL && end->type == LINK)
        return 0;
    if (wnd->selected_node == end)
        return 0;

    /* Init arrow */
    arrow->prev = g_ptr_array_new ();
    arrow->next = g_ptr_array_new ();
    arrow->link = g_ptr_array_new ();
    arrow->number = wnd->number;
    arrow->text = NULL;
    arrow->type = LINK;

    /* Create new text */

    /* Add to Array */
    add_ptr_list_element (wnd->nodes, arrow);

    wnd->number++;

    /* Not clicked on circle -> first create new circle */
    if (end == NULL)
    {
        if (wnd->selected_node->type == NPC)
            type = PLAYER;
        else
            type = NPC;

        while (!new_circle (wnd, point, type));
        end = (DlgNode *) get_ptr_list_element (wnd->nodes, wnd->number - 1);
    }

    /* Now,  wnd -> selected_node  contains start-circle and  end  contains end-circle */

    /* Insert arrow into start- and end-circle */
    g_ptr_array_add (wnd->selected_node->next, arrow);
    g_ptr_array_add (end->prev, arrow);

    /* Insert start- & end-circle into arrow */
    g_ptr_array_add (arrow->prev, wnd->selected_node);
    g_ptr_array_add (arrow->next, end);

    /* draw arrow */
    redraw_arrow (wnd, arrow);
    draw_arrow (wnd, arrow, 0);
    show_preview (wnd);
    
    return 1;
}

/* add new link */
int 
new_link (MainFrame * wnd, GdkPoint point)
{
    DlgNode *arrow = get_cur_selection (wnd, point);
    u_int32 i;

    /* exit function, if
       - marked node is no circle 
       - no node has been selected
       - selected node is no arrow
       - selected node is PLAYER and node following link too
       - selected node is already a link */
    if (wnd->selected_node->type == LINK)
        return 0;
    if (arrow == NULL)
        return 0;
    if (arrow->type != LINK)
        return 0;

    if (wnd->selected_node->type == PLAYER &&
        ((DlgNode *) g_ptr_array_index (arrow->next, 0))->type == PLAYER)
        return 0;

    for (i = 0; i < wnd->selected_node->link->len; i++)
        if (arrow == g_ptr_array_index (wnd->selected_node->link, i))
            return 0;

    /* Add arrow to circles links */
    g_ptr_array_add (wnd->selected_node->link, arrow);

    /* Add circle to arrows links */
    g_ptr_array_add (arrow->link, wnd->selected_node);

    /* update graph */
    draw_circle (wnd, wnd->selected_node, 1);
    draw_arrow (wnd, arrow, 2);
    show_preview (wnd);

    return 1;
}

/* remove link */
int 
remove_link (MainFrame * wnd, GdkPoint point)
{
    DlgNode *arrow = get_cur_selection (wnd, point);

    /* exit function, if
       - marked node is no circle 
       - no node has been selected
       - selected node is no arrow
       - selected node is link of circle */
    if (wnd->selected_node->type == LINK)
        return 0;
    if (arrow == NULL)
        return 0;
    if (arrow->type != LINK)
        return 0;
    if (!g_ptr_array_remove (wnd->selected_node->link, arrow))
        return 0;

    /* remove circle from arrows links */
    g_ptr_array_remove (arrow->link, wnd->selected_node);

    /* update graph */
    draw_circle (wnd, wnd->selected_node, 1);
    draw_arrow (wnd, arrow, 0);
    show_preview (wnd);
    
    return 1;
}

/* change nodes text */
void 
edit_node (MainFrame * wnd)
{
    NodeData *cbd = (NodeData *) g_malloc (sizeof (NodeData));

    /* return, if no node is selected */
    if (wnd->selected_node == NULL)
        return;

    /* Init Dialog-Callback-Data */
    cbd->wnd = wnd;
    cbd->node = wnd->selected_node;
    cbd->retval = 0;


    /* circle selected */
    if (wnd->selected_node->type != LINK)
    {
        /* Create and display dialog for user-input */
        wnd->text_dlg = create_text_dialog (cbd);
        gtk_widget_show (wnd->text_dlg);
        gtk_window_set_modal ((GTK_WINDOW (wnd->text_dlg)), TRUE);

        /* Enter Dialog - Event - Loop */
        gtk_main ();

        /* In case  wnd -> selected_node -> type  has changed */
        draw_circle (wnd, wnd->selected_node, wnd->selected_node->highlite);

        /* won´t work without, don´t ask me why! */
        sort_nodes (wnd);
    }

    /* arrow selected */
    else
    {
    }

    show_preview (wnd);
    
    g_free (cbd);
}

/* delete a node */
void 
delete_node (MainFrame * wnd)
{
    /* return if no node selected */
    if (wnd->selected_node == NULL)
        return;

    /* Else
       - if selected_node is arrow, then remove it from start- and end-circle
       amd delete it
       - if its a circle, then remove attached arrows first */
    if (wnd->selected_node->type == LINK)
        delete_arrow (wnd, wnd->selected_node);
    else
    {
        /* delete all links */
        delete_links (wnd, wnd->selected_node);

        /* delete all preceding arrows */
        while (wnd->selected_node->prev->len > 0)
            delete_arrow (wnd, (DlgNode *) g_ptr_array_index (wnd->selected_node->prev, 0));

        /* delete all following arrows */
        while (wnd->selected_node->next->len > 0)
            delete_arrow (wnd, (DlgNode *) g_ptr_array_index (wnd->selected_node->next, 0));

        /* delete circle from nodes */
        remove_ptr_list_pointer (wnd->nodes, wnd->selected_node);
        free_node (wnd -> selected_node);

        /* rearange nodes */
        sort_nodes (wnd);
    }

    /* Set programs state to idle, since nothing is selected now */
    wnd->selected_node = NULL;
    wnd->mode = IDLE;

    redraw_graph (wnd);
    show_preview (wnd);
}

/* remove arrow from start- and end-circle */
void 
delete_arrow (MainFrame * wnd, DlgNode * arrow)
{
    DlgNode *circle;

    /* Links entfernen */
    delete_links (wnd, arrow);

    /* remove arrow from previous circle */
    circle = (DlgNode *) g_ptr_array_index (arrow->prev, 0);
    g_ptr_array_remove (circle->next, arrow);

    /* remove arrow from following circle */
    circle = (DlgNode *) g_ptr_array_index (arrow->next, 0);
    g_ptr_array_remove (circle->prev, arrow);

    /* delete arrow from nodes */
    remove_ptr_list_pointer (wnd->nodes, arrow);

    free_node (arrow);

    /* rearange nodes */
    sort_nodes (wnd);
}

/* removes node from other nodes links */
void 
delete_links (MainFrame * wnd, DlgNode * link)
{
    DlgNode *node;
    u_int32 i;

    /* try to delete node from all other nodes links */
    for (i = 0; i < wnd->number; i++)
    {
        node = (DlgNode *) get_ptr_list_element (wnd->nodes, i);
        g_ptr_array_remove (node->link, link);

        /* unmark former link */
        if (node->type == LINK)
            draw_arrow (wnd, node, 0);
        else
            draw_circle (wnd, node, 0);
    }
}

/* free memory allocated for a dialogue node */
void 
free_node (DlgNode * node)
{
    if (node->text)
        g_free (node->text);

    g_ptr_array_free (node->prev, FALSE);
    g_ptr_array_free (node->next, FALSE);
    g_ptr_array_free (node->link, FALSE);

    g_free (node);
}

/* fills gaps created by removal of nodes */
void 
sort_nodes (MainFrame * wnd)
{
    int i;

    /* set correct index of nodes */
    for (i = 0; i < wnd->nodes->size; i++)
        ((DlgNode *) get_ptr_list_element (wnd->nodes, i))->number = i;

    /* set correct number of nodes */
    wnd->number = i;
}

/* Select node to be dragged */
void 
new_mover (MainFrame * wnd, GdkPoint point)
{
    DlgNode *node;
    GdkRectangle t;

    /* return if no node is selected */
    if (wnd->selected_node == NULL)
        return;

    /* set apps new mode */
    wnd->mode = OBJECT_DRAGGED;

    /* Is dragged node circle or arrow? */
    if (wnd->selected_node->type != LINK)
        wnd->dragged_node = wnd->selected_node;
    else
    {
        wnd->dragged_node = (DlgNode *) malloc (sizeof (DlgNode));
        wnd->dragged_node->type = MOVER;
        wnd->dragged_node->text = NULL;
        wnd->dragged_node->prev = g_ptr_array_new ();
        wnd->dragged_node->next = g_ptr_array_new ();
        wnd->dragged_node->link = g_ptr_array_new ();

        g_ptr_array_add (wnd->dragged_node->next, wnd->selected_node);

        t.x = point.x - 15;
        t.y = point.y - 15;
        t.width = t.height = 30;

        /* Is arrow dragged by its tip? */
        if (point_in_rect (t, wnd->selected_node->line[1]))
        {
            node = (DlgNode *) g_ptr_array_index (wnd->selected_node->next, 0);
            g_ptr_array_add (wnd->dragged_node->next, node);

            wnd->dragged_node->position = node->position;
            wnd->dragged_node->number = 1;

            g_ptr_array_remove_index (wnd->selected_node->next, 0);
            g_ptr_array_add (wnd->selected_node->next, wnd->dragged_node);

            return;
        }

        /* arrow dragged by its tail? */
        if (point_in_rect (t, wnd->selected_node->line[0]))
        {
            node = (DlgNode *) g_ptr_array_index (wnd->selected_node->prev, 0);
            g_ptr_array_add (wnd->dragged_node->next, node);

            wnd->dragged_node->position = node->position;
            wnd->dragged_node->number = 0;

            g_ptr_array_remove_index (wnd->selected_node->prev, 0);
            g_ptr_array_add (wnd->selected_node->prev, wnd->dragged_node);

            return;
        }

        free_node (wnd->dragged_node);

        wnd->dragged_node = NULL;
        wnd->mode = OBJECT_MARKED;
    }

    return;
}

/* move node and attached arrows */
void 
move_node (MainFrame * wnd, GdkPoint point)
{
    u_int32 i;

    /* move node */
    wnd->dragged_node->position.x = point.x;
    wnd->dragged_node->position.y = point.y;

    /* update arrows */
    for (i = 0; i < wnd->dragged_node->prev->len; i++)
        redraw_arrow (wnd, (DlgNode *) g_ptr_array_index (wnd->dragged_node->prev, i));

    for (i = 0; i < wnd->dragged_node->next->len; i++)
        redraw_arrow (wnd, (DlgNode *) g_ptr_array_index (wnd->dragged_node->next, i));

    return;
}

/* dragging ended */
void 
end_moving (MainFrame * wnd, GdkPoint point)
{
    DlgNode *node = get_cur_selection (wnd, point);
    DlgNode *arrow, *circle, *old_circle;

    /* was arrow moved -> then:
       - assure that cursor is above circle
       - assure that arros start and end are different
       - update attachment
       - delete the temporary MOVER */
    if (wnd->dragged_node->type == MOVER)
    {
        /* retrieve the moved arrow */
        arrow = (DlgNode *) g_ptr_array_index (wnd->dragged_node->next, 0);
        old_circle = (DlgNode *) g_ptr_array_index (wnd->dragged_node->next, 1);

        /* if something´s wrong, restore old graph */
        if (node == NULL || node == (DlgNode *) g_ptr_array_index (arrow->next, 0) ||
            node == (DlgNode *) g_ptr_array_index (arrow->prev, 0) || node->type == LINK)
            circle = old_circle;

        /* everything allright */
        else
            circle = node;

        /* arrows tail was moved */
        if (wnd->dragged_node->number == 0)
        {
            /* remove arrow from old circle */
            g_ptr_array_remove (old_circle->next, arrow);

            /* add arrow to new circle */
            g_ptr_array_add (circle->next, arrow);
            g_ptr_array_remove_index (arrow->prev, 0);
            g_ptr_array_add (arrow->prev, circle);
        }

        /* arrows tip was moved */
        else
        {
            /* remove arrow from old circle */
            g_ptr_array_remove (old_circle->prev, arrow);

            /* add arrow to new circle */
            g_ptr_array_add (circle->prev, arrow);
            g_ptr_array_remove_index (arrow->next, 0);
            g_ptr_array_add (arrow->next, circle);
        }

        /* redraw arrow */
        redraw_arrow (wnd, arrow);

        /* delete temporary circle */
        free_node (wnd->dragged_node);
    }

    /* was circle moved, there is not much to do ;-) */
    wnd->dragged_node = NULL;
    wnd->mode = OBJECT_MARKED;

    redraw_graph (wnd);
    show_preview (wnd);
}

/* Shows Preview of Dialogue */
void
show_preview (MainFrame * wnd)
{
    u_int32 i, j;
    GList *tmp_list = NULL;
    DlgNode *circle, *node;

    /* empty list */
    gtk_list_clear_items (GTK_LIST (wnd->list), 0, -1);

    if (wnd->selected_node == NULL)
        return;

    if (wnd->selected_node->type == LINK)
        circle = (DlgNode *) g_ptr_array_index (wnd->selected_node->next, 0);
    else
        circle = wnd->selected_node;
            
    /* Insert prev links */
    for (i = 0; i < circle->prev->len; i++)
    {
        node = (DlgNode *) g_ptr_array_index (circle->prev, i);

        for (j = 0; j < node->link->len; j++)
            tmp_list = g_list_append (tmp_list, create_list_item (wnd, (DlgNode *) g_ptr_array_index (node->link, j), 2));
    }
    
    /* Insert prev nodes */
    for (i = 0; i < circle->prev->len; i++)
    {
        node = (DlgNode *) g_ptr_array_index (circle->prev, i);
        tmp_list = g_list_append (tmp_list, create_list_item (wnd, (DlgNode *) g_ptr_array_index (node->prev, 0), 0));
    }
    
    /* Insert selected node */
    tmp_list = g_list_append (tmp_list, create_list_item (wnd, circle, 1));
    
    /* Insert following nodes */   
    for (i = 0; i < circle->next->len; i++)
    {
        node = (DlgNode *) g_ptr_array_index (circle->next, i);
        tmp_list = g_list_append (tmp_list, create_list_item (wnd, (DlgNode *) g_ptr_array_index (node->next, 0), 0));
    }

    /* Insert following links */
    for (i = 0; i < circle->link->len; i++)
    {
        node = (DlgNode *) g_ptr_array_index (circle->link, i);
        tmp_list = g_list_append (tmp_list, create_list_item (wnd, (DlgNode *) g_ptr_array_index (node->next, 0), 2));
    }

    gtk_widget_hide (wnd->list);
    gtk_list_append_items (GTK_LIST (wnd->list), tmp_list); 
    gtk_widget_show (wnd->list);
}

/* Draws a Circle */
void 
draw_circle (MainFrame * wnd, DlgNode * circle, int highlite)
{
    GdkGC *gc;
    GString *links = g_string_new ("");
    int x, y;

    /* assure that we only redraw circles */
    if (circle->type == LINK)
        return;

    /* select color for drawing circle */
    switch (highlite)
    {
    case 0:                    /* not selected */
        {
            if (circle->type == NPC)
                gc = wnd->graph->style->black_gc;
            else
                gc = wnd->color[GC_DARK_BLUE];

            circle->highlite = 0;
            break;
        }

    case 1:                    /* selected */
        {
            if (circle->type == NPC)
                gc = wnd->color[GC_DARK_RED];
            else
                gc = wnd->color[GC_RED];

            circle->highlite = 1;
            break;
        }

    case 2:                    /* Has link */
        {
            gc = wnd->color[GC_PURPLE];

            circle->highlite = 2;
            break;
        }

    case 3:                    /* mouse over */
        {
            gc = wnd->color[GC_GREEN];
            break;
        }

    default:
        return;
    }

    circle->position.x += wnd->x_offset;
    circle->position.y += wnd->y_offset;

    /* Insert number of links */
    g_string_sprintf (links, "%i", circle->link->len);

    /* place text in circles center */
    x = circle->position.x + (20 - gdk_string_width (wnd->font, links->str)) / 2;
    y = circle->position.y + (21 + gdk_string_height (wnd->font, links->str)) / 2;

    /* draw everything */
    gdk_draw_arc (wnd->pixmap, wnd->graph->style->white_gc, TRUE, circle->position.x, circle->position.y, 20, 20, 0, 36000);
    gdk_draw_arc (wnd->pixmap, gc, FALSE, circle->position.x, circle->position.y, 20, 20, 0, 36000);
    gdk_draw_string (wnd->pixmap, wnd->font, gc, x, y, links->str);
    gtk_widget_draw (wnd->graph, &circle->position);

    circle->position.x -= wnd->x_offset;
    circle->position.y -= wnd->y_offset;

    g_string_free (links, TRUE);
}

/* Init arrow-shape */
void 
redraw_arrow (MainFrame * wnd, DlgNode * arrow)
{
    DlgNode *node;
    GdkPoint start, end;
    float a, b, x, y;

    /* assure that we only redraw arrows */
    if (arrow->type != LINK)
        return;

    /* start- and end-point of arrow */
    node = (DlgNode *) g_ptr_array_index (arrow->prev, 0);
    start = center_point (node->position);

    node = (DlgNode *) g_ptr_array_index (arrow->next, 0);
    end = center_point (node->position);

    /* gradient of line */
    x = start.x - end.x;
    y = start.y - end.y;

    if ((x < 0 ? -x : x) > (y < 0 ? -y : y))
    {
        a = x < 0 ? -1.0f : 1.0f;
        b = (float)y / (x < 0 ? (float)-x : (float)x);
    }
    else
    {
        a = (float)x / (y < 0 ? (float)-y : (float)y);
        b = y < 0 ? -1.0f : 1.0f;
    }

    /* line from start- to end-circle */
    arrow->line[0] = offset_point (start, (int)(-10.0f * a), (int)(-10.0f * b));
    arrow->line[1] = offset_point (end, (int)(10.0f * a), (int)(10.0f * b));

    /* arrows tip */
    arrow->tip[0] = arrow->line[1];
    arrow->tip[1] = offset_point (arrow->line[1], (int)(a * 10.0f + b * 5.0f), (int)(b * 10.0f - a * 5.0f));
    arrow->tip[2] = offset_point (arrow->line[1], (int)(a * 10.0f - b * 5.0f), (int)(b * 10.0f + a * 5.0f));

    /* calculate arrows new position */
    arrow->position = create_rectangle (arrow->line[0], arrow->line[1]);
    arrow->position = inflate_rectangle (arrow->position, 2, 2);
}

/* Draws an arrow */
void 
draw_arrow (MainFrame * wnd, DlgNode * arrow, int highlite)
{
    GdkGC *gc;
    GdkPoint line[2];
    GdkPoint tip[3];
    GdkRectangle rect = inflate_rectangle (arrow->position, 10, 10);

    /* assure that we only redraw arrows */
    if (arrow->type != LINK)
        return;

    /* select color for drawing arrow */
    switch (highlite)
    {
    case 0:                    /* not selected */
        {
            gc = wnd->graph->style->black_gc;
            arrow->highlite = 0;
            break;
        }

    case 1:                    /* selected */
        {
            gc = wnd->color[GC_DARK_RED];
            arrow->highlite = 1;
            break;
        }

    case 2:                    /* link */
        {
            gc = wnd->color[GC_PURPLE];
            arrow->highlite = 2;
            break;
        }

    case 3:                    /* Mouse over */
        {
            gc = wnd->color[GC_GREEN];
            break;
        }

    default:
        return;
    }
    
    /* Current position */
    line[0] = offset_point (arrow->line[0], wnd->x_offset, wnd->y_offset);
    line[1] = offset_point (arrow->line[1], wnd->x_offset, wnd->y_offset);
    tip[0] = offset_point (arrow->tip[0], wnd->x_offset, wnd->y_offset);
    tip[1] = offset_point (arrow->tip[1], wnd->x_offset, wnd->y_offset);
    tip[2] = offset_point (arrow->tip[2], wnd->x_offset, wnd->y_offset);

    rect.x += wnd->x_offset;
    rect.y += wnd->y_offset;
    
    /* draw everything */
    gdk_draw_polygon (wnd->pixmap, gc, FALSE, line, 2);
    gdk_draw_polygon (wnd->pixmap, gc, FALSE, tip, 3);
    gtk_widget_draw (wnd->graph, &rect);
}

/* redraw graph */
void 
redraw_graph (MainFrame * wnd)
{
    u_int32 i;
    DlgNode *node;
    GdkRectangle t;

    /* get visible part of graph */
    t.x = -wnd->x_offset;
    t.y = -wnd->y_offset;
    t.width = wnd->graph->allocation.width;
    t.height = wnd->graph->allocation.height;

    /* Clear graph */
    gdk_draw_rectangle (wnd->pixmap, wnd->graph->style->white_gc, TRUE, 0, 0, t.width, t.height);

    /* check for each node, wether it is visible */
    for (i = 0; i < wnd->number; i++)
    {
        node = (DlgNode *) get_ptr_list_element (wnd->nodes, i);

        if (point_in_rect (t, center_point (node->position)))
        {
            if (node->type == LINK)
                draw_arrow (wnd, node, node->highlite);
            else
                draw_circle (wnd, node, node->highlite);
        }
    }

    /* normalize rect */
    t.x = 0;
    t.y = 0;

    /* draw backing image to screen */
    gtk_widget_draw (wnd->graph, &t);

    /* Mark object below cursor if neccessary */
    // if (wnd->mode != OBJECT_DRAGGED)
    // {
    //     wnd->below_pointer = NULL;
    //     mouse_over (wnd, point);
    // }
}

/* create new dialogue */
void
new_dialogue (MainFrame * wnd)
{
    /* first, clear current dialogue and set app back ... */
    delete_dialogue (wnd);
    init_app (wnd);

    /* ... then update display */
    gtk_window_set_title (GTK_WINDOW (wnd->wnd), "Adonthell Dialogue Editor v0.2 - [new_dialogue]");
    redraw_graph (wnd);
    show_preview (wnd);
}

/* load a dialogue from disk */
void 
load_dialogue (MainFrame * wnd)
{
    int i, type;
    gchar str[31];
    GString *file = g_string_new (NULL);
    FILE *in;
    GtkWidget *fs = create_fileselection (file, 1);

    gtk_file_selection_set_filename ((GtkFileSelection *) fs, wnd->file_name);

    /* chose file */
    gtk_widget_show (fs);
    gtk_main ();

    /* Was load cancelled? */
    if (file->str == NULL)
    {
        g_string_free (file, TRUE);
        return;
    }

    /* open File */
    in = fopen (file->str, "rb");

    if (!in)
    {
        g_message ("could not open file %s", file->str);
        g_string_free (file, TRUE);
        return;
    }

    /* Check if its a correct file */
    fgets (str, 31, in);
    fseek (in, 81, SEEK_CUR);

    if (g_strcasecmp ("Adonthell Dialogue System v0.1", str))
    {
        g_message ("%s is no valid dialogue file", file->str);
        g_string_free (file, TRUE);
        return;
    }

    /* first, clear current dialogue and set app back ... */
    delete_dialogue (wnd);
    init_app (wnd);

    /* ... then load all nodes ... */
    wnd->number = h2d (4, in);

    for (i = 0; i < (int) wnd->number; i++)
    {
        type = h2d (4, in);

        if (type != LINK)
            load_circle (wnd, in, type);
        else
            load_arrow (wnd, in);
    }

    /* ... and redraw them */
    redraw_graph (wnd);

    /* set new window - title */
    wnd->file_name = g_strdup (file->str);
    gtk_window_set_title (GTK_WINDOW (wnd->wnd), g_strjoin (NULL, "Adonthell Dialogue Editor v0.2 - [", strrchr (wnd->file_name, '/') + 1, "]", NULL));

    /* look wether compiled dlg exists and set Run - Menuitem accordingly
     * (Currently we simply set it to FALSE, since that is faster (to code)) */
    gtk_widget_set_sensitive (wnd->dialogue_run, FALSE);

    /* clean up */
    g_string_free (file, TRUE);
    fclose (in);
}

/* load circle from a file */
void 
load_circle (MainFrame * wnd, FILE * in, int type)
{
    DlgNode *circle = (DlgNode *) malloc (sizeof (DlgNode));

    /* init arrow */
    circle->prev = g_ptr_array_new ();
    circle->next = g_ptr_array_new ();
    circle->link = g_ptr_array_new ();
    circle->highlite = 0;

    /* assign type and load number */
    circle->type = type;
    circle->number = h2d (4, in);

    /* load position */
    circle->position.x = h2d (4, in);
    circle->position.y = h2d (4, in);
    circle->position.height = h2d (4, in);
    circle->position.width = h2d (4, in);

    /* load text */
    circle->text = rs (4, in);

    /* add to array of nodes */
    add_ptr_list_element (wnd->nodes, circle);
}

/* load an arrow from file */
void 
load_arrow (MainFrame * wnd, FILE * in)
{
    int num, i;
    DlgNode *arrow = (DlgNode *) malloc (sizeof (DlgNode));
    DlgNode *circle;

    /* init arrow */
    arrow->prev = g_ptr_array_new ();
    arrow->next = g_ptr_array_new ();
    arrow->link = g_ptr_array_new ();
    arrow->highlite = 0;

    /* assign type and load number */
    arrow->type = LINK;
    arrow->number = h2d (4, in);

    /* load and assign start-circle */
    circle = (DlgNode *) get_ptr_list_element (wnd->nodes, h2d (4, in));

    g_ptr_array_add (circle->next, arrow);
    g_ptr_array_add (arrow->prev, circle);

    /* load and assign end-circle */
    circle = (DlgNode *) get_ptr_list_element (wnd->nodes, h2d (4, in));

    g_ptr_array_add (circle->prev, arrow);
    g_ptr_array_add (arrow->next, circle);

    /* load text */
    arrow->text = rs (4, in);

    /* load  links */
    num = h2d (4, in);
    for (i = 0; i < num; i++)
    {
        circle = (DlgNode *) get_ptr_list_element (wnd->nodes, h2d (4, in));

        g_ptr_array_add (circle->link, arrow);
        g_ptr_array_add (arrow->link, circle);
    }

    /* recalculate arrows shape */
    redraw_arrow (wnd, arrow);

    /* Add to Array of nodes */
    add_ptr_list_element (wnd->nodes, arrow);
}

/* save a dialogue to disk */
void 
save_dialogue (MainFrame * wnd)
{
    u_int32 i;
    int index = 0;
    int *table = (int *)malloc (sizeof (int) * wnd->number);
    GString *file = g_string_new (NULL);
    FILE *out;
    GtkWidget *fs = create_fileselection (file, 0);

    gtk_file_selection_set_filename ((GtkFileSelection *) fs, wnd->file_name);

    /* chose file */
    gtk_widget_show (fs);
    gtk_main ();

    /* Was save cancelled? */
    if (file->str == NULL)
    {
        g_string_free (file, TRUE);
        return;
    }

    /* open File */
    out = fopen (file->str, "wb");

    if (!out)
    {
        g_message ("could not open file %s", file->str);
        g_string_free (file, TRUE);
        return;
    }

    /* Be on the safe side */
    sort_nodes (wnd);

    /* Write Header */
    fprintf (out, "Adonthell Dialogue System v0.1 Editor File. (c) 1999 by Kai Sterker. Belongs to the Adonthell project.");
    for (i = 0; i < 9; i++)
        fputc ('\0', out);

    /* Number of nodes */
    d2h (wnd->number, 4, out);

    /* Save Circles and create position-table */
    for (i = 0; i < wnd->number; i++)
        if (((DlgNode *) get_ptr_list_element (wnd->nodes, i))->type != LINK)
        {
            save_circle ((DlgNode *) get_ptr_list_element (wnd->nodes, i), out, index);
            table[i] = index;
            index++;
        }

    /* Save Arrows */
    for (i = 0; i < wnd->number; i++)
        if (((DlgNode *) get_ptr_list_element (wnd->nodes, i))->type == LINK)
        {
            save_arrow ((DlgNode *) get_ptr_list_element (wnd->nodes, i), out, index, table);
            index++;
        }

    /* set new window - title */
    wnd->file_name = g_strdup (file->str);
    gtk_window_set_title (GTK_WINDOW (wnd->wnd), g_strjoin (NULL, "Adonthell Dialogue Editor v0.2 - [", strrchr (wnd->file_name, '/') + 1, "]", NULL));

    /* clean up */
    g_string_free (file, TRUE);
    free (table);

    fclose (out);
}

void 
save_circle (DlgNode * circle, FILE * out, int number)
{
    /* save type and number */
    d2h (circle->type, 4, out);
    d2h (number, 4, out);

    /* save position */
    d2h (circle->position.x, 4, out);
    d2h (circle->position.y, 4, out);
    d2h (circle->position.height, 4, out);
    d2h (circle->position.width, 4, out);

    /* save text */
    if (circle->text != NULL)
    {
        d2h (strlen (circle->text) + 1, 4, out);
        fprintf (out, circle->text);
    }
    else
        d2h (0, 4, out);
}

void 
save_arrow (DlgNode * arrow, FILE * out, int number, int *table)
{
    u_int32 i;

    /* save type and number */
    d2h (arrow->type, 4, out);
    d2h (number, 4, out);

    /* start- and end-circle */
    d2h (table[((DlgNode *) g_ptr_array_index (arrow->prev, 0))->number], 4, out);
    d2h (table[((DlgNode *) g_ptr_array_index (arrow->next, 0))->number], 4, out);

    /* save text */
    if (arrow->text != NULL)
    {
        d2h (strlen (arrow->text) + 1, 4, out);
        fprintf (out, arrow->text);
    }
    else
        d2h (0, 4, out);

    /* links */
    d2h (arrow->link->len, 4, out);
    for (i = 0; i < arrow->link->len; i++)
        d2h (table[((DlgNode *) g_ptr_array_index (arrow->link, i))->number], 4, out);
}

void 
mouse_over (MainFrame * wnd, GdkPoint point)
{
    DlgNode *node = get_cur_selection (wnd, point);

    /* Exit function if  
       - node below pointer is unchanged
       - no node below cursor */
    if (node == wnd->below_pointer)
        return;
    if (node == NULL)
    {
        if (wnd->below_pointer->type == LINK)
            draw_arrow (wnd, wnd->below_pointer, wnd->below_pointer->highlite);
        else
            draw_circle (wnd, wnd->below_pointer, wnd->below_pointer->highlite);

        gtk_widget_draw (wnd->graph, &wnd->below_pointer->position);
        wnd->below_pointer = NULL;

        if (wnd->tooltip != NULL)
        {
            gtk_widget_destroy (wnd->tooltip);
            wnd->tooltip = NULL;
        }

        return;
    }

    /* remove highlite */
    if (wnd->below_pointer != NULL)
    {
        if (wnd->below_pointer->type == LINK)
            draw_arrow (wnd, wnd->below_pointer, wnd->below_pointer->highlite);
        else
            draw_circle (wnd, wnd->below_pointer, wnd->below_pointer->highlite);

        if (wnd->tooltip != NULL)
        {
            gtk_widget_destroy (wnd->tooltip);
            wnd->tooltip = NULL;
        }

        gtk_widget_draw (wnd->graph, &wnd->below_pointer->position);
    }

    wnd->below_pointer = node;

    /* Draw border and objects text */
    show_sel (wnd, node);
    show_tooltip (wnd, node);
}

/* Select a Node */
int 
select_object (MainFrame * wnd, GdkPoint point)
{
    DlgNode *node = get_cur_selection (wnd, point);
    u_int32 i;

    /* No object selected */
    if (node == NULL)
        return 0;

    /* set selected object */
    wnd->selected_node = node;

    /* highlite node and its links */
    switch (node->type)
    {
    case NPC:
    case PLAYER:
        {
            draw_circle (wnd, node, 1);

            for (i = 0; i < wnd->selected_node->link->len; i++)
                draw_arrow (wnd, (DlgNode *) g_ptr_array_index (wnd->selected_node->link, i), 2);

            break;
        }

    case LINK:
        {
            draw_arrow (wnd, node, 1);

            for (i = 0; i < wnd->selected_node->link->len; i++)
                draw_circle (wnd, (DlgNode *) g_ptr_array_index (wnd->selected_node->link, i), 2);

            break;
        }
    }

    /* Set new Program-Mode */
    wnd->mode = OBJECT_MARKED;

    show_preview (wnd);
    
    return 1;
}

/* Select a Node */
void 
select_object_index (MainFrame * wnd, int index)
{
    u_int32 i;

    /* set selected object */
    wnd->selected_node = (DlgNode *) get_ptr_list_element (wnd->nodes, index);

    /* highlite node and its links */
    switch (wnd->selected_node->type)
    {
    case NPC:
    case PLAYER:
        {
            draw_circle (wnd, wnd->selected_node, 1);

            for (i = 0; i < wnd->selected_node->link->len; i++)
                draw_arrow (wnd, (DlgNode *) g_ptr_array_index (wnd->selected_node->link, i), 2);

            break;
        }

    case LINK:
        {
            draw_arrow (wnd, wnd->selected_node, 1);

            for (i = 0; i < wnd->selected_node->link->len; i++)
                draw_circle (wnd, (DlgNode *) g_ptr_array_index (wnd->selected_node->link, i), 2);

            break;
        }
    }

    /* Set new Program-Mode */
    wnd->mode = OBJECT_MARKED;

    show_preview (wnd);
}

/* Deselect a Node */
int 
deselect_object (MainFrame * wnd)
{
    u_int32 i;

    /* No object selected */
    if (wnd->selected_node == NULL)
        return 0;

    /* unhighlite node and its links */
    switch (wnd->selected_node->type)
    {
    case NPC:
    case PLAYER:
        {
            draw_circle (wnd, wnd->selected_node, 0);

            for (i = 0; i < wnd->selected_node->link->len; i++)
                draw_arrow (wnd, (DlgNode *) g_ptr_array_index (wnd->selected_node->link, i), 0);

            break;
        }

    case LINK:
        {
            draw_arrow (wnd, wnd->selected_node, 0);

            for (i = 0; i < wnd->selected_node->link->len; i++)
                draw_circle (wnd, (DlgNode *) g_ptr_array_index (wnd->selected_node->link, i), 0);

            break;
        }
    }

    /* set selected object */
    wnd->selected_node = NULL;

    /* Set new Program-Mode */
    wnd->mode = IDLE;

    show_preview (wnd);

    return 1;
}

void 
show_tooltip (MainFrame * wnd, DlgNode * node)
{
    int x, y;

    if (wnd->tooltip == NULL)
    {
        gdk_window_get_origin (gtk_widget_get_parent_window (wnd->graph), &x, &y);

        x = x + node->position.x + node->position.width + wnd->x_offset;
        y = y + node->position.y + node->position.height + wnd->y_offset;

        wnd->tooltip = create_tooltip (node->text, x, y);
        gtk_widget_show (wnd->tooltip);
    }
}

/* Highlight node below cursor */
void 
show_sel (MainFrame * wnd, DlgNode * node)
{
    if (node->type == LINK)
        draw_arrow (wnd, node, 3);
    else
        draw_circle (wnd, node, 3);

    gtk_widget_draw (wnd->graph, &wnd->below_pointer->position);
}

/* Check if a node is below cursor */
DlgNode *
get_cur_selection (MainFrame * wnd, GdkPoint point)
{
    int i, x, dx, dy;
    DlgNode *node;
    GdkRectangle rect;

    /* Mousepointer over node? */
    for (i = 0; i < (int) wnd->number; i++)
    {
        node = (DlgNode *) get_ptr_list_element (wnd->nodes, i);
        rect = node->position;

        if (point_in_rect (rect, point))
        {
            /* node == circle? -> return */
            if (node->type != LINK)
                return node;

            /* node == arrow? -> just a bit geometry ;-) */
            /* straight g: ax + by + c = 0 through 2 points p(x, y) und q(x, y): */
            /* with a = (p.y - q.y), b = (q.x - p.x), c = (-(p.x * a + p.y * b)) */

            /* insert point in above function; values |x| < 500 are close to the arrow */
            else
            {
                dx = node->line[1].x - node->line[0].x;
                dy = node->line[0].y - node->line[1].y;

                x = (dy * point.x + dx * point.y) - (node->line[0].x * dy + node->line[0].y * dx);

                if (x < 500 && x > -500)
                    return node;
            }
        }
    }

    return NULL;
}

/* center the view on node */
void
center_object (MainFrame * wnd, DlgNode * node)
{
    GdkRectangle view;
    GdkPoint pos = offset_point (center_point (node->position), wnd->x_offset, wnd->y_offset);
    int x, y; 

    view.x = 0;
    view.y = 0;
    view.width = wnd->graph->allocation.width;
    view.height = wnd->graph->allocation.height;
    
    x = -view.width / 5;
    y = -view.height / 5;

    /* is node outside the views inner 60% ? */
    if (!point_in_rect (inflate_rectangle (view, x, y), pos))
    {
        wnd->x_offset -= pos.x - wnd->graph->allocation.width / 2;
        wnd->y_offset -= pos.y - wnd->graph->allocation.height / 2;

        redraw_graph (wnd);
    }
}
