/*
   $Id$ 

   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

class dialog;

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <gtk/gtk.h>
#include <vector>
#include <time.h>

#include "../../types.h"
#include "../../character.h"
#include "dlgnode.h"
#include "main.h"
#include "geometrie.h"
#include "compile.h"
#include "interface.h"
#include "circle.h"
#include "graph.h"

// Create a new circle
int 
new_circle (MainFrame * wnd, GdkPoint point, int type)
{
    Circle *circle = new Circle (wnd->number, type, point.x, point.y);

    // disable autoscrolling before the modal dialog pops up
    wnd->scroll = 0;
    
    // Create Dialog to edit Circle's attributes ...
    crcle_dlg dlg (circle, wnd);

    gtk_window_set_transient_for (GTK_WINDOW (dlg.dlg), GTK_WINDOW (wnd->wnd));

    // ... and enter it's event loop
    gtk_main ();

    // Circle created
    if (dlg.retval == 1)
    {
        // One Node more
        wnd->number++;

        // Add to Array
        wnd->nodes.push_back (circle);

        // Display Node
        draw_circle (wnd, circle, 0);
    }
    else
    {
        delete circle;
        return 0;
    }
    
    return 1;
}

// create new arrow
int 
new_arrow (MainFrame * wnd, GdkPoint point)
{
    Arrow *arrow;
    DlgNode *end = get_cur_selection (wnd, point);
    int type;
    u_int32 i;

    // Exit function if  
    //   - no node is marked
    //   - the marked node is no circle
    //   - the clicked node is no circle
    //   - the clicked node is the marked node 
    //   - a connection already existe 
    if (wnd->selected_node == NULL)
        return 0;
    if (wnd->selected_node->type == LINK)
        return 0;
    if (end != NULL && end->type == LINK)
        return 0;
    if (wnd->selected_node == end)
        return 0;
    if (end != NULL)
        for (i = 0; i < wnd->selected_node->next.size (); i++)
            if (end == wnd->selected_node->next[i]->next[0])
                return 0;

    arrow = new Arrow (wnd->number, LINK);

    // Add to Array 
    wnd->nodes.push_back (arrow);

    wnd->number++;

    // Not clicked on circle -> first create new circle 
    if (end == NULL)
    {
        if (wnd->selected_node->type == NPC)
            type = PLAYER;
        else
            type = NPC;

        while (!new_circle (wnd, point, type));
        end = wnd->nodes[wnd->number - 1];
    }

    // Now,  wnd -> selected_node  contains start-circle and  end  contains end-circle 

    // Insert arrow into start- and end-circle 
    wnd->selected_node->next.push_back (arrow);
    end->prev.push_back (arrow);

    // Insert start- & end-circle into arrow 
    arrow->prev.push_back (wnd->selected_node);
    arrow->next.push_back (end);

    // draw arrow 
    redraw_arrow (wnd, arrow);
    draw_arrow (wnd, arrow, 0);
    show_preview (wnd);
    
    return 1;
}

// add new link 
int 
new_link (MainFrame * wnd, GdkPoint point)
{
    DlgNode *arrow = get_cur_selection (wnd, point);
    u_int32 i;

    // exit function, if
    //   - marked node is no circle 
    //   - no node has been selected
    //   - selected node is no arrow
    //   - selected node is PLAYER and node following link too
    //   - selected node is already a link 
    if (wnd->selected_node->type == LINK)
        return 0;
    if (arrow == NULL)
        return 0;
    if (arrow->type != LINK)
        return 0;

    if (wnd->selected_node->type == PLAYER && arrow->next[0]->type == PLAYER)
        return 0;

    for (i = 0; i < wnd->selected_node->link.size (); i++)
        if (arrow == wnd->selected_node->link[i])
            return 0;

    // Add arrow to circles links 
    wnd->selected_node->link.push_back (arrow);

    // Add circle to arrows links 
    arrow->link.push_back (wnd->selected_node);

    // update graph 
    draw_circle (wnd, wnd->selected_node, 1);
    draw_arrow (wnd, (Arrow *) arrow, 2);
    show_preview (wnd);

    return 1;
}

// remove link 
int 
remove_link (MainFrame * wnd, GdkPoint point)
{
    DlgNode *arrow = get_cur_selection (wnd, point);

    // exit function, if
    //   - marked node is no circle 
    //   - no node has been selected
    //   - selected node is no arrow
    //   - selected node is link of circle 
    if (wnd->selected_node->type == LINK)
        return 0;
    if (arrow == NULL)
        return 0;
    if (arrow->type != LINK)
        return 0;
    if (!remove_data (wnd->selected_node->link, arrow))
        return 0;

    // remove circle from arrows links 
    remove_data (arrow->link, wnd->selected_node);

    // update graph 
    draw_circle (wnd, wnd->selected_node, 1);
    draw_arrow (wnd, (Arrow *) arrow, 0);
    show_preview (wnd);
    
    return 1;
}

// change nodes text 
void 
edit_node (MainFrame * wnd)
{
    // return, if no node is selected 
    if (wnd->selected_node == NULL)
        return;

    // circle selected 
    if (wnd->selected_node->type != LINK)
    {
        // disable autoscrolling before the modal dialog pops up
        wnd->scroll = 0;

        // Close Tooltip
        if (wnd->tooltip != NULL)
        {
            gtk_widget_destroy (wnd->tooltip);
            wnd->tooltip = NULL;
        }
        
        // Create and display dialog for user-input 
        crcle_dlg dlg ((Circle *) wnd->selected_node, wnd);

        // Enter Dialog - Event - Loop 
        gtk_main ();

        // In case  wnd -> selected_node -> type  has changed
        draw_circle (wnd, wnd->selected_node, wnd->selected_node->highlite);
    }

    show_preview (wnd);
}

// delete a node 
void 
delete_node (MainFrame * wnd)
{
    // return if no node selected 
    if (wnd->selected_node == NULL)
        return;

    // Else
    //   - if selected_node is arrow, then remove it from start- and end-circle
    //     and delete it
    //   - if its a circle, then remove attached arrows first 
    if (wnd->selected_node->type == LINK)
        delete_arrow (wnd, wnd->selected_node);
    else
    {
        // delete all links 
        delete_links (wnd, wnd->selected_node);

        // delete all preceding arrows 
        while (wnd->selected_node->prev.size () > 0)
            delete_arrow (wnd, wnd->selected_node->prev[0]);

        // delete all following arrows 
        while (wnd->selected_node->next.size () > 0)
            delete_arrow (wnd, wnd->selected_node->next[0]);

        // delete circle from nodes 
        remove_data (wnd->nodes, wnd->selected_node);
        delete wnd->selected_node;

        // rearange nodes 
        sort_nodes (wnd);
    }

    // Set programs state to idle, since nothing is selected now 
    wnd->selected_node = NULL;
    wnd->mode = IDLE;

    redraw_graph (wnd);
    show_preview (wnd);
}

// remove arrow from start- and end-circle 
void 
delete_arrow (MainFrame * wnd, DlgNode * arrow)
{
    DlgNode *circle;

    // Links entfernen 
    delete_links (wnd, arrow);

    // remove arrow from previous circle 
    circle = arrow->prev[0];
    remove_data (circle->next, arrow);

    // remove arrow from following circle 
    circle = arrow->next[0];
    remove_data (circle->prev, arrow);

    // delete arrow from nodes 
    remove_data (wnd->nodes, arrow);

    delete arrow;

    // rearange nodes 
    sort_nodes (wnd);
}

// removes node from other nodes links 
void 
delete_links (MainFrame * wnd, DlgNode * link)
{
    DlgNode *node;
    u_int32 i;

    // try to delete node from all other nodes links 
    for (i = 0; i < wnd->number; i++)
    {
        node = wnd->nodes[i];
        remove_data (node->link, link);

        // unmark former link 
        if (node->type == LINK)
            draw_arrow (wnd, (Arrow *) node, 0);
        else
            draw_circle (wnd, node, 0);
    }
}

// fills gaps created by removal of nodes 
void 
sort_nodes (MainFrame * wnd)
{
    u_int32 i;

    // set correct index of nodes 
    for (i = 0; i < wnd->nodes.size (); i++)
        wnd->nodes[i]->number = i;

    // set correct number of nodes 
    wnd->number = i;
}

// Select node to be dragged 
int 
new_mover (MainFrame * wnd, GdkPoint point)
{
    DlgNode *node = get_cur_selection (wnd, point);
    GdkRectangle t;

    // No node selected at all
    if (node == NULL) 
        return 0;

    // No dragging when multiple nodes are selected
    if (wnd->mode == MULTI_SELECT || wnd->mode == MULTI_MARKED)
        return 0;

    // if no node selected, select node for dragging        
    if (wnd->selected_node == NULL)
        select_object_index (wnd, node->number);
        
    // else check wether dragged and selected node are the same
    else if (node != wnd->selected_node)
        return 0;
    
    // set app's new mode 
    wnd->mode = OBJECT_DRAGGED;

    // Is dragged node circle or arrow? 
    if (wnd->selected_node->type != LINK)
    {
        wnd->dragged_node = wnd->selected_node;
        return 1;
    }
    else
    {
        wnd->dragged_node = new DlgNode;
        wnd->dragged_node->type = MOVER;

        wnd->dragged_node->next.push_back (wnd->selected_node);

        t.x = point.x - 15;
        t.y = point.y - 15;
        t.width = t.height = 30;

        // Is arrow dragged by its tip? 
        if (point_in_rect (t, ((Arrow *) wnd->selected_node)->line[1]))
        {
            node = wnd->selected_node->next[0];
            wnd->dragged_node->next.push_back (node);

            wnd->dragged_node->position = node->position;
            wnd->dragged_node->number = 1;

            wnd->selected_node->next.pop_back ();
            wnd->selected_node->next.push_back (wnd->dragged_node);

            return 1;
        }

        // arrow dragged by its tail? 
        if (point_in_rect (t, ((Arrow *) wnd->selected_node)->line[0]))
        {
            node = wnd->selected_node->prev [0];
            wnd->dragged_node->next.push_back (node);

            wnd->dragged_node->position = node->position;
            wnd->dragged_node->number = 0;

            wnd->selected_node->prev.pop_back ();
            wnd->selected_node->prev.push_back (wnd->dragged_node);

            return 1;
        }

        delete wnd->dragged_node;

        wnd->dragged_node = NULL;
        wnd->mode = OBJECT_MARKED;
    }

    return 0;
}

// move node and attached arrows 
void 
move_node (MainFrame * wnd, GdkPoint point)
{
    u_int32 i;

    // move node 
    wnd->dragged_node->position.x = point.x;
    wnd->dragged_node->position.y = point.y;

    // update arrows 
    for (i = 0; i < wnd->dragged_node->prev.size (); i++)
        redraw_arrow (wnd, (Arrow *) wnd->dragged_node->prev[i]);

    for (i = 0; i < wnd->dragged_node->next.size (); i++)
        redraw_arrow (wnd, (Arrow *) wnd->dragged_node->next[i]);

    return;
}

// dragging ended 
void 
end_moving (MainFrame * wnd, GdkPoint point)
{
    DlgNode *node = get_cur_selection (wnd, point);
    DlgNode *arrow, *circle, *old_circle;

    // was arrow moved -> then:
    //   - assure that cursor is above circle
    //   - assure that arros start and end are different
    //   - update attachment
    //   - delete the temporary MOVER 
    if (wnd->dragged_node->type == MOVER)
    {
        // retrieve the moved arrow 
        arrow = wnd->dragged_node->next[0];
        old_circle = wnd->dragged_node->next[1];

        // if something´s wrong, restore old graph 
        if (node == NULL || node == arrow->next[0] || node == arrow->prev[0] ||
            node->type == LINK)
            circle = old_circle;

        // everything allright 
        else
            circle = node;

        // arrows tail was moved 
        if (wnd->dragged_node->number == 0)
        {
            // remove arrow from old circle 
            remove_data (old_circle->next, arrow);

            // add arrow to new circle 
            circle->next.push_back (arrow);
            arrow->prev.erase (arrow->prev.begin ());
            arrow->prev.push_back (circle);
        }

        // arrows tip was moved 
        else
        {
            // remove arrow from old circle 
            remove_data (old_circle->prev, arrow);

            // add arrow to new circle 
            circle->prev.push_back (arrow);
            arrow->next.erase (arrow->next.begin ());
            arrow->next.push_back (circle);
        }

        // redraw arrow 
        redraw_arrow (wnd, (Arrow *) arrow);

        // delete temporary circle 
        delete wnd->dragged_node;
    }

    // was circle moved, there is not much to do ;-) 
    wnd->dragged_node = NULL;
    wnd->mode = OBJECT_MARKED;

    redraw_graph (wnd);
    show_preview (wnd);
}

// Shows Preview of Dialogue 
void
show_preview (MainFrame * wnd)
{
    u_int32 i, j;
    GList *tmp_list = NULL;
    DlgNode *circle, *node;

    // empty list 
    gtk_list_clear_items (GTK_LIST (wnd->list), 0, -1);

    if (wnd->selected_node == NULL)
        return;

    if (wnd->selected_node->type == LINK)
        circle = wnd->selected_node->next[0];
    else
        circle = wnd->selected_node;
            
    // Insert prev links 
    for (i = 0; i < circle->prev.size (); i++)
    {
        node = circle->prev[i];

        for (j = 0; j < node->link.size (); j++)
            tmp_list = g_list_append (tmp_list, create_list_item (wnd, node->link[j], 2));
    }
    
    // Insert prev nodes 
    for (i = 0; i < circle->prev.size (); i++)
    {
        node = circle->prev[i];
        tmp_list = g_list_append (tmp_list, create_list_item (wnd, node->prev[0], 0));
    }
    
    // Insert selected node 
    tmp_list = g_list_append (tmp_list, create_list_item (wnd, circle, 1));
    
    // Insert following nodes    
    for (i = 0; i < circle->next.size (); i++)
    {
        node = circle->next[i];
        tmp_list = g_list_append (tmp_list, create_list_item (wnd, node->next[0], 0));
    }

    // Insert following links 
    for (i = 0; i < circle->link.size (); i++)
    {
        node = circle->link[i];
        tmp_list = g_list_append (tmp_list, create_list_item (wnd, node->next[0], 2));
    }

    gtk_widget_hide (wnd->list);
    gtk_list_append_items (GTK_LIST (wnd->list), tmp_list); 
    gtk_widget_show (wnd->list);
}

// Draws a Circle 
void 
draw_circle (MainFrame * wnd, DlgNode * circle, int highlite)
{
    GdkGC *gc;
    GString *links = g_string_new ("");
    int x, y;

    // assure that we only redraw circles 
    if (circle->type == LINK)
        return;

    // select color for drawing circle 
    switch (highlite)
    {
    case 0:                    // not selected 
        {
            if (circle->type == NPC)
                gc = wnd->graph->style->black_gc;
            else
                gc = wnd->color[GC_DARK_BLUE];

            circle->highlite = 0;
            break;
        }

    case 1:                    // selected 
        {
            if (circle->type == NPC)
                gc = wnd->color[GC_DARK_RED];
            else
                gc = wnd->color[GC_RED];

            circle->highlite = 1;
            break;
        }

    case 2:                    // Has link 
        {
            gc = wnd->color[GC_PURPLE];

            circle->highlite = 2;
            break;
        }

    case 3:                    // mouse over 
        {
            gc = wnd->color[GC_GREEN];
            break;
        }

    default:
        return;
    }

    circle->position.x += wnd->x_offset;
    circle->position.y += wnd->y_offset;

    // Insert number of links 
    if (circle->link.size ())
        g_string_sprintf (links, "%i", circle->link.size ());

    // Indicate wether node contains additional code
    if (((Circle *) circle)->variables != "" || ((Circle *) circle)->conditions != "")
        g_string_append_c (links, '!');

    // place text in circles center 
    x = circle->position.x + (20 - gdk_string_width (wnd->font, links->str)) / 2;
    y = circle->position.y + (21 + gdk_string_height (wnd->font, links->str)) / 2;

    // draw everything 
    gdk_draw_arc (wnd->pixmap, wnd->graph->style->white_gc, TRUE, circle->position.x, circle->position.y, 20, 20, 0, 36000);
    gdk_draw_arc (wnd->pixmap, gc, FALSE, circle->position.x, circle->position.y, 20, 20, 0, 36000);
    gdk_draw_string (wnd->pixmap, wnd->font, gc, x, y, links->str);
    gtk_widget_draw (wnd->graph, &circle->position);

    circle->position.x -= wnd->x_offset;
    circle->position.y -= wnd->y_offset;

    g_string_free (links, TRUE);
}

// Init arrow-shape 
void 
redraw_arrow (MainFrame * wnd, Arrow * arrow)
{
    DlgNode *node;
    GdkPoint start, end;
    float a, b, x, y;

    // assure that we only redraw arrows 
    if (arrow->type != LINK)
        return;

    // start- and end-point of arrow 
    node = arrow->prev[0];
    start = center_point (node->position);

    node = arrow->next[0];
    end = center_point (node->position);

    // gradient of line 
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

    // line from start- to end-circle 
    arrow->line[0] = offset_point (start, (int)(-10.0f * a), (int)(-10.0f * b));
    arrow->line[1] = offset_point (end, (int)(10.0f * a), (int)(10.0f * b));

    // arrows tip 
    arrow->tip[0] = arrow->line[1];
    arrow->tip[1] = offset_point (arrow->line[1], (int)(a * 10.0f + b * 5.0f), (int)(b * 10.0f - a * 5.0f));
    arrow->tip[2] = offset_point (arrow->line[1], (int)(a * 10.0f - b * 5.0f), (int)(b * 10.0f + a * 5.0f));

    // calculate arrows new position 
    arrow->position = create_rectangle (arrow->line[0], arrow->line[1]);
    arrow->position = inflate_rectangle (arrow->position, 2, 2);
}

// Draws an arrow 
void 
draw_arrow (MainFrame * wnd, Arrow * arrow, int highlite)
{
    GdkGC *gc;
    GdkPoint line[2];
    GdkPoint tip[3];
    GdkRectangle rect = inflate_rectangle (arrow->position, 10, 10);

    // assure that we only redraw arrows 
    if (arrow->type != LINK)
        return;

    // select color for drawing arrow 
    switch (highlite)
    {
    case 0:                    // not selected 
        {
            gc = wnd->graph->style->black_gc;
            arrow->highlite = 0;
            break;
        }

    case 1:                    // selected 
        {
            gc = wnd->color[GC_DARK_RED];
            arrow->highlite = 1;
            break;
        }

    case 2:                    // link 
        {
            gc = wnd->color[GC_PURPLE];
            arrow->highlite = 2;
            break;
        }

    case 3:                    // Mouse over 
        {
            gc = wnd->color[GC_GREEN];
            break;
        }

    default:
        return;
    }
    
    // Current position 
    line[0] = offset_point (arrow->line[0], wnd->x_offset, wnd->y_offset);
    line[1] = offset_point (arrow->line[1], wnd->x_offset, wnd->y_offset);
    tip[0] = offset_point (arrow->tip[0], wnd->x_offset, wnd->y_offset);
    tip[1] = offset_point (arrow->tip[1], wnd->x_offset, wnd->y_offset);
    tip[2] = offset_point (arrow->tip[2], wnd->x_offset, wnd->y_offset);

    rect.x += wnd->x_offset;
    rect.y += wnd->y_offset;
    
    // draw everything 
    gdk_draw_polygon (wnd->pixmap, gc, FALSE, line, 2);
    gdk_draw_polygon (wnd->pixmap, wnd->graph->style->white_gc, TRUE, tip, 3);
    gdk_draw_polygon (wnd->pixmap, gc, FALSE, tip, 3);
    gtk_widget_draw (wnd->graph, &rect);
}

// redraw graph 
void 
redraw_graph (MainFrame * wnd)
{
    u_int32 i;
    DlgNode *node;
    GdkRectangle t;

    // get visible part of graph 
    t.x = -wnd->x_offset;
    t.y = -wnd->y_offset;
    t.width = wnd->graph->allocation.width;
    t.height = wnd->graph->allocation.height;

    // Clear graph 
    gdk_draw_rectangle (wnd->pixmap, wnd->graph->style->white_gc, TRUE, 0, 0, t.width, t.height);

    // check for each node, wether it is visible 
    for (i = 0; i < wnd->number; i++)
    {
        node = wnd->nodes[i];

        if (rect_in_rect (t, node->position))
        {
            if (node->type == LINK)
                draw_arrow (wnd, (Arrow *) node, node->highlite);
            else
                draw_circle (wnd, node, node->highlite);
        }
    }

    // normalize rect 
    t.x = 0;
    t.y = 0;

    // draw backing image to screen
    if (wnd->mode == MULTI_SELECT)
    {
        wnd->mode = IDLE;
        gtk_widget_draw (wnd->graph, &t);
        wnd->mode = MULTI_SELECT;
    }
    else gtk_widget_draw (wnd->graph, &t);

    // Mark object below cursor if neccessary 
    // if (wnd->mode != OBJECT_DRAGGED)
    // {
    //     wnd->below_pointer = NULL;
    //     mouse_over (wnd, point);
    // }
}

void draw_multiselbox (MainFrame * wnd, GdkPoint end)
{
    GdkRectangle rect;

    if (wnd->mode != MULTI_SELECT)
    {
        wnd->multsel_start = end;
        wnd->mode = MULTI_SELECT;

        return;
    }

    rect = create_rectangle (wnd->multsel_start, end);
    if (rect.width == 0) rect.width = 1;
    if (rect.height == 0) rect.height = 1;
    
    gtk_widget_draw (wnd->graph, &rect);
}

// create new dialogue 
void
new_dialogue (MainFrame * wnd)
{
    // first, clear current dialogue and set app back ... 
    deselect_object (wnd);
    delete_dialogue (wnd);
    init_app (wnd);

    // ... then update display 
    update_gui (wnd);
    redraw_graph (wnd);
}

// load a dialogue from disk 
void 
load_dialogue (MainFrame * wnd, const char *file)
{
    int i = 1, n;
    string s;
    Circle *circle;
    Arrow *arrow;
 
    // Was load cancelled? 
    if (file == NULL)
        return;

    // open File 
    loadlgin = fopen (file, "rb");

    if (!loadlgin)
    {
        g_message ("could not open file %s", file);
        return;
    }

    // first, clear current dialogue and set app back ... 
    deselect_object (wnd);
    delete_dialogue (wnd);
    init_app (wnd);

    // ... then load all nodes ... 

    while (i)
    {
        switch (i = parse_dlgfile (s, n))
        {
            case LOAD_VARS:
            {
                if (parse_dlgfile (s, n) == LOAD_STR) wnd->pset_vars = s;
                break;             
            }

            case LOAD_FUNC:
            {
                if (parse_dlgfile (s, n) == LOAD_STR) wnd->cust_func = s;
                break;             
            }

            case LOAD_NAME:
            {
                if (parse_dlgfile (s, n) == LOAD_STR)
                {
                    delete wnd->myplayer->name;
                    wnd->myplayer->name = strdup (s.c_str ());
                }
                break;             
            }

            case LOAD_RACE:
            {
                if (parse_dlgfile (s, n) == LOAD_NUM); // wnd->myplayer->set ("race", n);
                break;
            }

            case LOAD_GENDER:
            {
                if (parse_dlgfile (s, n) == LOAD_NUM); // wnd->myplayer->set ("gender", n);
                break;
            }
            
            case LOAD_CIRCLE:
            {
                circle = new Circle;
                circle->load (wnd->number++);

                wnd->nodes.push_back (circle);
                break;
            }

            case LOAD_ARROW:
            {
                arrow = new Arrow;
                arrow->load (wnd->number++, wnd->nodes);
                redraw_arrow (wnd, arrow);

                wnd->nodes.push_back (arrow);
                break;
            }
            
            default: break;
        }
    }

    // ... and redraw them 
    redraw_graph (wnd);

    // set number of nodes
    wnd->number = wnd->nodes.size ();

    // set new window - title 
    wnd->file_name = g_strdup (file);

    // Make gui resemble the loaded dialogue
    update_gui (wnd);
    
    // clean up 
    fclose (loadlgin);
}

// save a dialogue to disk 
void 
save_dialogue (MainFrame * wnd)
{
    time_t tm;
    u_int32 i;
    u_int32 index = 0;
    u_int32 *table = new u_int32[wnd->number];
    GString *file = g_string_new (NULL);
    ofstream out;
    GtkWidget *fs = create_fileselection (file, 0);

    gtk_file_selection_set_filename ((GtkFileSelection *) fs, wnd->file_name);

    // chose file 
    gtk_widget_show (fs);
    gtk_main ();

    // Was save cancelled? 
    if (file->str == NULL)
    {
        g_string_free (file, TRUE);
        return;
    }

    // open File 
    out.open (file->str);

    if (!out)
    {
        g_message ("could not open file %s", file->str);
        g_string_free (file, TRUE);
        return;
    }

    // Be on the safe side 
    sort_nodes (wnd);

    // Write Header: Adonthell Dialogue System file version 1 
    out << "# Adonthell Dialogue System source file\n"
        << "# Generated by Dialogue Editor v" << _VERSION_ 
        << ", (C) 2000 Kai Sterker\n\n"
        << "# Last saved by " << g_get_user_name () << " on ";

    time (&tm);
    out << asctime (gmtime (&tm)) << "\n";
    
    // Number of nodes 
    out << "# " << wnd->number << " Nodes in Dialogue\n";

    // Preset variables
    if (wnd->pset_vars != "") out << "\nVars §" << wnd->pset_vars << "§\n";

    // Preset variables
    if (wnd->cust_func != "") out << "\nFunc §" << wnd->cust_func << "§\n";

    // Players name, race and gender
    if (strcmp ("Banec", wnd->myplayer->name))
        out << "\nName §" << wnd->myplayer->name << "§";
    if (wnd->myplayer->get ("race") != 0)
        out << "\nRace " << wnd->myplayer->get ("race");
    if (wnd->myplayer->get ("gender") != 1)
        out << "\nGender " << wnd->myplayer->get ("gender");
    
    // Save Circles and create position-table 
    for (i = 0; i < wnd->number; i++)
        if (wnd->nodes[i]->type != LINK)
        {
            wnd->nodes[i]->save (out, table);
            table[i] = index;
            index++;
        }

    // Save Arrows 
    for (i = 0; i < wnd->number; i++)
        if (wnd->nodes[i]->type == LINK)
        {
            wnd->nodes[i]->save (out, table);
            index++;
        }

    wnd->file_name = g_strdup (file->str);

    update_gui (wnd);

    // clean up 
    g_string_free (file, TRUE);
    delete[] table;

    out.close ();
}

// stuff to do after loading or saving
void update_gui (MainFrame *wnd)
{
    FILE *test;
    string script = wnd->file_name;
    script += ".py";

    // set new window - title 
    gtk_window_set_title (GTK_WINDOW (wnd->wnd), g_strjoin (NULL,
        "Adonthell Dialogue Editor v", _VERSION_, " - [",
        strrchr (wnd->file_name, '/') + 1, "]", NULL));

    // check wether dialogue script exists and set "run" menuitem accordingly
    test = fopen (script.c_str (), "r");
    if (!test) gtk_widget_set_sensitive (wnd->dialogue_run, FALSE);
    else 
    {
        gtk_widget_set_sensitive (wnd->dialogue_run, TRUE);
        fclose (test);
    }
}

void 
mouse_over (MainFrame * wnd, GdkPoint point)
{
    DlgNode *node = get_cur_selection (wnd, point);

    // Exit function if  
    //   - node below pointer is unchanged
    //   - no node below cursor 
    if (node == wnd->below_pointer)
        return;
    if (node == NULL)
    {
        if (wnd->below_pointer->type == LINK)
            draw_arrow (wnd, (Arrow *) wnd->below_pointer, wnd->below_pointer->highlite);
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

    // remove highlite 
    if (wnd->below_pointer != NULL)
    {
        if (wnd->below_pointer->type == LINK)
            draw_arrow (wnd, (Arrow *) wnd->below_pointer, wnd->below_pointer->highlite);
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

    // Draw border and objects text 
    show_sel (wnd, node);
    show_tooltip (wnd, node);
}

// Select a Node 
int 
select_object (MainFrame * wnd, GdkPoint point)
{
    DlgNode *node = get_cur_selection (wnd, point);
    u_int32 i;

    // No object selected 
    if (node == NULL)
        return 0;

    // set selected object 
    wnd->selected_node = node;

    // highlite node and its links 
    switch (node->type)
    {
    case NPC:
    case PLAYER:
        {
            draw_circle (wnd, node, 1);

            for (i = 0; i < wnd->selected_node->link.size (); i++)
                draw_arrow (wnd, (Arrow *) wnd->selected_node->link[i], 2);

            break;
        }

    case LINK:
        {
            draw_arrow (wnd, (Arrow *) node, 1);

            for (i = 0; i < wnd->selected_node->link.size (); i++)
                draw_circle (wnd, wnd->selected_node->link[i], 2);

            break;
        }
    }

    // Set new Program-Mode 
    wnd->mode = OBJECT_MARKED;
    
    show_preview (wnd);
    
    return 1;
}

// Select a Node 
void 
select_object_index (MainFrame * wnd, int index)
{
    u_int32 i;

    // set selected object 
    wnd->selected_node = wnd->nodes[index];

    // highlite node and its links 
    switch (wnd->selected_node->type)
    {
    case NPC:
    case PLAYER:
        {
            draw_circle (wnd, wnd->selected_node, 1);

            for (i = 0; i < wnd->selected_node->link.size (); i++)
                draw_arrow (wnd, (Arrow *) wnd->selected_node->link[i], 2);

            break;
        }

    case LINK:
        {
            draw_arrow (wnd, (Arrow *) wnd->selected_node, 1);

            for (i = 0; i < wnd->selected_node->link.size (); i++)
                draw_circle (wnd, wnd->selected_node->link[i], 2);

            break;
        }
    }

    if (wnd->mode != MULTI_MARKED)
    {
        // Set new Program-Mode 
        wnd->mode = OBJECT_MARKED;
        
        show_preview (wnd);
    }
}

// Deselect a Node 
int 
deselect_object (MainFrame * wnd)
{
    u_int32 i;

    // No object selected 
    if (wnd->selected_node == NULL)
        return 0;

    // unhighlite node and its links 
    switch (wnd->selected_node->type)
    {
        case NPC:
        case PLAYER:
        {
            draw_circle (wnd, wnd->selected_node, 0);

            for (i = 0; i < wnd->selected_node->link.size (); i++)
                draw_arrow (wnd, (Arrow *) wnd->selected_node->link[i], 0);

            break;
        }

        case LINK:
        {
            draw_arrow (wnd, (Arrow *) wnd->selected_node, 0);

            for (i = 0; i < wnd->selected_node->link.size (); i++)
                draw_circle (wnd, wnd->selected_node->link[i], 0);

            break;
        }
    }

    // set selected object 
    wnd->selected_node = NULL;

    // Set new Program-Mode 
    wnd->mode = IDLE;

    show_preview (wnd);

    return 1;
}

void multsel_objects (MainFrame * wnd)
{
    u_int32 i;

    wnd->mode = MULTI_MARKED;

    // Add single selection to multi selection
    if (wnd->selected_node)
        add_to_selection (wnd, wnd->selected_node);

    wnd->multsel_rect.x -= wnd->x_offset;
    wnd->multsel_rect.y -= wnd->y_offset;

    // select (toggle) all nodes in rectangle specified by the user 
    for (i = 0; i < wnd->number; i++)
        if (wnd->nodes[i]->type != LINK)
            if (rect_in_rect (wnd->nodes[i]->position, wnd->multsel_rect))
                add_to_selection (wnd, wnd->nodes[i]);

    wnd->multsel_rect.x += wnd->x_offset;
    wnd->multsel_rect.y += wnd->y_offset;

    // see how many nodes remain selected
    switch (wnd->multsel.size ())
    {
        case 0:
        {
            wnd->mode = IDLE;
            wnd->selected_node = NULL;
            break;
        }
        case 1:
        {
            wnd->mode = OBJECT_MARKED;
            wnd->selected_node = wnd->multsel[0];
            wnd->multsel.clear ();
            break;
        }
        default:
        {
            wnd->mode = MULTI_MARKED;
            wnd->selected_node = NULL;
        }
    }

    gtk_widget_draw (wnd->graph, &wnd->multsel_rect);
    show_preview (wnd);
}

void add_to_selection (MainFrame *wnd, DlgNode *node)
{
    // select 'new' nodes and deselect 'old' ones
    if (find (wnd->multsel.begin (), wnd->multsel.end (), node) == wnd->multsel.end ())
    {
        wnd->multsel.push_back (node);
        select_object_index (wnd, node->number);
    }
    else
    {
        remove_data (wnd->multsel, node);
        wnd->selected_node = node;
        deselect_object (wnd);
    }
}

void 
show_tooltip (MainFrame *wnd, DlgNode *node)
{
    int x, y;
    GString *str;
    Circle *circle;

    if (node->type == LINK) return;

    if (wnd->tooltip == NULL)
    {
        circle = (Circle *) node;
        str = g_string_new (circle->text.c_str ());

        if (circle->conditions != "")
        {
            str = g_string_prepend (str, "\n\n");
            str = g_string_prepend (str, circle->conditions.c_str ());
        }

        if (circle->variables != "")
        {
            str = g_string_append (str, "\n\n");
            str = g_string_append (str, circle->variables.c_str ());
        }
        
        gdk_window_get_origin (gtk_widget_get_parent_window (wnd->graph), &x, &y);

        x = x + node->position.x + node->position.width + wnd->x_offset;
        y = y + node->position.y + node->position.height + wnd->y_offset;

        wnd->tooltip = create_tooltip (str->str, x, y);
        gtk_widget_show (wnd->tooltip);

        g_string_free (str, TRUE);
    }
}

// Highlight node below cursor 
void 
show_sel (MainFrame * wnd, DlgNode * node)
{
    if (node->type == LINK)
        draw_arrow (wnd, (Arrow *) node, 3);
    else
        draw_circle (wnd, node, 3);

    gtk_widget_draw (wnd->graph, &wnd->below_pointer->position);
}

// Check if a node is below cursor 
DlgNode *
get_cur_selection (MainFrame * wnd, GdkPoint point)
{
    int i, x, dx, dy;
    DlgNode *node;
    GdkRectangle rect;

    // Mousepointer over node? 
    for (i = 0; i < (int) wnd->number; i++)
    {
        node = wnd->nodes[i];
        rect = node->position;

        if (point_in_rect (rect, point))
        {
            // node == circle? -> return 
            if (node->type != LINK)
                return node;

            // node == arrow? -> just a bit geometry ;-) 
            // straight g: ax + by + c = 0 through 2 points p(x, y) und q(x, y): 
            // with a = (p.y - q.y), b = (q.x - p.x), c = (-(p.x * a + p.y * b)) 

            // insert point in above function; values |x| < 500 are close to the arrow 
            else
            {
                Arrow *arrow = (Arrow *) node;
                
                dx = arrow->line[1].x - arrow->line[0].x;
                dy = arrow->line[0].y - arrow->line[1].y;

                x = (dy * point.x + dx * point.y) - (arrow->line[0].x * dy + arrow->line[0].y * dx);

                if (x < 500 && x > -500)
                    return node;
            }
        }
    }

    return NULL;
}

// center the view on node 
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

    // is node outside the views inner 60% ? 
    if (!point_in_rect (inflate_rectangle (view, x, y), pos))
    {
        wnd->x_offset -= pos.x - wnd->graph->allocation.width / 2;
        wnd->y_offset -= pos.y - wnd->graph->allocation.height / 2;

        redraw_graph (wnd);
    }
}

// prepare everything for 'auto-scrolling' (TM) ;-)
void begin_scrolling (MainFrame *wnd, GdkPoint point)
{
    int width = wnd->graph->allocation.width;
    int height = wnd->graph->allocation.height;

    point.x += wnd->x_offset;
    point.y += wnd->y_offset;

    wnd->scroll_x = 0;
    wnd->scroll_y = 0;

    // set scrolling offset and direction    
    if (point.x < 20) wnd->scroll_x = 15;
    if (point.y < 20) wnd->scroll_y = 15;
    if (point.x + 20 > width) wnd->scroll_x = -15;
    if (point.y + 20 > height) wnd->scroll_y = -15;

    // enable scrolling
    if (wnd->scroll_x || wnd->scroll_y)
    {
        if (!wnd->scroll)
        {
            wnd->scroll = 1;
            gtk_timeout_add (100, scroll_graph, wnd);
        }
    }
    else wnd->scroll = 0;
}

// Once 'auto-scrolling' is activated, this function is called every
// 10th of a second until it returns FALSE
int scroll_graph (gpointer data)
{
    MainFrame *wnd = (MainFrame *) data;
    int x, y;

    gtk_widget_get_pointer (wnd->graph, &x, &y);
    
    // stop scrolling if outside widget or too far from widget's border
    if (x < 0 || x > wnd->graph->allocation.width || 
        y < 0 || y > wnd->graph->allocation.height) wnd->scroll = 0;   
    if (!wnd->scroll) return FALSE;
    
    // wait approx. 1 second before starting to scroll
    if (wnd->scroll++ < 6) return TRUE;
    
    wnd->x_offset += wnd->scroll_x;
    wnd->y_offset += wnd->scroll_y;

    redraw_graph (wnd);

    return TRUE; 
}

// Removes data from a vector and reports success or failure
template <class T> u_int8 remove_data (vector<T> &array, T data)
{
    vector<T>::iterator i;

    if ((i = find (array.begin (), array.end (), data)) == array.end ()) return 0;
    else array.erase (i);

    return 1;
}
