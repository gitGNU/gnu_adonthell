#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "input_event.h"

class input_listener
{
private:
    bool Listen_to[NBR_INPUT_TYPES];
    
public:
    input_listener ();
    ~input_listener ();

    void is_listening_to (input_type t) 
    {
        return Listen_to[t]; 
    }

    void listen_to (input_type t, bool val = true) 
    {
        Listen_to[t] = val; 
    }

    int raise_event (input_event * ev)
    {
        return 1; 
    }
};

#endif
