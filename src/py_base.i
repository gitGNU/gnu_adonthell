%module base
%{

#include <string>
#include "types.h"
#include "game.h"
#include "event.h"
#include "time_event.h"
#include "event_handler.h"
#include "fileops.h"
#include "gamedata.h"
#include "gamedate.h"
#include "gametime.h"
#include "schedule.h"
#include "storage.h"
#include "character_base.h"
#include "character.h"

%}

%include "py_wrappers_base.i"

%include "event.h"
%include "time_event.h"
%include "event_handler.h"
%include "game.h"
%include "fileops.h"
%include "gamedata.h"
%include "gametime.h"
%include "gamedate.h"
%include "schedule.h"
%include "storage.h"
%include "character_base.h"
%include "character.h"
