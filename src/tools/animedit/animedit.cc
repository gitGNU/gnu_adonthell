/*
   $Id:

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "screen.h"
#include "gametime.h"
#include "win_manager.h"
#include "win_label.h"
#include <unistd.h>
#include "game.h"

int main (int argc, char * argv[]) 
{
    game::init ();
    cout << game::user_data_dir () << " " << game::global_data_dir () << " "
         << game::game_data_dir () << endl; 
    screen::set_video_mode (640, 480, 16);

    win_manager wmgr;
    
    input::init (); 
    win_manager::init (); 

    win_manager::add_theme ("original"); 
    win_manager::add_font ("original"); 

    win_theme * th = win_manager::get_theme ("original");
    win_font * ft = win_manager::get_font ("original"); 
    win_container * wc = new win_container ();
    wc->set_border (*th); 
    wc->set_background (*th); 
    wc->set_trans_background (false); 

    wc->move (20, 20);
    wc->resize (300, 200); 

    wc->set_visible_border (true); 
    wc->set_visible_background (true); 
    wc->set_visible (true);

    wmgr.add (wc);

    while (!input::has_been_pushed (SDLK_ESCAPE)) 
    {
        input::update (); 
        wmgr.update ();
        wmgr.draw (); 
        screen::show ();
        gametime::update ();
        screen::clear (); 
    }
    
    wmgr.remove (wc); 
    delete wc; 
    
    win_manager::cleanup (); 
    input::shutdown (); 
    return 0; 
}
