%module input
%{

#include <string>
#include "types.h"
#include "input/event.h"
#include "input/listener.h"
#include "input/manager.h"

using namespace input;

%}

%include "py_wrappers_base.i"

%include "event.h"
%include "keyboard_event.h"
%include "mouse_event.h"
%include "joystick_event.h"
%include "control_event.h"
%include "listener.h"
%include "manager.h"
