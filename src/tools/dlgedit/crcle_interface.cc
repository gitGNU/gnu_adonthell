/*
   $Id$ 

   Copyright (C) 2000 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <gtk/gtk.h>

#include "circle.h"
#include "crcle_callbacks.h"
#include "crcle_interface.h"
#include "callbacks.h" 

GtkWidget*
create_dlg_node_window (Circle *circle, crcle_dlg *dlg)
{
    GtkWidget *dlg_node_window;
    GtkWidget *vbox1;
    GtkWidget *notebook1;
    GtkWidget *vbox2;
    GtkWidget *scrolledwindow1;
    GtkWidget *text_entry;
    GSList *character_group = NULL;
    GtkWidget *player_button;
    GtkWidget *hbox1;
    GtkWidget *npc_button;
    GtkWidget *npc_selection;
    GtkWidget *npc_selection_menu;
    GtkWidget *glade_menuitem;
    GtkWidget *label5;
    GtkWidget *npc_mood;
    GtkWidget *npc_mood_menu;
    GtkWidget *label1;
    GtkWidget *scrolledwindow6;
    GtkWidget *condition_entry;
    GtkWidget *label2;
    GtkWidget *scrolledwindow5;
    GtkWidget *variable_entry;
    GtkWidget *label3;
    GtkWidget *label6;
    GtkWidget *label4;
    GtkWidget *frame1;
    GtkWidget *scrolledwindow4;
    GtkWidget *annotation_entry;
    GtkWidget *hbuttonbox1;
    GtkWidget *ok_button;
    GtkWidget *cancel_button;
    GtkTooltips *tooltips;
    GdkColor dark_blue;
    GdkColor green;

    dark_blue.red = 0;
    dark_blue.green = 0;
    dark_blue.blue = 35000;

    green.red = 0;
    green.green = 27500;
    green.blue = 0;

    tooltips = gtk_tooltips_new ();

    dlg_node_window = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (dlg_node_window), "dlg_node_window", dlg_node_window);
    gtk_widget_set_usize (dlg_node_window, 400, 320);
    gtk_window_set_title (GTK_WINDOW (dlg_node_window), "Edit Dialogue Node");
    gtk_window_set_position (GTK_WINDOW (dlg_node_window), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal (GTK_WINDOW (dlg_node_window), TRUE);
    gtk_window_set_policy (GTK_WINDOW (dlg_node_window), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (dlg_node_window), vbox1);

    notebook1 = gtk_notebook_new ();
    gtk_widget_ref (notebook1);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "notebook1", notebook1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (notebook1);
    gtk_box_pack_start (GTK_BOX (vbox1), notebook1, TRUE, TRUE, 0);
    gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook1), FALSE);
    gtk_notebook_set_tab_hborder (GTK_NOTEBOOK (notebook1), 6);
    GTK_WIDGET_UNSET_FLAGS (notebook1, GTK_CAN_FOCUS);

    vbox2 = gtk_vbox_new (FALSE, 4);
    gtk_widget_ref (vbox2);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "vbox2", vbox2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox2);
    gtk_container_add (GTK_CONTAINER (notebook1), vbox2);
    gtk_container_set_border_width (GTK_CONTAINER (vbox2), 4);

    scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "scrolledwindow1", scrolledwindow1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_box_pack_start (GTK_BOX (vbox2), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    text_entry = gtk_text_new (NULL, NULL);
    gtk_widget_ref (text_entry);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "text_entry", text_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (text_entry);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), text_entry);
    GTK_WIDGET_SET_FLAGS (text_entry, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, text_entry, "Enter the Text to appear within the conversation", NULL);
    gtk_text_set_editable (GTK_TEXT (text_entry), TRUE);
    gtk_text_set_word_wrap (GTK_TEXT (text_entry), TRUE);
    if (circle->type == NPC) 
        gtk_text_insert (GTK_TEXT (text_entry), text_entry->style->font,
            &text_entry->style->black, &text_entry->style->white,
            circle->text.c_str (), -1);
    else
        gtk_text_insert (GTK_TEXT (text_entry), text_entry->style->font,
            &dark_blue, &text_entry->style->white, circle->text.c_str (), -1);

    dlg->text_entry = text_entry;

    GtkStyle *style = gtk_style_copy (gtk_widget_get_default_style ());
    style->fg[GTK_STATE_NORMAL] = dark_blue;
    style->fg[GTK_STATE_ACTIVE] = dark_blue;
    style->fg[GTK_STATE_PRELIGHT] = dark_blue;
    style->fg[GTK_STATE_SELECTED] = dark_blue;

    GtkWidget *label = gtk_label_new ("Player");
    gtk_widget_set_style (label, style);
    gtk_label_set_justify ((GtkLabel *) label, GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
    gtk_widget_show (label);

    player_button = gtk_radio_button_new (character_group);
    gtk_container_add (GTK_CONTAINER(player_button), label);
    character_group = gtk_radio_button_group (GTK_RADIO_BUTTON (player_button));
    gtk_widget_ref (player_button);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "player_button", player_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (player_button);
    gtk_box_pack_start (GTK_BOX (vbox2), player_button, FALSE, FALSE, 0);
    if (circle->type == PLAYER)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (player_button), TRUE);
    GTK_WIDGET_UNSET_FLAGS (player_button, GTK_CAN_FOCUS);

    hbox1 = gtk_hbox_new (FALSE, 10);
    gtk_widget_ref (hbox1);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "hbox1", hbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, TRUE, 0);

    npc_button = gtk_radio_button_new_with_label (character_group, "NPC:");
    character_group = gtk_radio_button_group (GTK_RADIO_BUTTON (npc_button));
    gtk_widget_ref (npc_button);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "npc_button", npc_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (npc_button);
    gtk_box_pack_start (GTK_BOX (hbox1), npc_button, FALSE, FALSE, 0);
    if (circle->type == NPC)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (npc_button), TRUE);
    GTK_WIDGET_UNSET_FLAGS (npc_button, GTK_CAN_FOCUS);

    npc_selection = gtk_option_menu_new ();
    gtk_widget_ref (npc_selection);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "npc_selection", npc_selection, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (npc_selection);
    gtk_box_pack_start (GTK_BOX (hbox1), npc_selection, TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips, npc_selection, "Select the speaker", NULL);
    npc_selection_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label ("default");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (npc_selection_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (npc_selection), npc_selection_menu);

    label5 = gtk_label_new ("Mood: ");
    gtk_widget_ref (label5);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "label5", label5, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label5);
    gtk_box_pack_start (GTK_BOX (hbox1), label5, TRUE, FALSE, 0);
    gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_RIGHT);
    gtk_misc_set_alignment (GTK_MISC (label5), 0.01, 0.5);

    npc_mood = gtk_option_menu_new ();
    gtk_widget_ref (npc_mood);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "npc_mood", npc_mood, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (npc_mood);
    gtk_box_pack_start (GTK_BOX (hbox1), npc_mood, TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips, npc_mood, "How the speaker's image appears to the player", NULL);
    npc_mood_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label ("friendly");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (npc_mood_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (npc_mood), npc_mood_menu);

    label1 = gtk_label_new ("Text");
    gtk_widget_ref (label1);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "label1", label1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label1);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label1);

    scrolledwindow6 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow6);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "scrolledwindow6", scrolledwindow6, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow6);
    gtk_container_add (GTK_CONTAINER (notebook1), scrolledwindow6);
    gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow6), 4);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow6), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    condition_entry = gtk_text_new (NULL, NULL);
    gtk_widget_ref (condition_entry);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "condition_entry", condition_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (condition_entry);
    gtk_container_add (GTK_CONTAINER (scrolledwindow6), condition_entry);
    GTK_WIDGET_SET_FLAGS (condition_entry, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, condition_entry, "Enter the conditions that have to be met before this node is available in the dialogue", NULL);
    gtk_text_set_editable (GTK_TEXT (condition_entry), TRUE);
    gtk_text_set_word_wrap (GTK_TEXT (condition_entry), TRUE);
    gtk_text_insert (GTK_TEXT (condition_entry), condition_entry->style->font,
        &condition_entry->style->black, &condition_entry->style->white,
        circle->conditions.c_str (), -1);

    style = gtk_style_copy (gtk_widget_get_default_style ());
    style->fg[GTK_STATE_NORMAL] = green;
    style->fg[GTK_STATE_ACTIVE] = green;
    style->fg[GTK_STATE_PRELIGHT] = green;
    style->fg[GTK_STATE_SELECTED] = green;

    label2 = gtk_label_new ("Conditions");
    gtk_widget_ref (label2);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "label2", label2, (GtkDestroyNotify) gtk_widget_unref);
    if (circle->conditions != "") gtk_widget_set_style (label2, style);
    gtk_widget_show (label2);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label2);

    scrolledwindow5 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow5);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "scrolledwindow5", scrolledwindow5, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow5);
    gtk_container_add (GTK_CONTAINER (notebook1), scrolledwindow5);
    gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow5), 4);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow5), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    variable_entry = gtk_text_new (NULL, NULL);
    gtk_widget_ref (variable_entry);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "variable_entry", variable_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (variable_entry);
    gtk_container_add (GTK_CONTAINER (scrolledwindow5), variable_entry);
    GTK_WIDGET_SET_FLAGS (variable_entry, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, variable_entry, "Here you can set or change variables for use in this and following dialogues as well as other game modules.", NULL);
    gtk_text_set_editable (GTK_TEXT (variable_entry), TRUE);
    gtk_text_insert (GTK_TEXT (variable_entry), variable_entry->style->font,
        &variable_entry->style->black, &variable_entry->style->white,
        circle->variables.c_str (), -1);

    label3 = gtk_label_new ("Variables");
    gtk_widget_ref (label3);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "label3", label3, (GtkDestroyNotify) gtk_widget_unref);
    if (circle->variables != "") gtk_widget_set_style (label3, style);
    gtk_widget_show (label3);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), label3);

    label6 = gtk_label_new ("Unused");
    gtk_widget_ref (label6);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "label6", label6, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label6);
    gtk_container_add (GTK_CONTAINER (notebook1), label6);

    label4 = gtk_label_new ("Misc. Commands");
    gtk_widget_ref (label4);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "label4", label4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label4);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 3), label4);

    frame1 = gtk_frame_new ("Annotations:");
    gtk_widget_ref (frame1);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "frame1", frame1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame1);
    gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 4);
    gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_OUT);

    scrolledwindow4 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow4);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "scrolledwindow4", scrolledwindow4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow4);
    gtk_container_add (GTK_CONTAINER (frame1), scrolledwindow4);
    gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow4), 4);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    annotation_entry = gtk_text_new (NULL, NULL);
    gtk_widget_ref (annotation_entry);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "annotation_entry", annotation_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (annotation_entry);
    gtk_container_add (GTK_CONTAINER (scrolledwindow4), annotation_entry);
    gtk_tooltips_set_tip (tooltips, annotation_entry, "Here is room for comments and other reminders", NULL);
    gtk_text_set_editable (GTK_TEXT (annotation_entry), TRUE);
    gtk_text_insert (GTK_TEXT (annotation_entry), annotation_entry->style->font,
        &annotation_entry->style->black, &annotation_entry->style->white,
        circle->comment.c_str (), -1);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    ok_button = gtk_button_new_with_label ("OK");
    gtk_widget_ref (ok_button);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "ok_button", ok_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ok_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), ok_button);
    GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

    cancel_button = gtk_button_new_with_label ("Cancel");
    gtk_widget_ref (cancel_button);
    gtk_object_set_data_full (GTK_OBJECT (dlg_node_window), "cancel_button", cancel_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (cancel_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), cancel_button);
    GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

    gtk_signal_connect (GTK_OBJECT (notebook1), "switch_page", GTK_SIGNAL_FUNC (on_switch_page), dlg);
    gtk_signal_connect (GTK_OBJECT (text_entry), "changed", GTK_SIGNAL_FUNC (on_text_entry_changed), dlg);
    gtk_signal_connect (GTK_OBJECT (player_button), "pressed", GTK_SIGNAL_FUNC (on_player_button_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (npc_button), "pressed", GTK_SIGNAL_FUNC (on_npc_button_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (condition_entry), "changed", GTK_SIGNAL_FUNC (on_condition_entry_changed), dlg);
    gtk_signal_connect (GTK_OBJECT (variable_entry), "changed", GTK_SIGNAL_FUNC (on_variable_entry_changed), dlg);
    gtk_signal_connect (GTK_OBJECT (annotation_entry), "changed", GTK_SIGNAL_FUNC (on_annotation_entry_changed), dlg);
    gtk_signal_connect (GTK_OBJECT (ok_button), "pressed", GTK_SIGNAL_FUNC (on_circle_ok_button_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (cancel_button), "pressed", GTK_SIGNAL_FUNC (on_circle_cancel_button_pressed), dlg_node_window);
    gtk_signal_connect (GTK_OBJECT (dlg_node_window), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);

    gtk_widget_grab_focus (text_entry);
    gtk_widget_grab_default (text_entry);
    gtk_object_set_data (GTK_OBJECT (dlg_node_window), "tooltips", tooltips);

    // Show window
    gtk_widget_show (dlg_node_window);

    return dlg_node_window;
}
