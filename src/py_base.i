%module base
%{

#include <string>
#include "types.h"
#include "game.h"
#include "event.h"
#include "fileops.h"
#include "gamedate.h"
#include "gametime.h"
#include "schedule.h"

%}

%include "py_wrappers_base.i"

%include "event.h"
%include "game.h"
%include "fileops.h"
%include "gametime.h"
%include "gamedate.h"
%include "schedule.h"
