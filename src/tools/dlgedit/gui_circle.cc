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

#include <gtk/gtk.h>
#include "gui_circle.h"

/**
 * @file gui_circle.h
 *
 * @author Kai Sterker
 * @brief The Edit Circle dialog
 */

GuiCircle::GuiCircle (node_type type, DlgCircleEntry *e)
{
    entry = e;
    
    GtkWidget *vbox1;
    GtkWidget *notebook1;
    GtkWidget *vbox2;
    GtkWidget *scrolledwindow1;
    GtkWidget *label;
    
    // the different text entries
    GtkWidget *text_entry;
    GtkWidget *annotation_entry;
    GtkEditable *condition_entry;
    GtkEditable *variable_entry;
    
    GSList *character_group = NULL;
    GtkWidget *player_button;
    GtkWidget *narrator_button;
    GtkWidget *hbox1;
    GtkWidget *npc_button;
    GtkWidget *npc_selection_menu;
    GtkWidget *glade_menuitem;
    GtkWidget *npc_mood;
    GtkWidget *npc_mood_menu;
    GtkWidget *frame1;
    GtkWidget *scrolledwindow4;
    GtkWidget *hbuttonbox1;
    GtkWidget *ok_button;
    GtkWidget *cancel_button;

    // color to use for Player and Narrator text and labels    
    GdkColor dark_blue  = { 0, 0, 0, 35000 };
    GdkColor green      = { 0, 0, 27300, 15600 };
    GdkColor dark_green = { 0, 0, 27500, 0 };
    
    int position = 0;
    
    // Tooltips for the dialog
    GtkTooltips *tooltips = gtk_tooltips_new ();

    GtkStyle *style = NULL;
     
    // the "Edit Dialogue Node" dialog
    dialog = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (dialog), "dialog", dialog);
    gtk_widget_set_usize (dialog, 400, 320);
    gtk_window_set_title (GTK_WINDOW (dialog), "Edit Dialogue Node");
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
    gtk_window_set_policy (GTK_WINDOW (dialog), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "vbox1", vbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (dialog), vbox1);

    // the GtkNotebook with the different 'pages' of the dialog
    notebook1 = gtk_notebook_new ();
    gtk_widget_ref (notebook1);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "notebook1", notebook1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (notebook1);
    gtk_box_pack_start (GTK_BOX (vbox1), notebook1, TRUE, TRUE, 0);
    gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook1), FALSE);
    gtk_notebook_set_tab_hborder (GTK_NOTEBOOK (notebook1), 6);
    GTK_WIDGET_UNSET_FLAGS (notebook1, GTK_CAN_FOCUS);

    // Page 1: Edit Text
    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox2);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "vbox2", vbox2, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox2);
    gtk_container_add (GTK_CONTAINER (notebook1), vbox2);
    gtk_container_set_border_width (GTK_CONTAINER (vbox2), 4);

    // Scrollbar for the text entry
    scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "scrolledwindow1", scrolledwindow1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_box_pack_start (GTK_BOX (vbox2), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    // the text entry
    text_entry = gtk_text_new (NULL, NULL);
    gtk_widget_ref (text_entry);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "text_entry", text_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (text_entry);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), text_entry);
    GTK_WIDGET_SET_FLAGS (text_entry, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips, text_entry, "Enter the Text to appear within the conversation", NULL);
    gtk_text_set_editable (GTK_TEXT (text_entry), TRUE);
    gtk_text_set_word_wrap (GTK_TEXT (text_entry), TRUE);
    
    // set the textcolor of the entry according to the given type
    if (type == NPC)
        gtk_text_insert (GTK_TEXT (text_entry), text_entry->style->font,
            &text_entry->style->black, &text_entry->style->white,
            e->text ().c_str (), -1);
    else if (type == NARRATOR)
    {
        gtk_text_insert ((GtkText *) text_entry, text_entry->style->font,
            &dark_green, &text_entry->style->white, " ", -1);
        gtk_text_insert (GTK_TEXT (text_entry), text_entry->style->font,
            &dark_green, &text_entry->style->white, e->text ().c_str (), -1);
        gtk_editable_delete_text ((GtkEditable *) text_entry, 0, 1);
    }
    else
    {
        gtk_text_insert ((GtkText *) text_entry, text_entry->style->font,
            &dark_blue, &text_entry->style->white, " ", -1);
        gtk_text_insert (GTK_TEXT (text_entry), text_entry->style->font,
            &dark_blue, &text_entry->style->white, e->text ().c_str (), -1);
        gtk_editable_delete_text ((GtkEditable *) text_entry, 0, 1);
    }
    
    // The radiobuttons to chose the text-type from
    label = gtk_label_new ("Narrator");
    gtk_label_set_justify ((GtkLabel *) label, GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
    gtk_widget_show (label);

    // make the narrator-radiobutton-text green
    style = gtk_style_copy (gtk_widget_get_default_style ());
    style->fg[GTK_STATE_NORMAL] = dark_green;
    style->fg[GTK_STATE_ACTIVE] = dark_green;
    style->fg[GTK_STATE_PRELIGHT] = dark_green;
    style->fg[GTK_STATE_SELECTED] = dark_green;
    gtk_widget_set_style (label, style);

    narrator_button = gtk_radio_button_new (character_group);
    gtk_container_add (GTK_CONTAINER(narrator_button), label);
    character_group = gtk_radio_button_group (GTK_RADIO_BUTTON (narrator_button));
    gtk_widget_ref (narrator_button);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "narrator_button", narrator_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (narrator_button);
    gtk_box_pack_start (GTK_BOX (vbox2), narrator_button, FALSE, FALSE, 0);
    GTK_WIDGET_UNSET_FLAGS (narrator_button, GTK_CAN_FOCUS);
    
    // activate the narrator button if neccessary
    if (type == NARRATOR)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (narrator_button), TRUE);

    // the player button
    label = gtk_label_new ("Player");
    gtk_label_set_justify ((GtkLabel *) label, GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
    gtk_widget_show (label);

    // make the player-radiobutton-text blue
    style = gtk_style_copy (gtk_widget_get_default_style ());
    style->fg[GTK_STATE_NORMAL] = dark_blue;
    style->fg[GTK_STATE_ACTIVE] = dark_blue;
    style->fg[GTK_STATE_PRELIGHT] = dark_blue;
    style->fg[GTK_STATE_SELECTED] = dark_blue;
    gtk_widget_set_style (label, style);

    player_button = gtk_radio_button_new (character_group);
    gtk_container_add (GTK_CONTAINER(player_button), label);
    character_group = gtk_radio_button_group (GTK_RADIO_BUTTON (player_button));
    gtk_widget_ref (player_button);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "player_button", player_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (player_button);
    gtk_box_pack_start (GTK_BOX (vbox2), player_button, FALSE, FALSE, 0);
    GTK_WIDGET_UNSET_FLAGS (player_button, GTK_CAN_FOCUS);

    // activate the player button if neccessary
    if (type == PLAYER)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (player_button), TRUE);

    hbox1 = gtk_hbox_new (FALSE, 10);
    gtk_widget_ref (hbox1);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "hbox1", hbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, TRUE, 0);

    // the NPC radiobutton
    npc_button = gtk_radio_button_new_with_label (character_group, "NPC:");
    character_group = gtk_radio_button_group (GTK_RADIO_BUTTON (npc_button));
    gtk_widget_ref (npc_button);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "npc_button", npc_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (npc_button);
    gtk_box_pack_start (GTK_BOX (hbox1), npc_button, FALSE, FALSE, 0);
    GTK_WIDGET_UNSET_FLAGS (npc_button, GTK_CAN_FOCUS);

    // activate the NPC button if neccessary
    if (type == NPC)
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (npc_button), TRUE);

    // the option menu with the available NPC's
    npc_selection = gtk_option_menu_new ();
    gtk_widget_ref (npc_selection);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "npc_selection", npc_selection, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (npc_selection);
    gtk_box_pack_start (GTK_BOX (hbox1), npc_selection, TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips, npc_selection, "Select the speaker", NULL);
    npc_selection_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label ("default");
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), NULL);
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (npc_selection_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (npc_selection), npc_selection_menu);

    /*
    dictionary <character *>::iterator itc; 
    for (itc = data::characters.begin (); itc != data::characters.end () && itc->second != NULL; itc++) 
        //     while ((mychar = (character *) data::characters.next ()) != NULL)
    {
        // don't add the player character to the list
        if (!strcmp (itc->second->get_name().c_str (), ((character *) data::the_player)->get_name().c_str ())) continue;
        
        glade_menuitem = gtk_menu_item_new_with_label (itc->second->get_name().c_str ());
        gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), (void *) itc->second->get_name().c_str ());
        gtk_widget_show (glade_menuitem);
        gtk_menu_append (GTK_MENU (npc_selection_menu), glade_menuitem);
        gtk_option_menu_set_menu (GTK_OPTION_MENU (npc_selection), npc_selection_menu);
    }
    
    if (circle->character != "") 
        set_option (GTK_OPTION_MENU (npc_selection), circle->character.c_str());
    */
    
    // the option menu with the available moods
    label = gtk_label_new ("Mood: ");
    gtk_widget_ref (label);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "label", label, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (hbox1), label, TRUE, FALSE, 0);
    gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_RIGHT);
    gtk_misc_set_alignment (GTK_MISC (label), 0.01, 0.5);

    npc_mood = gtk_option_menu_new ();
    gtk_widget_ref (npc_mood);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "npc_mood", npc_mood, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (npc_mood);
    gtk_box_pack_start (GTK_BOX (hbox1), npc_mood, TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips, npc_mood, "How the speaker's image appears to the player", NULL);
    npc_mood_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label ("default");
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (npc_mood_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (npc_mood), npc_mood_menu);

    // The heading of this page
    label = gtk_label_new ("Text");
    gtk_widget_ref (label);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "label", label, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label);

    // the entry for annotations
    frame1 = gtk_frame_new ("Annotations:");
    gtk_widget_ref (frame1);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "frame1", frame1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (frame1);
    gtk_box_pack_start (GTK_BOX (vbox2), frame1, TRUE, TRUE, 0);

    scrolledwindow4 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow4);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "scrolledwindow4", scrolledwindow4, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow4);
    gtk_container_add (GTK_CONTAINER (frame1), scrolledwindow4);
    gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow4), 4);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow4), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    annotation_entry = gtk_text_new (NULL, NULL);
    gtk_widget_ref (annotation_entry);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "annotation_entry", annotation_entry, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (annotation_entry);
    gtk_container_add (GTK_CONTAINER (scrolledwindow4), annotation_entry);
    gtk_tooltips_set_tip (tooltips, annotation_entry, "Here is room for comments and other reminders", NULL);
    gtk_text_set_editable (GTK_TEXT (annotation_entry), TRUE);
    gtk_text_insert (GTK_TEXT (annotation_entry), annotation_entry->style->font,
        &annotation_entry->style->black, &annotation_entry->style->white,
        e->annotation ().c_str (), -1);

    // Condition-editor
    cond_edit = new GuiEdit (notebook1);
    condition_entry = GTK_EDITABLE (cond_edit->widget ());
    
    // append tooltips
    gtk_tooltips_set_tip (tooltips, GTK_WIDGET (condition_entry), 
        "Enter the conditions that have to be met before this node is available in the dialogue", NULL);

    // display the current conditios
    gtk_editable_insert_text (condition_entry, 
        entry->condition ().c_str (), entry->condition ().length (), &position);
    
    style = gtk_style_copy (gtk_widget_get_default_style ());
    style->fg[GTK_STATE_NORMAL] = green;
    style->fg[GTK_STATE_ACTIVE] = green;
    style->fg[GTK_STATE_PRELIGHT] = green;
    style->fg[GTK_STATE_SELECTED] = green;

    label = gtk_label_new ("Conditions");
    gtk_widget_ref (label);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "label", label, (GtkDestroyNotify) gtk_widget_unref);
    if (e->condition () != "") gtk_widget_set_style (label, style);
    gtk_widget_show (label);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label);

    // Code-editor
    code_edit = new GuiEdit (notebook1);
    variable_entry =  GTK_EDITABLE (code_edit->widget ());
    
    // append tooltips
    gtk_tooltips_set_tip (tooltips, GTK_WIDGET (variable_entry), 
        "Here you can set or change variables for use in this and following dialogues as well as other game modules.", NULL);
            
    // display the current conditios
    gtk_editable_insert_text (variable_entry, 
        entry->code ().c_str (), entry->code ().length (), &position);
    
    label = gtk_label_new ("Variables");
    gtk_widget_ref (label);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "label", label, (GtkDestroyNotify) gtk_widget_unref);
    if (e->code () != "") gtk_widget_set_style (label, style);
    gtk_widget_show (label);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), label);

    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "hbuttonbox1", hbuttonbox1, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    // ok button
    ok_button = gtk_button_new_with_label ("OK");
    gtk_widget_ref (ok_button);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "ok_button", ok_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (ok_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), ok_button);
    GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

    // cancel button
    cancel_button = gtk_button_new_with_label ("Cancel");
    gtk_widget_ref (cancel_button);
    gtk_object_set_data_full (GTK_OBJECT (dialog), "cancel_button", cancel_button, (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (cancel_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), cancel_button);
    GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

    // the various input handlers
    /*
    gtk_signal_connect (GTK_OBJECT (notebook1), "switch_page", GTK_SIGNAL_FUNC (on_switch_page), dlg);
    gtk_signal_connect (GTK_OBJECT (text_entry), "changed", GTK_SIGNAL_FUNC (on_text_entry_changed), dlg);
    gtk_signal_connect (GTK_OBJECT (player_button), "clicked", GTK_SIGNAL_FUNC (on_player_button_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (npc_button), "clicked", GTK_SIGNAL_FUNC (on_npc_button_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (narrator_button), "clicked", GTK_SIGNAL_FUNC (on_narrator_button_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (condition_entry), "changed", GTK_SIGNAL_FUNC (on_condition_entry_changed), dlg);
    gtk_signal_connect (GTK_OBJECT (variable_entry), "changed", GTK_SIGNAL_FUNC (on_variable_entry_changed), dlg);
    gtk_signal_connect (GTK_OBJECT (annotation_entry), "changed", GTK_SIGNAL_FUNC (on_annotation_entry_changed), dlg);
    gtk_signal_connect (GTK_OBJECT (ok_button), "clicked", GTK_SIGNAL_FUNC (on_circle_ok_button_pressed), dlg);
    gtk_signal_connect (GTK_OBJECT (cancel_button), "clicked", GTK_SIGNAL_FUNC (on_circle_cancel_button_pressed), dialog);
    gtk_signal_connect (GTK_OBJECT (dialog), "delete_event", GTK_SIGNAL_FUNC (on_widget_destroy), NULL);
    */
    
    // give focus to the text entry
    gtk_widget_grab_focus (text_entry);
    gtk_widget_grab_default (text_entry);
    gtk_object_set_data (GTK_OBJECT (dialog), "tooltips", tooltips);

    // show window
    gtk_widget_show (dialog);
}
