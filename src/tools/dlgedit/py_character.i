%module character

%{

#include "types.h"
#include "character.h"
#include "storage.h"

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

%typemap(python,out) string
{
    $result = PyString_FromString((const char *)$1.c_str());
}


%include "../../types.h"
%include "../../storage.h"
%include "../../character_base.h"
%include "../../mapcharacter.h"
%include "../../character.h"
