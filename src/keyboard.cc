#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "gfx.h"
#include "keyboard.h"
#ifdef SDL
#include "SDL.h"
#endif

bool keyboard::charkeys[65536];

#ifdef SDL
u_int16 keyboard::last_key;

int keyboard_init(void *) {

  SDL_Event event;
  SDL_EnableUNICODE(1);

  // All key events are saved in the "charkeys" variable in the
  // keyboard namespace.
  while ( SDL_WaitEvent(&event) ) {

    switch (event.type) {
      case SDL_KEYDOWN:
         keyboard::charkeys[event.key.keysym.sym]=1;
	 keyboard::last_key = event.key.keysym.sym;
         break;
      case SDL_KEYUP:
         keyboard::charkeys[event.key.keysym.sym]=0;
         break;
    }
  }
  return(0);
}

void keyboard::update_keyboard() {}
u_int16 keyboard::getkeypressed() { return(last_key); }

#else

#include <ggi/ggi.h>

void keyboard::update_keyboard()
{
  ggi_event event;
  ggi_event_mask mask;
  struct timeval t={0,0};
  while((mask=ggiEventPoll(screen::vis,emKey,&t)))
    {
      ggiEventRead(screen::vis,&event,emKey);
      if(event.any.type==evKeyPress)
	{
	  charkeys[event.key.sym]=1;
	}else
      if(event.any.type==evKeyRelease)
	{
	  charkeys[event.key.sym]=0;
	}
    }
}

u_int16 keyboard::getkeypressed()
{
  u_int16 c=0;
  ggi_event event;
  ggi_event_mask mask;
  struct timeval t={0,0};
  
  if((mask=ggiEventPoll(screen::vis,emKey,&t)))
    {
      ggiEventRead(screen::vis,&event,emKey);
      if(event.any.type==evKeyPress)
	{
	  c=event.key.sym;
	  if(c==Shift_Key) charkeys[Shift_Key]=1;
	}
      if(event.any.type==evKeyRelease)
	{
	  charkeys[event.key.sym]=0;
	}
    }
  return(c);
}
#endif

bool keyboard::is_pushed(u_int16 key)
{
  return(charkeys[key]);
}
