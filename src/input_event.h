#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H


class input_event
{
private:
    input_type Type;
    
public:
    typedef enum
    {
        KEYBOARD_EVENT,
        MOUSE_EVENT,
        JOYSTICK_EVENT,
        CONTROL_EVENT, 
        NBR_INPUT_TYPES
    } input_type;
    
    input_event (input_type t)
    {
        Type = t; 
    }

    input_type type () 
    {
        return Type; 
    }
}; 

#endif
