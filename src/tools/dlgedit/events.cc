/*
   Copyright (C) 1999 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
 */

#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "../../types.h"
#include "dlgnode.h"
#include "main.h"
#include "graph.h"

/* Window resized */
gint 
configure_event (GtkWidget * widget, GdkEventConfigure * event, gpointer data)
{
    MainFrame *MainWnd = (MainFrame *) data;

    if (MainWnd->pixmap)
        gdk_pixmap_unref (MainWnd->pixmap);

    MainWnd->pixmap = gdk_pixmap_new (widget->window, widget->allocation.width,
        widget->allocation.height, -1);

    /* do a redraw */
    redraw_graph (MainWnd);
    show_preview (MainWnd);

    return TRUE;
}

/* Redraw the screen from the backing pixmap */
gint 
expose_event (GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
    MainFrame *MainWnd = (MainFrame *) data;

    gdk_draw_pixmap (widget->window, widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
        MainWnd->pixmap, event->area.x, event->area.y, event->area.x, event->area.y,
        event->area.width, event->area.height);

    return FALSE;
}

/* Mouse-button pressed on Drawing Area */
gint 
button_press_event (GtkWidget * widget, GdkEventButton * event, gpointer data)
{
    MainFrame *MainWnd = (MainFrame *) data;
    GdkPoint point;

    point.x = (s_int32) event->x - MainWnd->x_offset;
    point.y = (s_int32) event->y - MainWnd->y_offset;

    /* Left Button down */
    switch (event->button)
    {
    case 1:
        {
            switch (MainWnd->mode)
            {
                /* Unoccupied location -> create new Circle */
                /* Circle hit -> mark Circle */
            case IDLE:
                {
                    if (!select_object (MainWnd, point))
                        new_circle (MainWnd, point, NPC);
                    break;
                }

                /* Arrow hit -> add to marked Circles links */
                /* Else -> Create new Arrow */
            case OBJECT_MARKED:
                {
                    if (!new_link (MainWnd, point))
                        new_arrow (MainWnd, point);
                    break;
                }

            default:
                break;
            }

            break;
        }

        /* Middle Button */
    case 2:
        {
            /* If nothing selected, see if we're over a node */
            if (MainWnd->mode == IDLE)
                select_object (MainWnd, point);

            /* Edit node */
            if (MainWnd->mode == OBJECT_MARKED)
                edit_node (MainWnd);
                
            break;
        }

        /* Right Button */
    case 3:
        {
            switch (MainWnd->mode)
            {
            case IDLE:
                {
                    break;
                }

                /* circle              -> remove link */
                /* Unoccupied location -> deselect object */
            case OBJECT_MARKED:
                {
                    if (!remove_link (MainWnd, point))
                        deselect_object (MainWnd);
                    break;
                }

            default:
                break;
            }

            break;
        }

    default:
        break;
    }

    return TRUE;
}

/* Mouse moved over drawing area */
gint 
motion_notify_event (GtkWidget * widget, GdkEventMotion * event, gpointer data)
{
    MainFrame *MainWnd = (MainFrame *) data;
    GdkPoint point;

    point.x = (s_int32) event->x - MainWnd->x_offset;
    point.y = (s_int32) event->y - MainWnd->y_offset;

    switch (event->state)
    {
        /* Dragging dialogue nodes */
    case GDK_BUTTON_PRESS_MASK:
        {
            if (MainWnd->dragged_node == NULL)
                new_mover (MainWnd, point);
            else
            {
                move_node (MainWnd, point);

                /* update graph */
                redraw_graph (MainWnd);
            }
            break;
        }

        /* Highlighting dialogue nodes */
    default:
        {
            mouse_over (MainWnd, point);
            break;
        }
    }

    return FALSE;
}

/* Mouse-button released on Drawing Area */
gint 
button_release_event (GtkWidget * widget, GdkEventButton * event, gpointer data)
{
    MainFrame *MainWnd = (MainFrame *) data;
    GdkPoint point;

    point.x = (s_int32) event->x - MainWnd->x_offset;
    point.y = (s_int32) event->y - MainWnd->y_offset;

    if (MainWnd->dragged_node != NULL)
        end_moving (MainWnd, point);

    return TRUE;
}

/* Key pressed */
guint 
key_press_notify_event (GtkWidget * widget, GdkEventKey * event, gpointer data)
{
    MainFrame *MainWnd = (MainFrame *) data;
    u_int32 index;

    switch (event->keyval)
    {
        /* Edit selected node */
    case GDK_Return:
        {
            edit_node (MainWnd);
            break;
        }

        /* Deselect selected node */
    case GDK_Escape:
        {
            deselect_object (MainWnd);
            break;
        }

        /* Delete selected node */
    case GDK_Delete:
        {
            delete_node (MainWnd);
            break;
        }

        /* Cycle through nodes */
    case GDK_Tab:
        {
            if (MainWnd->nodes.empty ())
                break;

            /* nothing selected -> select first */
            if (MainWnd->selected_node == NULL)
            {
                center_object (MainWnd, MainWnd->nodes[0]);
                select_object_index (MainWnd, 0);
            }
            
            /* a node selected -> select next */
            else
            {
                index = (MainWnd->number == MainWnd->selected_node->number + 1) ? 0 : MainWnd->selected_node->number + 1;

                deselect_object (MainWnd);
                center_object (MainWnd, MainWnd->nodes[index]);
                select_object_index (MainWnd, index);
            }

            break;
        }
        /* Center screen on selected node */
    case GDK_c:
        {
            if (MainWnd->selected_node != NULL)
            {
                center_object (MainWnd, MainWnd->selected_node);
            }
            
            break;
        }
        /* scrolling */
    case GDK_Up:
    case GDK_KP_Up:
        {
            MainWnd->y_offset += 5;
            redraw_graph (MainWnd);
            break;
        }
    case GDK_Down:
    case GDK_KP_Down:
        {
            MainWnd->y_offset -= 5;
            redraw_graph (MainWnd);
            break;
        }
    case GDK_Left:
    case GDK_KP_Left:
        {
            MainWnd->x_offset += 5;
            redraw_graph (MainWnd);
            break;
        }
    case GDK_Right:
    case GDK_KP_Right:
        {
            MainWnd->x_offset -= 5;
            redraw_graph (MainWnd);
            break;
        }
    case GDK_KP_Home:
        {
            MainWnd->x_offset += 5;
            MainWnd->y_offset += 5;
            redraw_graph (MainWnd);
            break;
        }
    case GDK_KP_End:
        {
            MainWnd->x_offset += 5;
            MainWnd->y_offset -= 5;
            redraw_graph (MainWnd);
            break;
        }
    case GDK_KP_Page_Up:
        {
            MainWnd->x_offset -= 5;
            MainWnd->y_offset += 5;
            redraw_graph (MainWnd);
            break;
        }
    case GDK_KP_Page_Down:
        {
            MainWnd->x_offset -= 5;
            MainWnd->y_offset -= 5;
            redraw_graph (MainWnd);
            break;
        }
    default:
        break;
    }

    return TRUE;
}
