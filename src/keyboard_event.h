#ifndef KEYBOARD_EVENT_H
#define KEYBOARD_EVENT_H

#include "input_event.h"

class keyboard_event : public input_event
{
public:
    typedef enum
    {
        KEY_PUSHED, KEY_RELEASED
    } event_type; 

    typedef int key_type; 
    
    keyboard_event (event_type t, key_type k) : input_event (KEYBOARD_EVENT)
    {
        Type = t;
        Key = k; 
    }

    event_type type () 
    {
        return Type; 
    }

    key_type key () 
    {
        return Key; 
    }
    
private:
    event_type Type;
    key_type Key; 
}; 

#endif
