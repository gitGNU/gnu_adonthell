%module adonthell

%feature("compactdefaultargs");

%{

#include <string>
#include "types.h"
#include "time_event.h"
#include "map_event.h"
#include "event_list.h"
#include "event_handler.h"
#include "storage.h"
#include "gametime.h"
#include "gamedate.h"
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
#include "mapsquare.h"
#include "mapobject.h"
#include "mapcharacter.h"
#include "path.h"
#include "landmap.h"
#include "mapview.h"
#include "character.h"
#include "label_input.h"
#include "win_types.h"
#include "win_label.h"
#include "win_image.h"
#include "win_write.h"
#include "win_mapview.h"
#include "win_ttf.h"
#include "win_theme.h"
#include "win_base.h"
#include "text_bubble.h"
#include "win_container.h"
#include "win_scroll.h"
#include "win_select.h"
#include "win_manager.h"
#include "dialog_screen.h"
#include "data_screen.h"
#include "gamedata.h"
#include "game.h"
#include "nls.h"
#include "achievements.h"

// Workaround for keysym missing in older versions of SDL
#ifndef SDLK_UNDO
#define     SDLK_UNDO 322
#endif

// Workaround for bug in pyptrtypes.swg
#define SWIG_append_msg SWIG_append_errmsg

// This enum allows clearer ownership operation:
// obj.thisown = Python (Python will destroy the object)
// obj.thisown = C (C has to destroy the object)
enum {Python = 1, C = 0};

// pass an instance to python
PyObject *get_py_obj (void *instance, const char* class_name)
{
    swig_type_info * ti = SWIG_TypeQuery (class_name);
    if (ti) return SWIG_NewPointerObj (instance, ti, C);
    else printf ("*** get_py_obj: Type '%s' not known to SWIG!\n", class_name);
    return NULL;
}

%}

// Same enum available for Python
enum {Python = 1, C = 0};

%include "std_string.i"

%typemap (in) PyObject *pyfunc 
{ 
    if (!PyCallable_Check($input)) 
    { 
        PyErr_SetString (PyExc_TypeError, "Need a callable object!");
        return NULL;
    }
    $1 = $input; 
}

%typemap (in) PyObject*
{ 
    $1 = $input; 
}

%include "types.h"
%include "fileops.h"
%include "event.h"
%include "event_list.h"
%include "time_event.h"
%include "map_event.h"
%include "event_handler.h"
%include "storage.h"
%include "gametime.h"
%include "gamedate.h"
%include "SDL_keysym.h"
%include "input.h"
%include "audio.h"
%include "character_base.h"
%include "py_object.h"
%include "drawing_area.h"
%include "quest.h"
%include "drawable.h"
%include "surface.h"
%include "screen.h"
%include "image.h"
%include "animation.h"
%include "mapsquare_walkable.h"
%include "mapsquare.h"
%include "mapobject.h"
%include "mapcharacter.h"
%include "path.h"
%include "character.h"
%include "landmap.h"
%include "mapview.h"
%include "adonthell.h"
%include "win_types.h"
%include "win_wrappers.h"
%include "text_bubble.h"
%include "dialog_screen.h"
%include "data_screen.h"
%include "gamedata.h"
%include "game.h"
%include "nls.h"
%include "prefs.h"
%include "achievements.h"

