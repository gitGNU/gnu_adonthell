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
 * @file schedule_data.h
 *
 * @author Kai Sterker
 * @brief Declares the schedule_data class.
 */

#ifndef SCHEDULE_DATA_H__
#define SCHEDULE_DATA_H__


/**
 * Data needed to queue a schedule. Only used by the schedule
 * class internally.
 */
class schedule_data
{
public:
    schedule_data () { }

    schedule_data (string f, PyObject *a)
    {
        Py_XINCREF (a);

        file = f;
        args = a;
        time = "";
        absolute = false;
    }

    ~schedule_data ()
    {
        Py_XDECREF (args);
    }

    void put_state (ogzstream &file) { }
    void get_state (igzstream &file) { }

    string file;
    PyObject *args;
    string time;
    bool absolute;
};

#endif // SCHEDULE_DATA_H__
