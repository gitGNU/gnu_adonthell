/*
   $Id$

   Copyright (C) 1999/2000/2001/2003 Kai Sterker <kaisterker@linuxgames.com>
   Copyright (C) 2001 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   py_object.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the py_object class.
 */


#ifndef PY_OBJECT_H
#define PY_OBJECT_H
                   
#include <string>
#include "python_class.h"

using std::string; 


/**
 * Python object class.
 *
 * Use this class to create instances of Python classes contained in Python
 * modules, then control their execution. You can pass an argument tuple to
 * the class constructor and to any method you want to run. It is further
 * possible to access and change attributes of the Python instance.
 * 
 */ 
class py_object
{
public:
    /** 
     * Default constructor.
     * 
     */
    py_object ();
    
    /** 
     * Destructor.
     * 
     */
    ~py_object (); 

    /** 
     * Resets the script to it's post-constructor state.
     * 
     */
    void clear (); 

    /**
     * @name PyObject creation
     */
    //@{
    /** 
     * Creates an instance of a Python class.
     * 
     * @param file file name of the module to use.
     * @param classname name of the class to import.
     * @param args Python tuple containing the arguments to pass to the
     *             Python class constructor.
     */
    bool create_instance (string file, string classname, PyObject * args = NULL);

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
    bool reload_instance (string file, string classname, PyObject * args = NULL);
    //@}

    /**
     * @name PyObject method calling
     */
    //@{
    /** 
     * Call a method of this object.
     * 
     * @param name name of the method to call.
     * @param args Python tuple containing the arguments to pass to the method.
     * @return the return value of the method as PyObject. Needs to be 
     *     Py_DECREF'd when no longer needed.
     */
    PyObject *call_method_ret (const string &name, PyObject *args = NULL) const;
     
    /** 
     * Call a method of this object.
     * 
     * @param name name of the method to call.
     * @param args Python tuple containing the arguments to pass to the method.
     */
    void call_method (const string & name, PyObject * args = NULL) const
    {
        PyObject *result = call_method_ret (name, args);
        Py_XDECREF (result);
    }
    
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
    //@}

    /**
     * @name PyObject member access
     */
    //@{
    /**
     * Tests whether the object contains a certain attribute (i.e. method
     * or variable).
     *
     * @param name Name of the attribute to test for
     * @return <b>true</b> if the attribute exists, <b>false</b> otherwise.
     */
    bool has_attribute (const std::string & name);

    /**
     * Returns a new reference to an attribute of this object.
     *
     * @param name Name of the attribute to access
     * @return New reference to the attribute or NULL on error
     */
    PyObject* get_attribute (const string & name);

    /**
     * Returns the given attribute as integer value.
     *
     * @param name Name of the attribute to access
     * @return An integer.
     */
    s_int32 get_attribute_int (const string & name);

    /**
     * Returns the given attribute as string value.
     *
     * @param name Name of the attribute to access
     * @return A string.
     */
    string get_attribute_string (const string & name);

    /**
     * Assign a new attribute to the module, overriding an existing
     * attribute of the same name.
     *
     * @param name The attribute's name
     * @param value The attribute's value
     */
    void set_attribute (const string & name, PyObject *value);

    /**
     * Assign a new integer attribute to the module, overriding an
     * existing attribute of the same name.
     *
     * @param name The attribute's name
     * @param value The attribute's value
     */
    void set_attribute_int (const string & name, s_int32 value);

    /**
     * Assign a new string attribute to the module, overriding an
     * existing attribute of the same name.
     *
     * @param name The attribute's name
     * @param value The attribute's value
     */
    void set_attribute_string (const string & name, const string & value);
    //@}
    
    /**
     * @name Member access
     */
    //@{
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
    //@}

protected:
    /**
     * The python class instance wrapped by %py_object
     */    
    PyObject *Instance;

private:
    /**
     * Helper for create_instance and reload_instance
     *
     */
    bool instanciate (PyObject*, string, string, PyObject*);

    /**
     * The class name of the current script
     */
    std::string Classname;

    /**
     * The file name of the current script
     */
    std::string Filename;
};

#endif // PY_OBJECT_H
