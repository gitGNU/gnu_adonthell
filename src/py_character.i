%module character

%{

#include "types.h"
#include "mapcharacter.h"
#include "character.h"
#include "storage.h"
#include "dialog_engine.h"
#include "win_theme.h"

%}

#define u_int8 unsigned char
#define s_int8 signed char
#define u_int16 unsigned short
#define s_int16 short
#define u_int32 unsigned long
#define s_int32 signed long

%include "storage.h"
%include "mapcharacter.h"
%include "character.h"

%addmethods npc {
    // Start conversation with the NPC
    void talk ()
    {
        dialog_engine *de = new dialog_engine (self, new win_theme (win_theme::theme));
        de->run ();
    }
}

