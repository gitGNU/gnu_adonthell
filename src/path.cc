/*
   $Id$

   Copyright (C) 2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   path.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the path class.
 * 
 * 
 */

#include "path.h"
#include "landmap.h"
#include <queue>
#include <algorithm> 


u_int16 path::goal_estimate (u_int16 x, u_int16 y) 
{
    u_int16 est; 
    if (x > goal.x) est = x - goal.x;
    else est = goal.x - x;

    if (y > goal.y) est += y - goal.y;
    else est += goal.y - y;

    return est; 
}
 
bool path::calculate ()
{ 
    // Sorted Open nodes.
    priority_queue <mapsquare *, vector<mapsquare *>, compare_squarecost> sorted_nodes;  
    // Open nodes.
    vector <mapsquare *> opened_nodes;  
    // Closed nodes.
    vector <mapsquare *> closed_nodes; 

    moves_to_goal.clear ();

    mapsquare_area * smap = refmap->get_submap (submap); 
    
    mapsquare * n = smap->get_square (start.x, start.y);
    // Origin node
    n->g = 0;
    n->h = goal_estimate (n->x (), n->y ()); 
    n->f = n->g + n->h;
    n->parent = NULL; 

    sorted_nodes.push (n);
    opened_nodes.push_back (n);
    while (!sorted_nodes.empty ()) 
    {
        n = sorted_nodes.top ();
        sorted_nodes.pop ();
        opened_nodes.erase (find (opened_nodes.begin (), opened_nodes.end (), n));
        
        // Have we reached the goal?
        if (n->x () == goal.x && n->y () == goal.y) 
        {
            while (n->parent != NULL)
            {
                // Vertical move
                if (n->x () == n->parent->x ())
                {
                    // Go to north
                    if (n->y () - n->parent->y () < 0)
                        moves_to_goal.push_back (WALK_NORTH);
                    // Go to south
                    else moves_to_goal.push_back (WALK_SOUTH); 
                }
                // Horizontal move
                else
                {
                    // Go to west
                    if (n->x () - n->parent->x () < 0)
                        moves_to_goal.push_back (WALK_WEST);
                    // Go to east
                    else moves_to_goal.push_back (WALK_EAST); 
                }
                n = n->parent; 
            }
            return true; 
        }

        // Now proceeding with the successors of n
        mapsquare * np;
        
        // East square
        // Make sure that the square is not at the edge of the submap
        // and is directly reachable.
        // If so, add it to the opened nodes list.
        if (n->x () + 1 < smap->area_length ())
        {
            np = smap->get_square (n->x () + 1, n->y ());
            if (n->is_walkable_east () && np->is_walkable_west () && np->is_free () &&
                (np->can_use_for_pathfinding || (np->x () == goal.x && np->y () == goal.y))) 
            {
                u_int16 newg = n->g + 1;
                bool in_opened, in_closed; 
                in_opened = (find (opened_nodes.begin (), opened_nodes.end (), np) != opened_nodes.end ()); 
                in_closed = (find (closed_nodes.begin (), closed_nodes.end (), np) != closed_nodes.end ());
                
                // If np is in opened_nodes or closed_nodes and np->g <= newg, don't do anything.
                if (!((in_opened || in_closed) && np->g <= newg))
                    // else add the node to the opened nodes list (if necessary)
                { 
                    np->g = newg; 
                    np->h = goal_estimate (np->x (), np->y ());
                    np->f = np->g + np->h;
                    np->parent = n;

                    // if np is in closed_nodes, remove it
                    if (in_closed) 
                        closed_nodes.erase (find (closed_nodes.begin (), closed_nodes.end (), np)); 

                    // if np is not in opened_nodes yet, add it
                    if (!in_opened)
                    {
                        sorted_nodes.push (np); 
                        opened_nodes.push_back (np);
                    }
                } 
            }
        }


        // West square
        // Make sure that the square is not at the edge of the submap
        // and is directly reachable.
        // If so, add it to the opened nodes list.
        if (n->x () > 0)
        {
            np = smap->get_square (n->x () - 1, n->y ());
            if (n->is_walkable_west () && np->is_walkable_east () && np->is_free () &&
                (np->can_use_for_pathfinding || (np->x () == goal.x && np->y () == goal.y))) 
            {
                u_int16 newg = n->g + 1;
                bool in_opened, in_closed; 
                in_opened = (find (opened_nodes.begin (), opened_nodes.end (), np) != opened_nodes.end ()); 
                in_closed = (find (closed_nodes.begin (), closed_nodes.end (), np) != closed_nodes.end ());
                
                // If np is in opened_nodes or closed_nodes and np->g <= newg, don't do anything.
                if (!((in_opened || in_closed) && np->g <= newg))
                    // else add the node to the opened nodes list (if necessary)
                { 
                    np->g = newg; 
                    np->h = goal_estimate (np->x (), np->y ());
                    np->f = np->g + np->h;
                    np->parent = n;

                    // if np is in closed_nodes, remove it
                    if (in_closed) 
                        closed_nodes.erase (find (closed_nodes.begin (), closed_nodes.end (), np)); 

                    // if np is not in opened_nodes yet, add it
                    if (!in_opened)
                    {
                        sorted_nodes.push (np); 
                        opened_nodes.push_back (np);
                    }
                } 
            }
        }


        // North square
        // Make sure that the square is not at the edge of the submap
        // and is directly reachable.
        // If so, add it to the opened nodes list.
        if (n->y () > 0)
        {
            np = smap->get_square (n->x (), n->y () - 1);
            if (n->is_walkable_north () && np->is_walkable_south () && np->is_free () &&
                (np->can_use_for_pathfinding || (np->x () == goal.x && np->y () == goal.y))) 
              {
                u_int16 newg = n->g + 1;
                bool in_opened, in_closed; 
                in_opened = (find (opened_nodes.begin (), opened_nodes.end (), np) != opened_nodes.end ()); 
                in_closed = (find (closed_nodes.begin (), closed_nodes.end (), np) != closed_nodes.end ());
                
                // If np is in opened_nodes or closed_nodes and np->g <= newg, don't do anything.
                if (!((in_opened || in_closed) && np->g <= newg))
                    // else add the node to the opened nodes list (if necessary)
                { 
                    np->g = newg; 
                    np->h = goal_estimate (np->x (), np->y ());
                    np->f = np->g + np->h;
                    np->parent = n;

                    // if np is in closed_nodes, remove it
                    if (in_closed) 
                        closed_nodes.erase (find (closed_nodes.begin (), closed_nodes.end (), np)); 

                    // if np is not in opened_nodes yet, add it
                    if (!in_opened)
                    {
                        sorted_nodes.push (np); 
                        opened_nodes.push_back (np);
                    }
                } 
            }
        }

        // South square
        // Make sure that the square is not at the edge of the submap
        // and is directly reachable.
        // If so, add it to the opened nodes list.
        if (n->y () + 1 < smap->area_height ())
        {
            np = smap->get_square (n->x (), n->y () + 1);
            if (n->is_walkable_south () && np->is_walkable_north () && np->is_free () &&
                (np->can_use_for_pathfinding || (np->x () == goal.x && np->y () == goal.y))) 
            {
                u_int16 newg = n->g + 1;
                bool in_opened, in_closed; 
                in_opened = (find (opened_nodes.begin (), opened_nodes.end (), np) != opened_nodes.end ()); 
                in_closed = (find (closed_nodes.begin (), closed_nodes.end (), np) != closed_nodes.end ());
                
                // If np is in opened_nodes or closed_nodes and np->g <= newg, don't do anything.
                if (!((in_opened || in_closed) && np->g <= newg))
                    // else add the node to the opened nodes list (if necessary)
                { 
                    np->g = newg; 
                    np->h = goal_estimate (np->x (), np->y ());
                    np->f = np->g + np->h;
                    np->parent = n;

                    // if np is in closed_nodes, remove it
                    if (in_closed) 
                        closed_nodes.erase (find (closed_nodes.begin (), closed_nodes.end (), np)); 

                    // if np is not in opened_nodes yet, add it
                    if (!in_opened)
                    {
                        sorted_nodes.push (np); 
                        opened_nodes.push_back (np);
                    }
                } 
            }
        }

        closed_nodes.push_back (n); 
    }     
    return false; 
}
