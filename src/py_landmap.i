%module landmap

%{

#include "character.h"
#include "landmap.h"
#include "mapview.h"

%}

#define u_int8 unsigned char
#define s_int8 signed char
#define u_int16 unsigned short
#define s_int16 short
#define u_int32 unsigned long
#define s_int32 signed long

%include "event.h"
%include "mapcharacter.h"
%include "character.h"
%include "landmap.h"
%include "mapview.h"

