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

#include <map>
#include <string.h>
#include <iostream.h>
#include <gtk/gtk.h>

#include "../../types.h"
#include "dlgnode.h"
#include "function.h"
#include "callbacks.h"

// static variables
char *function::fct_string[] = { "LET", "IF" };
char *function::op_string[] = { "=", "+=", "-=", "==", "!=", "<", ">" };
map<char*, s_int32, ltstr> function::vars;

// Constructor
function::function (DlgNode *n) : node (n), number (-1), thefunction (IF)
{
    create_function_dialog ();
    select (-1);
    set_variables ();
    gtk_widget_show (function_dialog);
}

// Destructor
function::~function ()
{
    gtk_widget_destroy (function_dialog);
}

// Add a new function to node
void function::add ()
{
    map<char*, s_int32, ltstr>::iterator i;
    function_data *data = new function_data;
    char *entry[4];

    // add data to node
    data->function = thefunction;
    data->operation = theoperation;
    data->variable = g_strdup (gtk_entry_get_text ((GtkEntry *) GTK_COMBO (variable)->entry));
    data->value = g_strdup (gtk_entry_get_text ((GtkEntry *) GTK_COMBO (value)->entry));

    // add function to list
    entry[0] = function::fct_string[data->function];
    entry[1] = data->variable;
    entry[2] = function::op_string[data->operation];
    entry[3] = data->value;
    gtk_clist_append ((GtkCList*) function_list, entry);

    // add variable to list of variables
    i = vars.find (data->variable);
    if (i != vars.end ())
    {
        vars[data->variable]++;
    }
    else
    {
        vars[data->variable] = 1;

        // set new variable list
        set_variables ();
    }
    
    number++;
}

// Remove selected item from function-list
void function::remove ()
{
    gchar *text;

    // nothing selected (should not happen, but the extra check doesn't hurt either)
    if (selection == -1) return;

    // get the selected variable
    gtk_clist_get_text (GTK_CLIST (function_list), selection, 1, &text);

    // update count of variable and remove from combobox if count == 0
    if ((--vars[text]) == 0)
    {
        vars.erase (text);
        set_variables ();
    }
        
    // Remove function from list
    gtk_clist_remove (GTK_CLIST (function_list), selection);
    select (-1);
    number--;
}

// move function up in function list
void function::up ()
{
    if (selection > 0)
    {
        gtk_clist_swap_rows (GTK_CLIST (function_list), selection, selection-1);
        selection -= 1;
    }
}

// move function down in function list
void function::down ()
{
    if (selection < number)
    {
        gtk_clist_swap_rows (GTK_CLIST (function_list), selection, selection+1);
        selection += 1;
    }
}

// Select function in function-list
void function::select (s_int32 row)
{
    selection = row;

    // Nothing selected -> deactivate som buttons
    if (row < 0)
    {
        gtk_widget_set_sensitive (remove_button, FALSE);
        gtk_widget_set_sensitive (up_button, FALSE);
        gtk_widget_set_sensitive (down_button, FALSE); 
    }

    // A row selected -> activate the buttons again
    else
    {
        gtk_widget_set_sensitive (remove_button, TRUE);
        gtk_widget_set_sensitive (up_button, TRUE);
        gtk_widget_set_sensitive (down_button, TRUE);  
    }
}

// Set the variables that appear in the combobox
void function::set_variables ()
{
    GList *list = NULL;
    map<char*, s_int32, ltstr>::iterator i;

    // get used variables out of 'vars'
    for (i = vars.begin (); i != vars.end (); i++)
        list = g_list_append (list, (*i).first);

    if (list)
    {
        // add them to both comboboxes
        gtk_combo_set_popdown_strings ((GtkCombo *) variable, list);
        gtk_combo_set_popdown_strings ((GtkCombo *) value, list);

        // avoid that first popdown-string appears in entry
        gtk_entry_set_text ((GtkEntry *) (GTK_COMBO (variable)->entry), "");
        gtk_entry_set_text ((GtkEntry *) (GTK_COMBO (value)->entry), "");
    }
    else
    {
        gtk_list_clear_items ((GtkList *) (GTK_COMBO (variable)->list), 0, -1);
        gtk_list_clear_items ((GtkList *) (GTK_COMBO (value)->list), 0, -1);
    }
}

// Change the function
void function::set_function ()
{
    GtkWidget *menu = gtk_option_menu_get_menu ((GtkOptionMenu *) function);
    GtkWidget *menu_item = gtk_menu_get_active ((GtkMenu *) menu);
    thefunction = GPOINTER_TO_INT(gtk_object_get_user_data (GTK_OBJECT (menu_item)));

    cout << "fct " << thefunction << "\n"; 
    set_operators ();
}

// Change the operation
void function::set_operation ()
{
    GtkWidget *menu = gtk_option_menu_get_menu ((GtkOptionMenu *) operation);
    GtkWidget *menu_item = gtk_menu_get_active ((GtkMenu *) menu);
    theoperation = GPOINTER_TO_INT(gtk_object_get_user_data (GTK_OBJECT (menu_item)));

    cout << "op  " << theoperation << "\n"; 
}


// Change the accessible operations
void function::set_operators ()
{
    GtkWidget *menuitem;
    GtkWidget *menu;
    
    switch (thefunction)
    {
        // arithmetics
        case LET:
        {
            menu = gtk_menu_new ();

            // assign
            menuitem = gtk_menu_item_new_with_label ("=");
            gtk_widget_show (menuitem);
            gtk_menu_append (GTK_MENU (menu), menuitem);
            gtk_object_set_user_data (GTK_OBJECT (menuitem), (gpointer) GINT_TO_POINTER(ASSIGN));

            // add
            menuitem = gtk_menu_item_new_with_label ("+=");
            gtk_widget_show (menuitem);
            gtk_menu_append (GTK_MENU (menu), menuitem);
            gtk_object_set_user_data (GTK_OBJECT (menuitem), (gpointer) GINT_TO_POINTER(ADD));

            // sub
            menuitem = gtk_menu_item_new_with_label ("-=");
            gtk_widget_show (menuitem);
            gtk_menu_append (GTK_MENU (menu), menuitem);
            gtk_object_set_user_data (GTK_OBJECT (menuitem), (gpointer) GINT_TO_POINTER(SUB));

            theoperation = ASSIGN;
            break;
        }

        // comparisons
        case IF:
        {
            menu = gtk_menu_new ();

            // equal
            menuitem = gtk_menu_item_new_with_label ("==");
            gtk_widget_show (menuitem);
            gtk_menu_append (GTK_MENU (menu), menuitem);
            gtk_object_set_user_data (GTK_OBJECT (menuitem), (gpointer) GINT_TO_POINTER(EQ));

            // not equal
            menuitem = gtk_menu_item_new_with_label ("!=");
            gtk_widget_show (menuitem);
            gtk_menu_append (GTK_MENU (menu), menuitem);
            gtk_object_set_user_data (GTK_OBJECT (menuitem), (gpointer) GINT_TO_POINTER(NEQ));

            // less than
            menuitem = gtk_menu_item_new_with_label ("<");
            gtk_widget_show (menuitem);
            gtk_menu_append (GTK_MENU (menu), menuitem);
            gtk_object_set_user_data (GTK_OBJECT (menuitem), (gpointer) GINT_TO_POINTER(LT));

            // greater than
            menuitem = gtk_menu_item_new_with_label (">");
            gtk_widget_show (menuitem);
            gtk_menu_append (GTK_MENU (menu), menuitem);
            gtk_object_set_user_data (GTK_OBJECT (menuitem), (gpointer) GINT_TO_POINTER(GT));
            
            theoperation = EQ;
            break;
        }
        default: return;
    }

    gtk_option_menu_remove_menu ((GtkOptionMenu *) operation);
    gtk_option_menu_set_menu ((GtkOptionMenu *) operation, menu);
}

// Create the GUI
void function::create_function_dialog ()
{
    GtkWidget *vbox1;
    GtkWidget *hbox1;
    GtkWidget *function;
    GtkWidget *function_menu;
    GtkWidget *glade_menuitem;
    GtkWidget *add_button;
    GtkWidget *variable_entry;
    GtkWidget *value_entry;
    GtkWidget *scrolledwindow1;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *hbuttonbox2;
    GtkWidget *hseparator1;
    GtkWidget *hbuttonbox1;
    GtkWidget *fct_ok_buttpn;
    GtkWidget *fct_cancel_button;

    // Dialogbox
    function_dialog = gtk_window_new (GTK_WINDOW_DIALOG);
    gtk_object_set_data (GTK_OBJECT (function_dialog), "function_dialog", function_dialog);
    gtk_widget_set_usize (function_dialog, 420, 200);
    gtk_container_set_border_width (GTK_CONTAINER (function_dialog), 4);
    gtk_window_set_title (GTK_WINDOW (function_dialog), "Functions");
    gtk_window_set_position (GTK_WINDOW (function_dialog), GTK_WIN_POS_MOUSE);
    gtk_window_set_modal (GTK_WINDOW (function_dialog), TRUE);
    gtk_window_set_policy (GTK_WINDOW (function_dialog), FALSE, FALSE, FALSE);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_ref (vbox1);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "vbox1", vbox1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (vbox1);
    gtk_container_add (GTK_CONTAINER (function_dialog), vbox1);

    hbox1 = gtk_hbox_new (FALSE, 7);
    gtk_widget_ref (hbox1);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "hbox1", hbox1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);

    // Function - selection (with LET and IF function)
    function = gtk_option_menu_new ();
    gtk_widget_ref (function);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "function", function,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (function);
    gtk_box_pack_start (GTK_BOX (hbox1), function, FALSE, FALSE, 0);
    function_menu = gtk_menu_new ();
    glade_menuitem = gtk_menu_item_new_with_label ("LET");
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), (gpointer) GINT_TO_POINTER(LET));
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (function_menu), glade_menuitem);
    glade_menuitem = gtk_menu_item_new_with_label ("IF");
    gtk_object_set_user_data (GTK_OBJECT (glade_menuitem), (gpointer) GINT_TO_POINTER(IF));
    gtk_widget_show (glade_menuitem);
    gtk_menu_append (GTK_MENU (function_menu), glade_menuitem);
    gtk_option_menu_set_menu (GTK_OPTION_MENU (function), function_menu);
    gtk_option_menu_set_history (GTK_OPTION_MENU (function), thefunction);

    // Variable selection (either chose out of the list or add a new one)
    variable = gtk_combo_new ();
    gtk_widget_ref (variable);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "variable", variable,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (variable);
    gtk_box_pack_start (GTK_BOX (hbox1), variable, FALSE, FALSE, 0);

    variable_entry = GTK_COMBO (variable)->entry;
    gtk_widget_ref (variable_entry);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "variable_entry", variable_entry,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (variable_entry);
    gtk_widget_set_usize (variable_entry, 100, -2);

    // Operator selection (depends of selected function)
    operation = gtk_option_menu_new ();
    gtk_widget_ref (operation);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "operation", operation,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_box_pack_start (GTK_BOX (hbox1), operation, FALSE, FALSE, 0);
    gtk_widget_show (operation);

    set_operators ();

    // Value selection (chose variable out of list or enter digit)
    value = gtk_combo_new ();
    gtk_widget_ref (value);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "value", value,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (value);
    gtk_box_pack_start (GTK_BOX (hbox1), value, TRUE, TRUE, 0);

    value_entry = GTK_COMBO (value)->entry;
    gtk_widget_ref (value_entry);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "value_entry", value_entry,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (value_entry);

    // List container
    scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_ref (scrolledwindow1);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "scrolledwindow1", scrolledwindow1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (scrolledwindow1);
    gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

    // List of already assigned functions
    function_list = gtk_clist_new (4);
    gtk_widget_ref (function_list);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "function_list", function_list,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (function_list);
    gtk_container_add (GTK_CONTAINER (scrolledwindow1), function_list);
    gtk_clist_set_column_width (GTK_CLIST (function_list), 0, 65);
    gtk_clist_set_column_width (GTK_CLIST (function_list), 1, 110);
    gtk_clist_set_column_justification (GTK_CLIST (function_list), 2, GTK_JUSTIFY_CENTER);
    gtk_clist_set_column_width (GTK_CLIST (function_list), 2, 65);
    gtk_clist_set_column_width (GTK_CLIST (function_list), 3, 110);
    gtk_clist_column_titles_show (GTK_CLIST (function_list));
    
    label1 = gtk_label_new ("Function");
    gtk_widget_ref (label1);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "label1", label1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label1);
    gtk_clist_set_column_widget (GTK_CLIST (function_list), 0, label1);

    label2 = gtk_label_new ("Variable");
    gtk_widget_ref (label2);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "label2", label2,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label2);
    gtk_clist_set_column_widget (GTK_CLIST (function_list), 1, label2);

    label3 = gtk_label_new ("Operation");
    gtk_widget_ref (label3);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "label3", label3,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label3);
    gtk_clist_set_column_widget (GTK_CLIST (function_list), 2, label3);

    label4 = gtk_label_new ("Value");
    gtk_widget_ref (label4);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "label4", label4,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label4);
    gtk_clist_set_column_widget (GTK_CLIST (function_list), 3, label4);

    // Labels shouldn't be clickable as they serve no function anyway
    gtk_clist_column_titles_passive (GTK_CLIST (function_list));

    // Buttons for list maipulation
    hbuttonbox2 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox2);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "hbuttonbox2", hbuttonbox2,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox2);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox2, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox2), GTK_BUTTONBOX_START);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox2), 0);

    // Add to list
    add_button = gtk_button_new_with_label ("Add");
    gtk_widget_ref (add_button);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "add_button", add_button,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (add_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox2), add_button);
    GTK_WIDGET_SET_FLAGS (add_button, GTK_CAN_DEFAULT);

    // Remove from list
    remove_button = gtk_button_new_with_label ("Remove");
    gtk_widget_ref (remove_button);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "remove_button", remove_button,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (remove_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox2), remove_button);
    GTK_WIDGET_SET_FLAGS (remove_button, GTK_CAN_DEFAULT);

    // Move up in list
    up_button = gtk_button_new_with_label ("Up");
    gtk_widget_ref (up_button);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "up_button", up_button,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (up_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox2), up_button);
    GTK_WIDGET_SET_FLAGS (up_button, GTK_CAN_DEFAULT);

    // Move down in list
    down_button = gtk_button_new_with_label ("Down");
    gtk_widget_ref (down_button);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "down_button", down_button,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (down_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox2), down_button);
    GTK_WIDGET_SET_FLAGS (down_button, GTK_CAN_DEFAULT);

    hseparator1 = gtk_hseparator_new ();
    gtk_widget_ref (hseparator1);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "hseparator1", hseparator1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hseparator1);
    gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, FALSE, TRUE, 2);

    // OK and Cancel Buttons
    hbuttonbox1 = gtk_hbutton_box_new ();
    gtk_widget_ref (hbuttonbox1);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "hbuttonbox1", hbuttonbox1,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (hbuttonbox1);
    gtk_box_pack_start (GTK_BOX (vbox1), hbuttonbox1, FALSE, TRUE, 0);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
    gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox1), 0);

    fct_ok_buttpn = gtk_button_new_with_label ("OK");
    gtk_widget_ref (fct_ok_buttpn);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "fct_ok_buttpn", fct_ok_buttpn,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (fct_ok_buttpn);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), fct_ok_buttpn);
    GTK_WIDGET_SET_FLAGS (fct_ok_buttpn, GTK_CAN_DEFAULT);

    fct_cancel_button = gtk_button_new_with_label ("Cancel");
    gtk_widget_ref (fct_cancel_button);
    gtk_object_set_data_full (GTK_OBJECT (function_dialog), "fct_cancel_button", fct_cancel_button,
        (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (fct_cancel_button);
    gtk_container_add (GTK_CONTAINER (hbuttonbox1), fct_cancel_button);
    GTK_WIDGET_SET_FLAGS (fct_cancel_button, GTK_CAN_DEFAULT);

    // Lots o´ callbacks
    gtk_signal_connect (GTK_OBJECT (function_dialog), "delete_event",
        GTK_SIGNAL_FUNC (on_widget_destroy), (gpointer) NULL);
    gtk_signal_connect (GTK_OBJECT (add_button), "clicked",
        GTK_SIGNAL_FUNC (on_add_button_clicked), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (remove_button), "clicked",
        GTK_SIGNAL_FUNC (on_remove_button_clicked), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (up_button), "clicked",
        GTK_SIGNAL_FUNC (on_up_button_clicked), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (down_button), "clicked",
        GTK_SIGNAL_FUNC (on_down_button_clicked), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (fct_ok_buttpn), "clicked",
        GTK_SIGNAL_FUNC (on_fct_ok_buttpn_clicked), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (fct_cancel_button), "clicked",
        GTK_SIGNAL_FUNC (on_fct_cancel_button_clicked), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (function_list), "select_row",
        GTK_SIGNAL_FUNC (on_fct_select_row), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (function_list), "unselect_row",
        GTK_SIGNAL_FUNC (on_fct_unselect_row), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (function), "released",
        GTK_SIGNAL_FUNC (on_function_released), (gpointer) this);
    gtk_signal_connect (GTK_OBJECT (operation), "released",
        GTK_SIGNAL_FUNC (on_operation_released), (gpointer) this);
}
