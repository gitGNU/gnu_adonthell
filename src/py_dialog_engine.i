%module dialog_engine

%{

#include "types.h"
#include "character.h"
#include "win_types.h"
#include "win_theme.h"
#include "win_base.h"
#include "win_container.h"
#include "dialog_engine.h"

%}

#define u_int32 unsigned long
#define u_int8 unsigned char

%include "character.h"
%include "win_theme.h"
%include "win_base.h"
%include "win_container.h"
%include "dialog_engine.h"
