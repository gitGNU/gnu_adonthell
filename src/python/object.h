/*
   $Id$

   Copyright (C) 1999/2000/2001/2002 Kai Sterker
   Copyright (C) 2001    Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   object.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the object class.
 * 
 * 
 */


#ifndef PY_OBJECT_H_
#define PY_OBJECT_H_


#include <string>
#include "base.h"

namespace python
{

    /**
     * Python object class.
     *
     * Use this class to create instances of Python classes
     * contained in Python modules, then control their execution. You can pass
     * arguments to the class constructor when the script is imported and to
     * any method you wish to run.
     * 
     */ 
    class object
    {
    public:
        /** 
         * Default constructor.
         * 
         */
        object ();
    
        /** 
         * Destructor.
         * 
         */
        ~object (); 

        /** 
         * Resets the script to it's post-constructor state.
         * 
         */
        void clear (); 
 
        /** 
         * Creates an instance of a Python class.
         * 
         * @param file file name of the module to use.
         * @param classname name of the class to import.
         * @param args Python tuple containing the arguments to pass to the
         *             Python class constructor.
         */
        bool create_instance (std::string file, std::string classname, PyObject * args = NULL);

        /**
         * Similar to create_instance, except that it will reload the module
         * from disk, in case it has been changed in the meantime. Mainly interesting
         * for script development or tools like dlgedit.
         *
         * @param file file name of the module to use.
         * @param classname name of the class to import.
         * @param args Python tuple containing the arguments to pass to the
         *             Python class constructor.
         */
        bool reload_instance (std::string file, std::string classname, PyObject * args = NULL);

        /**
         * Direct access to the instance object
         *
         * @return the Python class instance
         */
        PyObject *get_instance () const
        {
            return Instance;
        }

        /**
         * Returns the arguments of this object.
         * 
         * @return arguments of this object.
         */
        PyObject *get_ctorargs () const
        {
            return CtorArgs;
        }

        /**
         * Returns the class name of this object.
         * 
         * @return class name of this object.
         */
        std::string class_name () const
        {
            return Classname;
        }
     
        /**
         * Returns the file name of this object.
         * 
         * @return fiöe name of this object.
         */
        std::string file_name () const
        {
            return Filename;
        }

        /** 
         * Call a method of this object.
         * 
         * @param name name of the method to call.
         * @param args Python tuple containing the arguments to pass to the method.
         */
        void call_method (const std::string & name, PyObject * args = NULL); 
    
        /** 
         * Calls the run () method of this object.
         * Equivalent to call_method ("run", args); 
         *
         * @param args Python tuple containing the arguments to pass to the method. 
         */
        void run (PyObject * args = NULL)
        {
            call_method ("run", args); 
        }

        /**
         * Returns a new reference to an attribute of this object.
         *
         * @param name Name of the attribute to access
         * @return New reference to the attribute or NULL on error
         */
        PyObject* get_attribute (const std::string & name);

        /**
         * Tests whether the object contains a certain attribute (i.e. method
         * or variable).
         *
         * @param name Name of the attribute to test for
         * @return <b>true</b> if the attribute exists, <b>false</b> otherwise.
         */
        bool has_attribute (const std::string & name);

        /**
         * Saves the state of the associated script, so that it can be
         * completely restored later on. Writes the file- and class name
         * of the script, the arguments passed to its constructor and
         * finally its internal state (i.e. the __dict__) to file. 
         *
         * @param file The stream to save the state to.
         */
        void put_state (ogzstream & file);
    
        /**
         * Restores the state of the object from file. Loads file-,
         * class name and constructor arguement. Then instanciates the
         * script and finally merges its __dict__ with the key/value pairs 
         * read from file.
         *
         * @param file The stream to load the state from.
         * @return <b>true</b>if loading succeeded, <b>false</b> otherwise.
         */
        bool get_state (igzstream & file);

#ifndef SWIG
        /**
         * Clone the %py_pbject. Note that this implementation makes
         * a shallow copy of the underlying python instance. However, thanks
         * to python's reference counting, deleting the clone will not effect
         * the original or vice versa. But changing one will change the other.
         *
         * @param script The &object to clone
         */
        void operator= (const object &script);
#endif // SWIG

    private:
        /**
         * Helper for create_instance and reload_instance. Instanciates a
         * class contained with a python module (i.e. at the toplevel of
         * a Python script).
         *
         * @param module The Python module that contains the class to instanciate.
         * @param path The full path to the module
         * @param classname The name of the class to instanciate
         * @param args A tuple of the arguments to pass to the class constructor.
         */
        bool instanciate (PyObject *module, std::string path, std::string classname, PyObject *args);

        /**
         * The class name of the current script
         */
        std::string Classname;
    
        /**
         * The file name of the current script
         */
        std::string Filename;

        /**
         * The actual instance of the Python class
         */
        PyObject *Instance;
    
        /**
         * Constructor arguments
         */
        PyObject *CtorArgs;
    };
}

#endif
