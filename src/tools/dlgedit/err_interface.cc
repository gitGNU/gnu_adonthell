/*
   $Id$
 
   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "error_dlg.h"
#include "err_callbacks.h"
#include "err_interface.h"
#include "callbacks.h"

GtkWidget*
create_error_window (error_dlg *dlg)
{
  GtkWidget *error_window;
  GtkWidget *vbox1;
  GtkWidget *scrolledwindow1;
  GtkWidget *error_msg;
  GtkWidget *hseparator1;
  GtkWidget *hbuttonbox1;
  GtkWidget *error_close;

  error_window = gtk_window_new (GTK_WINDOW_DIALOG);
  gtk_object_set_data (GTK_OBJECT (error_window), "error_window", error_window);
  gtk_widget_set_usize (error_window, 400, 320);
  gtk_window_set_title (GTK_WINDOW (error_window), "Message Window");
  gtk_window_set_policy (GTK_WINDOW (error_window), FALSE, FALSE, FALSE);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (error_window), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (error_window), vbox1);
  gtk_container_set_border_width (GTK_CONTAINER (vbox1), 4);

  scrolledwindow1 = gtk_scrolled_window_new ((GtkAdjustment*) NULL, (GtkAdjustment*) NULL);
  gtk_widget_ref (scrolledwindow1);
  gtk_object_set_data_full (GTK_OBJECT (error_window), "scrolledwindow1", scrolledwindow1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  error_msg = gtk_text_new ((GtkAdjustment*) NULL, (GtkAdjustment*) NULL);
  gtk_widget_ref (error_msg);
  gtk_object_set_data_full (GTK_OBJECT (error_window), "error_msg", error_msg,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (error_msg);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), error_msg);
  dlg->entry = error_msg;

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_ref (hseparator1);
  gtk_object_set_data_full (GTK_OBJECT (error_window), "hseparator1", hseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, FALSE, TRUE, 2);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox1);
  gtk_object_set_data_full (GTK_OBJECT (error_window), "hbuttonbox1", hbuttonbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, TRUE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_SPREAD);
  gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);
  gtk_button_box_set_child_ipadding (GTK_BUTTON_BOX (hbuttonbox1), 0, -1);

  error_close = gtk_button_new_with_label ("Close");
  gtk_widget_ref (error_close);
  gtk_object_set_data_full (GTK_OBJECT (error_window), "error_close", error_close,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (error_close);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), error_close);
  gtk_container_set_border_width (GTK_CONTAINER (error_close), 2);
  GTK_WIDGET_SET_FLAGS (error_close, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (error_close), "clicked",
                      GTK_SIGNAL_FUNC (on_error_close_clicked),
                      dlg);
  gtk_signal_connect (GTK_OBJECT (error_window), "delete_event",
                      GTK_SIGNAL_FUNC (on_error_delete),
                      dlg);

  gtk_widget_grab_focus (error_close);
  gtk_widget_grab_default (error_close);

  gtk_widget_show (error_window);

  return error_window;
}

