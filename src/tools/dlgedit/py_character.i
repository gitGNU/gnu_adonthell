%module character

%{

#include "types.h"
#include "character.h"
#include "storage.h"
#include "dialog_engine.h"

%}

#define u_int8 unsigned char
#define u_int16 unsigned short
#define u_int32 unsigned long

class storage
{
public:
    void set (const char*, int);
    int get (const char*);
};

%include "../../character.h"
