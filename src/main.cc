#include <iostream.h>
#include <stdio.h>
#include "types.h"
#include "keyboard.h"
#include "gfx.h"
#include "mappattern.h"
#include "mapcharacter.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "window.h"
#include "map.h"
#include "mapengine.h"
#ifdef SDL
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_mixer.h"
#include "audio.h"
#endif

void init_display(u_int8);

int main(int argc, char * argv[])
{
  map map1;
  if((argc!=2)&&(argc!=3)) {printf("Usage: %s mapname\n",argv[0]); exit(0);}
  if(argc==3)
  {
    if(!strcmp(argv[2],"-m1")) screen::init_display(1);
    if(!strcmp(argv[2],"-m0")) screen::init_display(0);
  }
  else screen::init_display(0);
  if(map1.load(argv[1])) {printf("Error loading %s!\n",argv[1]);return(1);}


#ifdef SDL
  SDL_Thread *input_thread;
  SDL_Thread *audio_thread;

  // Initialize SDL library
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
     fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
     exit(1);
  }

  init_display(0);

  audio_thread = SDL_CreateThread((void*)audio_init, NULL);
  if ( audio_thread != NULL) {
     fprintf(stderr, "Audio thread started.\n");
  } else {
     fprintf(stderr, "Couldn't create audio thread: %s\n", SDL_GetError());
     fprintf(stderr, "Audio will not be used\n");
  }

  input_thread = SDL_CreateThread((void*)keyboard_init, NULL);
  if ( input_thread != NULL) {
     fprintf(stderr, "User input thread started.\n");
  } else {
     fprintf(stderr, "Couldn't create input thread: %s\n", SDL_GetError());
     return(0);
  }

#endif
  mapengine::map_engine(&map1);
#ifdef SDL
  fprintf(stderr, "Killing threads...\n");
  if (input_thread != NULL) SDL_KillThread(input_thread);
  if (audio_thread != NULL) {
    SDL_KillThread(audio_thread);
    audio_cleanup();
    Mix_CloseAudio();
  }
  SDL_Quit();
#endif
  return(0);
}

#ifdef SDL
void init_display(u_int8 vidmode = 0)
{
//  u_int16 i;
  SDL_Surface *screen;

/*  switch (vidmode)
    {
    case 0:
      screenwidth = 320;
      screenheight = 200;
      sizefactor = 1;
      break;
    case 1:
      screenwidth = 640;
      screenheight = 400;
      sizefactor = 2;
      break;
    default:    */               /*printdisclaimer(); */
//      exit (1);
//    }

    // Try setting screen resolution to 16 bit
    screen = SDL_SetVideoMode(320, 200, 16, SDL_SWSURFACE);

    if (screen != NULL)
    {
      fprintf (stderr, "Using 16 bit color at 320 x 200.\n");
//      bytes_per_pixel = 2;
//      trans=0xF81F;  // That awful purple is the transparency color
    } else {
      // OK, try 24 bit since 16 wasn't supported
      screen = SDL_SetVideoMode(320, 200, 24, SDL_SWSURFACE);
      if (screen != NULL)
      {
        fprintf (stderr, "Using 24 bit color at %d x %d.\n", 320, 200);
//        bytes_per_pixel = 3;
//        trans=0xFFFF00;  // That awful purple is the transparency color
      } else {
        // Neither 16 or 24 bit color supported. Exitting...
        fprintf(stderr, "Your display doesn't support 16 or 24 bit color at 320 x 200. Exitting...\n");
        exit(1);
      }
  }
}
#endif
