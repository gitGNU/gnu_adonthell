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
 * @file gui_graph_events.cc
 *
 * @author Kai Sterker
 * @brief Event-callbacks for the dialogue graph view
 */

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "gui_dlgedit.h"

// Window resized
gint configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    GuiGraph *graph = (GuiGraph *) data;

    // resize the drawing area
    graph->resizeSurface (widget);

    // do a redraw
    graph->draw ();
    
    // redraw the preview
    GuiDlgedit::window->list()->draw ();

    return TRUE;
}

// Redraw the screen from the backing pixmap
gint expose_event (GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
    GuiGraph *graph = (GuiGraph *) data;

    gdk_draw_pixmap (widget->window, widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
        graph->pixmap (), event->area.x, event->area.y, event->area.x, event->area.y,
        event->area.width, event->area.height);
/*
    else
    {
        GdkGCValues val;
        
        event->area.x += MainWnd->x_offset;
        event->area.y += MainWnd->y_offset;
    
        gdk_gc_get_values (widget->style->black_gc, &val);
        
        gdk_draw_pixmap (event->window, widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
            MainWnd->pixmap, MainWnd->multsel_rect.x, MainWnd->multsel_rect.y, MainWnd->multsel_rect.x, MainWnd->multsel_rect.y,
            MainWnd->multsel_rect.width, MainWnd->multsel_rect.height);

        gdk_gc_set_line_attributes (widget->style->black_gc, val.line_width,
            GDK_LINE_ON_OFF_DASH, val.cap_style, val.join_style);
           
        gdk_draw_rectangle (event->window, widget->style->black_gc, FALSE, 
            event->area.x, event->area.y, event->area.width-1, event->area.height-1);

        MainWnd->multsel_rect = event->area;

        gdk_gc_set_line_attributes (widget->style->black_gc, val.line_width,
            GDK_LINE_SOLID, val.cap_style, val.join_style);
    }
*/       
    return FALSE;
}

/* Mouse-button pressed on Drawing Area */
gint 
button_press_event (GtkWidget * widget, GdkEventButton * event, gpointer data)
{
    /*
    MainFrame *MainWnd = (MainFrame *) data;
    GdkPoint point;
    u_int32 i;

    point.x = (s_int32) event->x - MainWnd->x_offset;
    point.y = (s_int32) event->y - MainWnd->y_offset;

    // Left Button down
    switch (event->button)
    {
        // Middle Button
        case 2:
        {
            // If nothing selected, see if we're over a node
            if (MainWnd->mode == IDLE)
                select_object (MainWnd, point);

            // Edit node
            if (MainWnd->mode == OBJECT_MARKED)
                edit_node (MainWnd);
                
            break;
        }

        // Right Button
        case 3:
        {
            switch (MainWnd->mode)
            {
                // circle              -> remove link
                // Unoccupied location -> deselect object
                case OBJECT_MARKED:
                {
                    if (!remove_link (MainWnd, point))
                        deselect_object (MainWnd);
                    break;
                }

                // More then one object marked: deselect all of 'em
                case MULTI_MARKED:
                {
                    for (i = 0; i < MainWnd->multsel.size (); i++)
                    {
                        MainWnd->selected_node = MainWnd->multsel[i];
                        deselect_object (MainWnd);                      
                    }

                    MainWnd->multsel.clear ();
                    break;
                }
                
                default: break;
            }

            break;
        }

        default: break;
    }
*/
    return TRUE;
}

// Mouse moved over drawing area
gint motion_notify_event (GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
    GuiGraph *graph = (GuiGraph *) data;
    DlgPoint point ((int) event->x, (int) event->y);

    graph->mouseMoved (point);
    
    return FALSE;
/*    
    MainFrame *MainWnd = (MainFrame *) data;
    GdkPoint point;
    
    point.x = (s_int32) event->x - MainWnd->x_offset;
    point.y = (s_int32) event->y - MainWnd->y_offset;

    // start/stop scrolling of dialogue area
    if (MainWnd->number > 0) begin_scrolling (MainWnd, point);

    // Dragging dialogue nodes
    if (event->state == GDK_BUTTON_PRESS_MASK)
    {
        if (MainWnd->dragged_node == NULL)
        {
            if (!new_mover (MainWnd, point)) draw_multiselbox (MainWnd, point);
        }
        else
        {
            move_node (MainWnd, point);

            // update graph
            redraw_graph (MainWnd);
        }
    }

    // Highlighting dialogue nodes
    else mouse_over (MainWnd, point);
*/
}

/* Mouse-button released on Drawing Area */
gint 
button_release_event (GtkWidget * widget, GdkEventButton * event, gpointer data)
{
/*    
    MainFrame *MainWnd = (MainFrame *) data;
    GdkPoint point;
    u_int32 index;

    point.x = (s_int32) event->x - MainWnd->x_offset;
    point.y = (s_int32) event->y - MainWnd->y_offset;

    if (event->button == 1)
    {
        switch (MainWnd->mode)
        {
            // Unoccupied location -> create new Circle
            // Circle hit -> mark Circle
            case IDLE:
            {
                if (!select_object (MainWnd, point))
                    new_circle (MainWnd, point, NODE_NPC);
                break;
            }

            // Arrow hit -> add to marked Circles links
            // Else -> Create new Arrow
            case OBJECT_MARKED:
            {
                if (!new_link (MainWnd, point))
                   new_arrow (MainWnd, point);
                break;
            }

            case OBJECT_DRAGGED:
            {
                if (MainWnd->dragged_node != NULL)
                    end_moving (MainWnd, point);
                break;
            }

            case MULTI_SELECT:
            {
                multsel_objects (MainWnd);
                break;
            }

            // If single node selected -> cancel multi-selection 
            case MULTI_MARKED:
            {
                if (get_cur_selection (MainWnd, point))
                {
                    for (index = 0; index < MainWnd->multsel.size (); index++)
                    {
                        MainWnd->selected_node = MainWnd->multsel[index];
                        deselect_object (MainWnd);                      
                    }

                    MainWnd->multsel.clear ();
                    select_object (MainWnd, point);
                }
            }
            
            default: break;
        }
    }
*/
    return TRUE;
}

// Key pressed
guint key_press_notify_event (GtkWidget * widget, GdkEventKey * event, gpointer user_data)
{
    GuiGraph *graph = (GuiGraph *) user_data;

    switch (event->keyval)
    {
        // select parent node
        case GDK_Up:
        {
            if (graph->selectParent ()) graph->centerNode ();
            break;
        }
        
        // select child node
        case GDK_Down:
        {
            if (graph->selectChild ()) graph->centerNode ();
            break;
        }
        
        // select sibling to the left
        case GDK_Left:
        {
            if (graph->selectSibling (PREV)) graph->centerNode ();
            break;
        }
        
        // select sibling to the right
        case GDK_Right:
        {
            if (graph->selectSibling (NEXT)) graph->centerNode ();
            break;
        }
    }
/*
    MainFrame *MainWnd = (MainFrame *) data;
    u_int32 index, offset = 5;

    if (event->state & GDK_SHIFT_MASK) offset *= 10;

    switch (event->keyval)
    {
        // Edit selected node
        case GDK_Return:
        {
            edit_node (MainWnd);
            break;
        }

        // Deselect selected node
        case GDK_Escape:
        {
            if (MainWnd->mode == OBJECT_MARKED)
                deselect_object (MainWnd);

            if (MainWnd->mode == MULTI_MARKED)
            {
                for (index = 0; index < MainWnd->multsel.size (); index++)
                {
                    MainWnd->selected_node = MainWnd->multsel[index];
                    deselect_object (MainWnd);                      
                }

                MainWnd->multsel.clear ();
            }

            break;
        }

        // Delete selected node
        case GDK_Delete:
        {
            if (MainWnd->mode == OBJECT_MARKED)
                delete_node (MainWnd);

            if (MainWnd->mode == MULTI_MARKED)
            {
                for (index = 0; index < MainWnd->multsel.size (); index++)
                {
                    MainWnd->selected_node = MainWnd->multsel[index];
                    delete_node (MainWnd);                      
                }

                MainWnd->multsel.clear ();
            }

            break;
        }

        // Cycle through Circles
        case GDK_Tab:
        {
            if (MainWnd->nodes.empty ())
                break;

            // multiple nodes selected -> deselect 'em all
            if (MainWnd->mode == MULTI_MARKED)
                for (index = 0; index < MainWnd->multsel.size (); index++)
                {
                     MainWnd->selected_node = MainWnd->multsel[index];
                     deselect_object (MainWnd);                      
                }

            // nothing selected -> select first 
            if (MainWnd->selected_node == NULL) index = 0;            

            // a node selected -> select next
            else 
            {   
                index = (MainWnd->number == MainWnd->selected_node->number + 1) ? 0 : MainWnd->selected_node->number + 1;
                deselect_object (MainWnd);
            }

            // Skip Arrows
            while (MainWnd->nodes[index]->type == NODE_LINK)
                index = (MainWnd->number == index + 1) ? 0 : index + 1;
            
            center_object (MainWnd, MainWnd->nodes[index]);
            select_object_index (MainWnd, index);

            break;
        }
        // Center screen on selected node
        case GDK_c:
        {
            if (MainWnd->selected_node != NULL)
            {
                center_object (MainWnd, MainWnd->selected_node);
            }
            
            break;
        }
        // scrolling
        case GDK_Up:
        case GDK_KP_Up:
        {
            MainWnd->y_offset += offset;
            redraw_graph (MainWnd);
            break;
        }
        case GDK_Down:
        case GDK_KP_Down:
        {
            MainWnd->y_offset -= offset;
            redraw_graph (MainWnd);
            break;
        }
        case GDK_Left:
        case GDK_KP_Left:
        {
            MainWnd->x_offset += offset;
            redraw_graph (MainWnd);
            break;
        }
        case GDK_Right:
        case GDK_KP_Right:
        {
            MainWnd->x_offset -= offset;
            redraw_graph (MainWnd);
            break;
        }
        case GDK_KP_Home:
        {
            MainWnd->x_offset += offset;
            MainWnd->y_offset += offset;
            redraw_graph (MainWnd);
            break;
        }
        case GDK_KP_End:
        {
            MainWnd->x_offset += offset;
            MainWnd->y_offset -= offset;
            redraw_graph (MainWnd);
            break;
        }
        case GDK_KP_Page_Up:
        {
            MainWnd->x_offset -= offset;
            MainWnd->y_offset += offset;
            redraw_graph (MainWnd);
            break;
        }
        case GDK_KP_Page_Down:
        {
            MainWnd->x_offset -= offset;
            MainWnd->y_offset -= offset;
            redraw_graph (MainWnd);
            break;
        }
        default:
            break;
    }
*/
    return TRUE;
}
