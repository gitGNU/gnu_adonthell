%module input
%{

#include <string>
#include "types.h"
#include "input_event.h"
#include "input_listener.h"
#include "input_manager.h"

%}

%include "py_wrappers_base.i"

%include "input_event.h"
%include "keyboard_event.h"
%include "mouse_event.h"
%include "joystick_event.h"
%include "control_event.h"
%include "input_listener.h"
%include "input_manager.h"
