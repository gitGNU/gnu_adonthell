%module win_label

%{

#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"
#include "win_theme.h"
#include "win_border.h"
#include "win_background.h"
#include "win_base.h"
#include "win_label.h"

%}

#define u_int8 unsigned char
#define s_int8 signed char
#define u_int16 unsigned short
#define s_int16 short
#define u_int32 unsigned long
#define s_int32 signed long

%include "win_types.h"
%include "win_base.h"
%include "win_label.h"