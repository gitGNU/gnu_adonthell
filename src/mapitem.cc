/*
   $Id$

   Copyright (C) 1999 Raphael Alla.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.
   See the COPYING file for more details.

 */

/*
   load_item
   read the text file specified as an argument
   keyword have to start the line
   see items.txt for authorized syntax of the text file
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "mapevent.h"
#include "mapitem.h"
#include "instruction.h"

mapitem::mapitem()
{
	number=0;
	pattern=0;
	event_walk=0;
	event_action=0;
	*name='\0';
	status=0;
}


int loaditems (char *filename, mapitem ** item_ptr, mapevent ** event_ptr, u_int16 * event_nb)
// returns the number of items which have been loaded in memory
{
    char string[100];           // contain lines read from the file

    char *word[20];             // an array which contains all the words of the string

    int wordc;                  // number of words in the string;

    int line = 0;
    char keyword;               // stores a value defining a keyword

    mapitem *items;                //stores the item to be read from the file

    mapevent *event = *event_ptr; //stores the events to be read from the file

    int current_item_nb;
    int current_event_nb;
    int nb_items;
    instruction* pi;

    int otherevent_flag = 0;    // equals 1 if the OTHEREVENT keyword applies for this event, 0 otherwise

    FILE *file;

    file = fopen (filename, "r");
    if (file == NULL)
    {
        printf ("Error opening the file %s\n", filename);
        exit (1);
    }

    line = 0;
    current_item_nb = 1;
    current_event_nb = *event_nb + 1;

    nb_items = allocate_item_memory (file, &items, &event, event_nb);

    // Load file into memory
    rewind (file);

    while (fgets (string, 100, file) != NULL)
    {
        ++line;
        if (test_line (string, word, &wordc) == 0)
            continue;

        if (strcasecmp (word[0], "ITEM") != 0)
            load_error ("ITEM keyword expected", line, string);

//        create_new_item (&items[current_item_nb]);

        items[current_item_nb].number = current_item_nb;
        strcpy (items[current_item_nb].name, word[1]);
        items[current_item_nb].event_walk = current_event_nb;

        //now reads specific item characterisitcs
        while (1)
        {
            fgets (string, 100, file);  // no end of file check as it should not happen

            ++line;

            if (test_line (string, word, &wordc) == 0)
                continue;

            keyword = *(strstr (KEYWORDS, word[0]) - 1);
            if (keyword == KEYWORD_END)
                break;          //end of this item

            if (keyword == KEYWORD_POS)
            {
                if (wordc != 3)
                    load_error_argc (line, string);
                items[current_item_nb].x = atoi (word[1]);
                items[current_item_nb].y = atoi (word[2]);
                continue;
            }

            if (keyword == KEYWORD_PATTERN)
            {
                if (wordc != 2)
                    load_error_argc (line, string);
                items[current_item_nb].pattern = atoi (word[1]);
                continue;
            }

            if (keyword == KEYWORD_OTHEREVENT)
            {
                if (wordc != 2)
                    load_error_argc (line, string);
                event[current_event_nb - 1].set_otherevent(atoi(word[1]));
                otherevent_flag = 1;
                continue;
            }

            if (keyword != KEYWORD_EVENT)
                load_error ("Keyword EVENT expected", line, string);

            create_new_event (&event[current_event_nb]);

            //reads event specificities
            switch (*(strstr (KEYWORDS_EVENTS, word[1]) - 1))
                    // switch according to the char just before the keyword in KEYWORDS
            {
                case KEYWORD_TELEPORT:
                    if (wordc != 4 && wordc != 6)
                        load_error_argc (line, string);
                        pi= new event_teleport;
                        pi->type = EVENT_TELEPORT;
                        pi->param_val[1] = atoi (word[2]);
                        pi->param_val[2] = atoi (word[3]);
                        pi->param_val[3] = 255;  // no change

                    if (wordc == 4)
                    {
                        pi->param_val[4] = event[current_event_nb].param[1];
                        pi->param_val[5] = event[current_event_nb].param[2];
                    }
                    else
                    {
                        pi->param_val[4] = atoi (word[4]);
                        pi->param_val[5] = atoi (word[5]);
                    }
                    pi->param_val[6] = 255;
                    pi->param_val[7] = 1;  // no change

                    break;
                case KEYWORD_MODIF_PICT:
                    pi->type=EVENT_MODIF_PICT;
                    break;
                case KEYWORD_GET:
                    pi->type = EVENT_GET;
                    break;
                case KEYWORD_GET_KEY:
                    pi->type= EVENT_GET_KEY;
                    break;
                case KEYWORD_WALK_FLAG:
                    pi->type= EVENT_WALK_FLAG;
                    break;
                case KEYWORD_OPEN:
                    pi->type= EVENT_OPEN;
                    break;
                case KEYWORD_PLACE_ITEM:
                    pi->type= EVENT_PLACE_ITEM;
                    break;
                case KEYWORD_FRAMEFACTOR:
                    pi->type= EVENT_FRAMEFACTOR;
                    pi->param_val[1] = atoi (word[2]);
                    break;
                default:
                    load_error ("Unknown keyword", line, string);
            }
    /*        if (otherevent_flag == 0)
                event[current_event_nb].otherevent = current_event_nb + 1;
            ++current_event_nb; */
            event[current_event_nb].push_back(pi);
        }
/*        if (otherevent_flag == 0)
            event[current_event_nb - 1].otherevent = 0;
        otherevent_flag = 0; */
        ++current_item_nb;
        ++current_event_nb;
    }

    *item_ptr = items;          // return the new values for the pointers

    *event_ptr = event;
    return (nb_items);
}

int test_line (char *string, char **word, int *wordc)
// returns 0 if line is a comment line or if line is empty
// otherwise divides the line in wordc words stored in the word array
{
    if (string[0] == '#')
        return (0);             // comment line

    linesep (string, word, wordc);
    if (*wordc == 0)
        return (0);             //empty line  

    return (1);                 // the line needs to be processed

}

int allocate_item_memory (FILE * file, item ** item_ptr, events ** event_ptr,
    u_int16 * total_nb_events)
// this function reads the file, and then allocates memory on the pointers
// item_ptr and events_ptr in order to be able to load into memory the items and
// events described by the file
// returns the number of items for which memory has been allocated
// total_nb_events is updated accordingly

{
    int line = 0;
    int nb_item = 0;
//    int nb_event = 0;
    int nb_enditem = 0;
    char string[100];
    char *word[20];             // an array which contains all the words of the string

    int wordc;                  // number of words in the string;


    //line=0;

    while (fgets (string, 100, file) != NULL)
    {
        line++;
        if (test_line (string, word, &wordc) == 0)
            continue;

        switch (*(strstr (KEYWORDS, word[0]) - 1))
                // switch according to the char just before the keyword in KEYWORDS
        {
            case KEYWORD_ITEM:
                ++nb_item;
                break;
            case KEYWORD_POS:
                break;
            case KEYWORD_EVENT:
//                ++nb_event;
                break;
            case KEYWORD_OTHEREVENT:
                break;
            case KEYWORD_PATTERN:
                break;
            case KEYWORD_END:
                ++nb_enditem;
                break;
            default:
                load_error ("Syntax error: %s unknown keyword\n", line, string);
        }
    }

    // Allocate memory

    *item_ptr = new mapitem[nb_item+1];
    *total_nb_events += nb_item;  // modif because now only one event per item
    *event_ptr = realloc (*event_ptr, sizeof (events) * (1 + *total_nb_events));
    if (*event_ptr == NULL || *item_ptr == NULL)
    {
        printf ("Error allocating memory\n");
        exit (1);
    }

    return (nb_item);
}

void create_new_event (events * new_event)
{
    //creates a new event
    new_event->type = 0;
    new_event->param1 = 0;
    new_event->param2 = 0;
    new_event->param3 = 0;
    new_event->param4 = 0;
    new_event->param5 = 0;
    new_event->param6 = 0;
    new_event->param7 = 0;
    new_event->param8 = 0;
    new_event->param9 = 0;
    *new_event->param10 = '\0';
    new_event->otherevent = 0;
}

void create_new_item (item * new_item)
{
//creates an empty item
    new_item->number = 0;       //current_item_nb;

    new_item->pattern = 0;
    new_item->x = 0;
    new_item->y = 0;
    new_item->event_walk = 0;
    new_item->event_action = 0;
    new_item->status = ITEM_OPENED;
    *new_item->name = '\0';
}

void load_error (char *Error_Message, int line, char *string)
{
    printf ("%s on line %i:\n%s", Error_Message, line, string);
    exit (1);
}

void load_error_argc (int line, char *string)
{
    load_error ("Bad number of arguments", line, string);
}
