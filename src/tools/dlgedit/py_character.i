%module character

%{

#include "types.h"
#include "character.h"
#include "storage.h"

%}

%typemap(python,in) string
{
    if (PyString_Check ($source))
    {
        $target = new string (PyString_AsString($source));
    }
    else
    {
        PyErr_SetString (PyExc_TypeError, "not a String");
        return NULL;
    }
}

%typemap(python,out) string
{
    $target = PyString_FromString((const char *)$source->c_str());
    delete $source;
}

%typemap (python, freearg) string
{
    if ($source != NULL)
    {
        delete $source;
    }
}

%include "../../types.h"
%include "../../storage.h"
%include "../../character_base.h"
%include "../../mapcharacter.h"
%include "../../character.h"
