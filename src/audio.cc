#ifdef SDL

#include "audio.h"
#include "SDL.h"
#include "SDL_mixer.h"

Mix_Music *music;

int audio_init() {

  // Open the audio device
  if ( Mix_OpenAudio(22050, AUDIO_S16, 1, 512) < 0 ) {
    fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    fprintf(stderr, "Audio will not be used.\n");
    return(1);
  }

  music = Mix_LoadMUS("water.it");
  if (music == NULL) {
    fprintf(stderr, "Background music couldn't be loaded: %s\n", SDL_GetError());
    return(1);
  }

  // Keep looping music
  while (1==1) {
    if ( ! Mix_PlayingMusic() ) Mix_PlayMusic(music, 0);
    sleep(100);
  }

  return(0);
}

void audio_cleanup() {
  Mix_FreeMusic(music);
  Mix_CloseAudio();
}
#endif