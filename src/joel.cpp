#include <stdio.h>
#include <unistd.h>
#include <list>
#include <stdlib.h>
#include "image.h"
#include "pnm.h"
#include "input.h"
#include "types.h"
#include "SDL_thread.h"
#include "prefs.h"
#include "game.h"
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_container.h"
#include "win_select.h"

int main(int argc,char **argv)
{
  
  config myconfig (argc > 1 ? argv[1] : "");
  
  // try to read adonthellrc
  myconfig.read_adonthellrc ();
  
  // change into data directory
  chdir (myconfig.datadir.data ());
  
  //clavier

  SDL_Thread * input_thread;
  input_thread = SDL_CreateThread(input_update, NULL);
  if ( input_thread != NULL) {
    printf("User input thread started.\n");
  } else {
    printf("Couldn't create input thread: %s\n", SDL_GetError());
    exit(1);
  }

  screen::set_video_mode(640,480);
  image background;
  
  
  background.load("gfxtree/window/editor/back_test.pnm");


  //load font
  win_font * testfont = new win_font(WIN_THEME_ORIGINAL);
 
  //new container
  win_container * cont1=new win_container(0,0,50,50);

  //new label and configuration
  win_label * lab1=cont1->add_label(25,25,50,50,testfont);

  lab1->show();
  lab1->set_text("Ca Fonctionne, ouais !!!!!!!!!!!!!!!");

  //new write
   
  //show container
  cont1->show();

  
  //create border
  win_border * bord1=cont1->set_border(WIN_THEME_ORIGINAL);
  bord1->show();

 
  //create background
  win_background * back1 = cont1->set_background(WIN_THEME_ORIGINAL);
  back1->show();

 
  //sleep(10);
  /*
  //win select
  win_select * testselect;
  testselect->add(wri1);
  testselect->add(lab1);
 

  */

  //cont1->remove_container(cont3);

  //cont1->remove_label(lab1);

  while(!input::is_pushed(Escape_Key))
    {
     
      cont1->update();
    
      background.putbox(160,40);
   
      cont1->draw();
      
    
      screen::show();
    
      screen::drawbox(0,0,640,480,0x000000);	
    
    }

  printf("Killing threads...\n");
  if (input_thread != NULL) SDL_KillThread(input_thread);
}














