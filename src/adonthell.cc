/*
   Copyright (C) 1999/2000/2001 Alexandre Courbot
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/


/** 
 * @file adonthell.cc
 *
 * @author Alexandre Courbot
 * @author Kai Sterker
 * @brief Implements the adonthell class.
 */

#include "win_manager.h"
#include "gametime.h"
#include "gamedate.h"
#include "adonthell.h"
#include "audio.h"

// Pointer to the active main loop
adonthell *data::engine = NULL; 

// constructor
adonthell::adonthell ()
{
    letsexit = false;
    update_map_ = false;
    control_active_ = false;
    
    // load the script taking care of additional game commands
    control.create_instance ("schedules.control", "control");
}

// start and execute the game's main loop 
void adonthell::main (win_base *wnd, const string name)
{
	win_manager mgr;

	if (wnd != NULL)
	{
		mgr.add (wnd /*, name */);
		mgr.set_focus (wnd);
	}
	else
	{
		mapview_start ();
		set_control_active (true);
		fade_in ();
	}

	while (letsexit == false)
	{
		main_loop ();

		// blit the surface to the physical screen
		screen::show ();

		// perform operations to keep the game's speed constant
		gametime::update ();

		// update the internal clock
		gamedate::update ();
	}

    // only leave one main loop at a time
    letsexit = false;    
}

// the main loop
void adonthell::main_loop ()
{
    input::update ();

    // check whether music has finished playing
    if (audio::is_background_finished ()) audio::run_schedule ();

    // on slower machines, we update several times before drawing,
    // i.e. we are skipping frames to keep the game's speed constant
    for (int i = 0; i < gametime::frames_to_skip (); i++)
    {
        // grab any user input and update the internal state of
        // all windows of the current level
        win_manager::active->input_update ();
        if (update_map ()) lmap.update ();
        win_manager::active->update ();
        if (control_active ()) control.run ();
    }

    if (!letsexit)
    {
        // first clear the screen to avoid artifacts
        screen::clear ();

        // draw everything to our display surface
        win_manager::active->draw ();
    }
}

// quit the main loop
void adonthell::main_quit ()
{
    letsexit = true;
}

// fade the screen out
void adonthell::fade_out ()
{
    s_int16 i = 0;
    
    while (i < 60)
    {
        gametime::update ();
        i += gametime::frames_to_skip () * 2;
        if (i > 60) i = 60;
        
        main_loop ();

        screen::transition (i * 2);
        screen::show ();
    }
}

// fade the screen in
void adonthell::fade_in ()
{
    s_int16 i = 60;
    
    while (i > 0)
    {
        gametime::update ();
        i -= gametime::frames_to_skip () * 2;
        if (i < 0) i = 0;
        
        main_loop ();

        if (letsexit)
        {
            // if we're fading in while a window is closing
        	// ensure that everything is rendered properly
            screen::clear ();
            win_manager::active->draw ();
        }

        screen::transition (i * 2);
        screen::show ();
    }
}

// load the engine state
s_int8 adonthell::get_state (igzstream& file)
{
    string name;
    
    // get the current time
    gamedate::get_state (file);

    // Get the map filename 
    name << file; 
    // Load the map from the file
    lmap.get (file); 
    // Load the map state (events)
    if (!lmap.get_state (file))
        return false; 

    view.mapview::attach_map (&lmap);

    // Load the mapview state
    view.mapview::get_state (file);
    view.pack ();
    
    return true;
}

// save the engine state
s_int8 adonthell::put_state (ogzstream& file)
{
    // save the current time
    gamedate::put_state (file);
    
    // Save the map filename
    string name = lmap.filename ();
    name >> file; 

    // Save the map itself
    lmap.put (file); 
    // Save the map state (events)
    lmap.put_state (file); 
    // Save the mapview state
    view.mapview::put_state (file);

    return 0;
}

void adonthell::mapview_start ()
{
    set_update_map (true);
    
    view.mapview::resize (screen::length (), screen::height ());
    view.mapview::attach_map (&lmap);

    view.set_visible (true);
    view.pack ();
    
    win_manager::active->add (&view);
}

void adonthell::mapview_stop ()
{
    set_update_map (false);
    view.mapview::detach_map ();
    win_manager::active->remove (&view);
}
