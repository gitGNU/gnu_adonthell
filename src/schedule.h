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
 * @file schedule.h
 *
 * @author Kai Sterker
 * @brief Declares the character schedule class.
 */

#ifndef SCHEDULE_H__
#define SCHEDULE_H__

#include "py_object.h"
#include "time_event.h"

/**
 * Path to the schedule scripts.
 */
#define SCHEDULE_DIR "schedules."

/**
 * This class manages %character schedules. Each %character has a
 * manager script that defines what it does all day long. The 
 * character's actual activity is controlled by different schedule 
 * scripts that are selected by the manager script, depending on 
 * various factors.
 */
class schedule
{
public:

    /**
     * Standart constructor. Initialize everything to NULL/false.
     */        
    schedule ();

    /**
     * Destructor.
     */        
    ~schedule ();
    
    /**
     * Executes the mapcharacter's %schedule script in case it is active
     * and running. If it stopped running, the manager script will be
     * launched to determine a new %schedule. If the %schedule isn't active,
     * nothing will happen.
     */
    void update ();

    /**
     * Assign a (new) %schedule script. This script is responsible for
     * the character's current activity.
     *
     * @param file Filename of the script to load.
     * @param args Addional arguments to pass to the script constructor.
     */
    void set_schedule (string file, PyObject * args = NULL);

    /**
     * Call this to explicitly clear the attached schedule. This will
     * also run the destructor (__del__ method) of the script, if it
     * exists.
     */
    void clear_schedule ()
    {
        Schedule.clear ();
    }

    /** 
     * Returns whether the %schedule is active or not. As long as the
     * %schedule is active, it is executed every %game cycle. If it's
     * inactive, it will cause the %character to freeze.
     * 
     * @return \c true if the %schedule is active, \c false otherwise.
     */
    bool is_active () const     { return Active; }
    
    /** 
     * Sets whether the %schedule is active or not. As long as the
     * %schedule is active, it is executed every %game cycle. If it's
     * inactive, it will cause the %character to freeze.
     * 
     * @param a \c true if the schedule should be activated, \c false otherwise.
     */
    void set_active (bool a)    { Active = a; }
    
    /** 
     * Returns whether the current %schedule is running or not. Once
     * it stops running, the manager script will be executed to
     * determine a new schedule.
     * 
     * @return \c true if the schedule is running, \c false otherwise.
     */
    bool is_running () const     { return Running; }
    
    /** 
     * Sets whether the %schedule is running or not. Once
     * it stops running, the manager script will be executed to
     * determine a new schedule.
     *
     * @param a \c false if the schedule should be stopped, \c true otherwise.
     */
    void set_running (bool r)    { Running = r; }

    /**
     * Assign a (new) manager script. This script is responsible for
     * the overall %character behaviour.
     *
     * @param file Filename of the script to load.
     * @param args Addional arguments to pass to the script constructor.
     */
    void set_manager (string file, PyObject * args = NULL);

    /**
     * Specify a period after which the manager script is run,
     * whether the active %schedule is finished or not. This function
     * makes use of the %event system and is much more efficient than
     * querying the current time within the schedule itself. It
     * therefore is the preferred method of letting schedules run a 
     * fixed amount of gametime.
     *
     * Note that setting a new %schedule script will cancel the alarm.
     * Setting a new alarm will also override the existing one. That
     * means, only one alarm can be active at any given time.
     *
     * @param time The amount of time the %schedule should be active,
     *      in the format used by time_event::time_event ().
     * @param absolute If <b>true</b>, the current day will be added
     *      to the time. For example, set_alarm ("14h", true) means
     *      "today at 14:00", whereas set_alarm ("14h", false) means
     *      "14 hours from now on".
     */
    void set_alarm (string time, bool absolute = false);
                
    /**
     * Restore the state of the %schedule class from disk. Loads
     * a state previously saved with the put_state () method.
     *
     * @param file The gzip compressed file to load the state from.
     * @return <b>true</b> if restoring succeeded, <b>false</b> otherwise.
     */
    bool get_state (igzstream &file);
    
    /**
     * Save the state of the %schedule class to disk. Use the
     * get_state () method to restore it later on.
     *
     * @param file The gzip compressed file to save the state to.
     */
    void put_state (ogzstream &out);
    
private:
#ifndef SWIG
    // whether the schedule should be executed 
    bool Active;

    // whether the manager should be executed
    bool Running;
    
    // the script describing a character's general behaviour.
    py_object Manager;
    
    // the script describing a specific activity in detail.
    py_object Schedule;
    
    // set the alarm to execute the manager script at a certain time
    time_event *Alarm;
#endif // SWIG
};

#endif // SCHEDULE_H__
