#ifndef _instruction_h
#define _instruction_h

#include "instruction_class.h"

class mapcharacter;
class map;

#define cinstr(x) class x : public instruction  \
{                                               \
   public:                                      \
   x() : instruction() {};                      \
   void exec(mapcharacter*, map*, u_int16, u_int16); }   

cinstr(event_0);
cinstr(event_1);
cinstr(event_2);
cinstr(event_3);
cinstr(event_4);
cinstr(event_5);
cinstr(event_6);
cinstr(event_7);
cinstr(event_8);
cinstr(event_9);
cinstr(event_10);
cinstr(event_11);
cinstr(event_255);

#endif
