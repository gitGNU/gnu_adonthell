%module base
%{

#include <string>
#include "types.h"
#include "game.h"
#include "fileops.h"
#include "gametime.h"

%}

%include "py_wrappers_base.i"

%include "game.h"
%include "fileops.h"
%include "gametime.h"
