/*
   $Id$
 
   Copyright (C) 2001 by Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "landmap.h"
#include "game.h"

int main(int argc, char * argv[])
{
    game the_game (argc, argv);
    if (!the_game.init ()) 
        return 1;

    landmap lmap;
    mapview mview;

    /*    mapobject mobj;
    lmap.add_submap(30,30);
    mobj.load("grass.obj");
    lmap.insert_mapobject(mobj,lmap.nbr_of_patterns,"grass.obj");
    mobj.load("flag.obj");
    lmap.insert_mapobject(mobj,lmap.nbr_of_patterns,"flag.obj");
    mobj.load("terry.obj");
    lmap.insert_mapobject(mobj,lmap.nbr_of_patterns,"terry.obj");
    for(i=0;i<30;i++)
      for(j=0;j<30;j++)
	lmap.set_square_pattern(0,i,j,0);
    lmap.set_square_pattern(0,16,13,1);
    lmap.set_square_pattern(0,15,15,1);
    lmap.set_square_pattern(0,14,17,1);
    lmap.set_square_pattern(0,13,12,1);
    */

    if(argc==2)
      lmap.load(argv[1]);

    //    lmap.load("mymap.map");
    mview.attach_map(&lmap);
    mview.set_current_submap(0);
    mview.resize(11,11);
    //    mview.set_pos(0,0);

    mview.editor();
    //    lmap.save("mymap.map");

    return 0;
}
