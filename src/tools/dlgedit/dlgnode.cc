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

#include "dlgnode.h"

Circle::Circle ()
{
    text = "";
    comment = "";
    conditions = "";
    variables =	"";
}

Circle::Circle (u_int32 num, u_int8 tp, s_int32 x, s_int32 y)
{
    text = "";
    comment = "";
    conditions = "";
    variables =	"";

    number = num;
    type = tp;

    position.x = x - 10;
    position.y = y - 10;
    position.width = 21;
    position.height = 21;
}

// Save all data neccessary to restore the circle
void Circle::save (ofstream &file)
{
    // Keyword "Circle" and circle's number
    file << "\nCircle\n";

    // circle's type
    file << "  Type " << (int) type << "\n";
    
    // circle's position
    file << "  Pos " << position.x << " " << position.y << "\n";

    // circle's annotation
    if (comment != "") file << "  Note [" << comment << "]\n";

    // circle's text
    file << "  Text [" << text << "]\n";

    // circle's condition(s)
    if (conditions != "") file << "  Cond [" << conditions << "]\n";

    // circle's variable(s)
    if (variables != "") file << "  Vars [" << variables << "]\n";

    file << "End\n";
};

// load an Circle from file
void Circle::load (u_int32 num)
{
    string str;
    int n;

    // The Circle's number (it's index in the nodes array)
    number = num;

    // as long as we haven't reached EOF or are finished with loading:
    while (1)
    {
        // look what we find in the file
        switch (parse_dlgfile (str, n))
        {
            // EOF or finished
            case 0:
            case LOAD_END: return;

            // Type of node
            case LOAD_TYPE:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM) type = n;
                break;
            }

            // Coordinates of Circle
            case LOAD_POS:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM) position.x = n;
                if (parse_dlgfile (str, n) == LOAD_NUM) position.y = n;
                position.width = position.height = 21;
                break;
            }

            // The Circle's Text
            case LOAD_TEXT:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) text = str;                
                break;
            }

            // The Circle's Annotations
            case LOAD_NOTE:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) comment = str;                
                break;
            }
            
            // The Circle's Conditions
            case LOAD_COND:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) conditions = str;                
                break;
            }
            
            // The Circle's Variables
            case LOAD_VARS:
            {
                if (parse_dlgfile (str, n) == LOAD_STR) variables = str;
                break;             
            }
                        
            // Just ignore everything else           
            default: break;
        }
    }
}


Arrow::Arrow (u_int32 num, u_int8 tp)
{
    number = num;
    type = tp;
}

// Save all data neccessary to restore the arrow
void Arrow::save (ofstream &file)
{
    u_int32 i;
    
    // Keyword "Arrow" and arrow's number
    file << "\nArrow\n";

    // arrow's type
    file << "  Type " << (int) type << "\n";

    // start circle
    file << "  Prev " << prev[0]->number << "\n";

    // end circle
    file << "  Next " << next[0]->number << "\n";

    // all the links
    for (i = 0; i < link.size (); i++)
        file << "  Link " << link[i]->number << "\n";

    file << "End\n";
}

// load an Arrow from file
void Arrow::load (u_int32 num, vector<DlgNode*> &nodes)
{
    DlgNode *circle;
    string str;
    int n;

    // The Arrows number (it's index in the nodes array)
    number = num;

    // as long as we haven't reached EOF or are finished with loading:
    while (1)
    {
        // look what we find in the file
        switch (parse_dlgfile (str, n))
        {
            // EOF or finished
            case 0:
            case LOAD_END: return;

            // Type of node
            case LOAD_TYPE:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM) type = n;
                break;
            }

            // Node prior to arrow
            case LOAD_PREV:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM) 
                {
                    circle = nodes[n];

                    circle->next.push_back (this);
                    prev.push_back (circle);
                }
                break;
            }

            // Node following arrow
            case LOAD_NEXT:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM) 
                {
                    circle = nodes [n];

                    circle->prev.push_back (this);
                    next.push_back (circle);
                }
                break;
            }

            // Nodes linked to the arrow
            case LOAD_LINK:
            {
                if (parse_dlgfile (str, n) == LOAD_NUM) 
                {
                    circle = nodes[n];

                    circle->link.push_back (this);
                    link.push_back (circle);
                }
                break;
            }

            // Just ignore everything else           
            default: break;
        }
    }
}