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

/**
 * @file schedule.cc
 *
 * @author Kai Sterker
 * @brief Implements the character schedule class.
 */
 
#include "schedule.h"
#include "gamedate.h"
#include "event_handler.h"

// standart constructor
schedule::schedule ()
{
    Active = false;
    Running = false;
    Queue = NULL;
    Alarm = NULL;
}

// destructor
schedule::~schedule ()
{
    if (Alarm)
    {
        event_handler::remove_event (Alarm);
        delete Alarm;        
    }
    
    if (Queue)
        delete Queue;
}

// execute the schedule
void schedule::update ()
{
    // no schedule active
    if (!Active) return;
    
    // no schedule running --> assign a new one
    if (!Running) 
    {
        // if no schedule queued, let the manager script decide
        if (!Queue)
        {
            // clearing the schedule before the manager runs
            // allows the manager to access the most recent data
            Schedule.clear ();
            Manager.run ();
        }
        
        // otherwise use the queued script
        else
        {
            set_schedule (Queue->file, Queue->args);
            if (Queue->time != "") 
                set_alarm (Queue->time, Queue->absolute);
            
            delete Queue;
            Queue = NULL;
        }
    }
        
    // finally run schedule
    Schedule.run ();
}

// assign a (new) schedule
void schedule::set_schedule (string file, PyObject *args)
{
    if (Running)
    {
        fprintf (stderr, "*** schedule::set_schedule: stop current schedule first!\n");
        return;
    }
    
    // no need to clear anything, as py_object takes care of that
    Schedule.create_instance (SCHEDULE_DIR + file, file, args);
    
    // 'run' schedule
    Running = true;
    
    // cancel alarm
    if (Alarm)
    {
        event_handler::remove_event (Alarm);
        delete Alarm;
        Alarm = NULL;
    }
}

// queue a schedule
void schedule::queue_schedule (string file, PyObject *args)
{
    if (Queue) delete Queue;

    Queue = new schedule_data (file, args);
}

// assign a (new) manager script
void schedule::set_manager (string file, PyObject *args)
{
    Manager.create_instance (SCHEDULE_DIR + file, file, args);
}

// set the alarm
void schedule::set_alarm (string time, bool absolute)
{
    // get rid of the current alarm
    if (Alarm)
    {
        event_handler::remove_event (Alarm);
        delete Alarm;        
    }

    // absolute hour of the day
    if (absolute)
    {
        char day[16];

        // if that hour has already passed today, assume the
        // same hour tomorrow
        if (gamedate::parse_time (time) > (u_int32) gamedate::hour () * 60)
            sprintf (day, "%id", gamedate::day ());
        else
            sprintf (day, "%id", gamedate::day () + 1);

        time += day;
    }

    // create and register the new alarm
    Alarm = new time_event (time, absolute);
    Alarm->set_callback (makeFunctor (*this, &schedule::on_alarm));
    event_handler::register_event (Alarm);
}

// set alarm for a queued schedule
void schedule::queue_alarm (string time, bool absolute)
{
    if (!Queue)
    {
        fprintf (stderr, "*** schedule::queue_alarm: queue a schedule first!\n");
        return;
    }
    
    Queue->time = time;
    Queue->absolute = absolute;
}

// save state to disk
void schedule::put_state (ogzstream &file)
{
    Active >> file;
    Running >> file;
    
    // current manager script
    Manager.put_state (file);
    
    // current schedule script
    Schedule.put_state (file);
    
    // save alarm, if set
    if (!Alarm)
    {
        false >> file;
    }
    else
    {
        true >> file;
        Alarm->put_state (file);
    }
    
    // save queue, if set
    if (!Queue)
    {
        false >> file;
    }
    else
    {
        true >> file;
        Queue->put_state (file);
    }
}

// load state from disk
bool schedule::get_state (igzstream &file)
{
    bool has_feature;
    
    Active << file;
    Running << file;
    
    // restore manager script
    if (!Manager.get_state (file))
    {
        fprintf (stderr, "*** schedule::get_state: failed loading manager script\n");
        return false;
    }
    
    // restore schedule script
    if (!Schedule.get_state (file))
    {
        fprintf (stderr, "*** schedule::get_state: failed loading schedule script\n");
        return false;
    }
    
    // restore alarm
    has_feature << file;
    if (has_feature)
    {
        Alarm = new time_event;
        Alarm->get_state (file);
        Alarm->set_callback (makeFunctor (*this, &schedule::on_alarm));
        
        // don't forget to register the alarm!
        event_handler::register_event (Alarm);
    }
    
    // restore queue
    has_feature << file;
    if (has_feature)
    {
        Queue = new schedule_data;
        Queue->get_state (file);
    }
    
    return true;
}
