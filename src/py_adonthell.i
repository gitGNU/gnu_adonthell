%module adonthell

%{

#include "types.h"
#include "storage.h"
#include "event.h"
#include "SDL_keysym.h"
#include "input.h"
#include "audio.h"
#include "character_base.h"
#include "quest.h"
#include "drawing_area.h"
#include "screen.h"
#include "image.h"
#include "animation.h"
#include "mapobject.h"
#include "mapcharacter.h"
#include "landmap.h"
#include "mapview.h"
#include "mapengine.h"
#include "character.h"
#include "callback_wrap.h"
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
#include "data.h"

%}

#define SDL_MIXER
#define SDL_HAS_64BIT_TYPE int
#define USE_MAP
#define USE_PYTHON

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
%include "SDL_keysym.h"
%include "input.h"
%include "audio.h"
%include "character_base.h"
%include "quest.h"
%include "screen.h"
%include "image.h"
%include "animation.h"
%include "mapobject.h"
%include "mapcharacter.h"
%include "character.h"
%include "landmap.h"
%include "mapview.h"
%include "mapengine.h"
%include "callback_wrap.h"
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
%include "data.h"
