%module adonthell

%{

#include <string>
#include "types.h"
#include "storage.h"
#include "event.h"
#include "gametime.h"
#include "SDL_keysym.h"
#include "input.h"
#include "audio.h"
#include "character_base.h"
#include "quest.h"
#include "drawing_area.h"
#include "drawable.h"
#include "surface.h"
#include "screen.h"
#include "image.h"
#include "animation.h"
#include "mapsquare_walkable.h"
#include "mapobject.h"
#include "mapcharacter.h"
#include "landmap.h"
#include "mapview.h"
#include "mapengine.h"
#include "character.h"
#include "win_types.h"
#include "win_font.h"
#include "win_theme.h"
#include "win_base.h"
#include "win_container.h"
#include "win_label.h"
#include "win_image.h"
#include "win_scrolled.h"
#include "win_select.h"
#include "win_manager.h"
#include "dialog_engine.h"
#include "data_screen.h"
#include "gamedata.h"
#include "game.h"

// This enum allows clearer ownership operation:
// obj.thisown = Python (Python will destroy the object)
// obj.thisown = C (C have to destroy the object)
enum {Python = 1, C = 0};

%}

// This enum allows clearer ownership operation:
// obj.thisown = Python (Python will destroy the object)
// obj.thisown = C (C have to destroy the object)
enum {Python = 1, C = 0};

#define SDL_MIXER
#define SDL_HAS_64BIT_TYPE int
#define USE_MAP
#define USE_PYTHON

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

%typemap (python,in) PyObject *pyfunc 
{ 
    if (!PyCallable_Check($source)) 
    { 
        PyErr_SetString (PyExc_TypeError, "Need a callable object!");
        return NULL;
    }
    $target = $source; 
}

%typemap (python,in) PyObject*
{ 
    $target = $source; 
}

%include "types.h"
%include "storage.h"
%include "event.h"
%include "gametime.h"
%include "SDL_keysym.h"
%include "input.h"
%include "audio.h"
%include "character_base.h"
%include "drawing_area.h"
%include "quest.h"
%include "drawable.h"
%include "surface.h"
%include "screen.h"
%include "image.h"
%include "animation.h"
%include "mapsquare_walkable.h"
%include "mapobject.h"
%include "mapcharacter.h"
%include "character.h"
%include "landmap.h"
%include "mapview.h"
%include "mapengine.h"
%include "win_types.h"
%include "win_font.h"
%include "win_theme.h"
%include "win_base.h"
%include "win_container.h"
%include "win_label.h"
%include "win_image.h"
%include "win_scrolled.h"
%include "win_select.h"
%include "win_manager.h"
%include "dialog_engine.h"
%include "data_screen.h"
%include "gamedata.h"
%include "game.h"