%module image

%{

#include "types.h"
#include "image.h"

%}

#define u_int8 unsigned char
#define u_int16 unsigned short
#define s_int16 short
#define u_int32 unsigned long

%include "image.h"