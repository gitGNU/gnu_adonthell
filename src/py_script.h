/*
   $Id$

   Copyright (C) 1999/2000/2001    Kai Sterker
   Copyright (C) 2001    Alexandre Courbot
Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   py_script.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the py_script class.
 * 
 * 
 */


#ifndef PY_SCRIPT_H_
#define PY_SCRIPT_H_


#include <string>
#include "python_class.h"


using namespace std; 


/**
 * Python compiled script class.
 *
 * Use this class to give scripting - abilities to your
 * own class. The loaded script is byte-compiled for
 * faster execution, and can then be launched on a regular
 * basis.
 * 
 */ 
class py_script
{
public:
    
    /** 
     * Default constructor.
     * 
     */
    py_script ();
    
    /** 
     * Destructor.
     * 
     */
    ~py_script (); 

    /** 
     * Resets the script to it's post-constructor state.
     * 
     */
    void clear (); 

    /** 
     * Sets the local to use for this script.
     * 
     * @param l locals to use for this script.
     */
    void set_locals (PyObject * l = NULL)
    {
        locals = l; 
    }
    
    /** 
     * Sets the script.
     * 
     * @param file file name of the script to use.
     */
    void set_script (string file);

    /**
     * Returns the file name of this script.
     * 
     * 
     * @return file name of this script.
     */
    string script_file () const
    {
        return script_file_;
    }
    
    /** 
     * Returns whether this script is activated of not.
     * 
     * 
     * @return \c true if the script is activated, \c false otherwise.
     */
    bool is_activated () const
    {
        return script_activated;
    }
    
    /** 
     * Sets whether the script should be activated or not.
     * 
     * @param a \c true if the script should be activated, \c false otherwise.
     */
    void set_active (bool a)
    {
        script_activated = a;
    }

    /** 
     * Runs the script.
     *
     * The script will only be run if it exists (set_script has been called)
     * and is activated.
     * 
     */
    void run (); 
    
private:
    bool script_activated;
    PyCodeObject *script;
    string script_file_;

    PyObject * locals;
};


#endif
