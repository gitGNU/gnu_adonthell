%module player

%{

#include "types.h"
#include "character.h"
#include "storage.h"

%}

class storage
{
public:
    void set (const char*, int);
    int get (const char*);
};

%include "character.h"