%module character

%{

#include "types.h"
#include "character.h"
#include "storage.h"
#include "dialog_engine.h"
#include "win_theme.h"

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

%include "character.h"

%addmethods npc {
    // Start conversation with the NPC
    void talk ()
    {
        dialog_engine *de = new dialog_engine (self, new win_theme (win_theme::theme));
        de->run ();
    }
}

