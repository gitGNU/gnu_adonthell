%module itemtest

%{
#include <string>
#include "types.h"
#include "storage.h"
#include "character_base.h"
#include "item_base.h"
%}

%typemap(python,in) string
{
    if (PyString_Check ($input))
    {
        $1 = string (PyString_AsString($input));
    }
    else
    {
        PyErr_SetString (PyExc_TypeError, "not a String");
        return NULL;
    }
}
%typemap(python,in) const string = string;

%typemap(python,in) string &
{
    if (PyString_Check ($input))
    {
        $1 = new string (PyString_AsString($input));
    }
    else
    {
        PyErr_SetString (PyExc_TypeError, "not a String");
        return NULL;
    }
}
%typemap(python,in) const string & = string &;

%typemap(python,out) string
{
    $result = PyString_FromString((const char *)$1.c_str()); 
}
%typemap(python,out) const string = string;

%typemap(python,out) string &
{
    $result = PyString_FromString((const char *)$1->c_str());
    delete $1; 
}
%typemap(python,out) const string & = string &;

%typemap (python, freearg) string &
{
    if ($1 != NULL)
    {
        delete $1;
    }
}
%typemap (python, freearg) const string & = string &;

%typemap (python,in) PyObject *pyfunc 
{ 
    if (!PyCallable_Check($input)) 
    { 
        PyErr_SetString (PyExc_TypeError, "Need a callable object!");
        return NULL;
    }
    $1 = $input; 
}

%typemap (python,in) PyObject*
{ 
    $1 = $input; 
}

%include "types.h"
%include "storage.h"
%include "character_base.h"
%include "item_base.h"
