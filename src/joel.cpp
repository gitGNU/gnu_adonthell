#include <stdio.h>
#include <unistd.h>
#include <list>
#include <stdlib.h>
#include "SDL_thread.h"
#include "types.h"
#include "screen.h"
#include "image.h"
#include "input.h"
#include "prefs.h"
#include "win_types.h"
#include "win_base.h"
#include "win_image.h"
#include "win_border.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_theme.h"
#include "win_container.h"
#include "win_scrolled.h"
#include "win_select.h"
#include "win_file.h"

void toto()
{
  cout << "Ca marche\n";
}

int main(int argc,char **argv)
{
  
  config myconfig (argc > 1 ? argv[1] : "");
  
  // try to read adonthellrc
  myconfig.read_adonthellrc ();
  // change into data directory
  chdir (myconfig.datadir.data ());
  //clavier
  screen::set_video_mode(320,240);
  input::init();
  win_theme th(WIN_THEME_ORIGINAL);
  win_font font(WIN_THEME_ORIGINAL);
  
  win_file * wf=new win_file(10,10,200,200,&th,&font);
  wf->set_visible(true);
  wf->set_activated(true);
  
  

  while(!(input::has_been_pushed(SDLK_ESCAPE)))
    {    
      wf->update();
      input::update();
      if((input::has_been_pushed(SDLK_m))) wf->move(wf->x()+10,wf->y()+10);
      screen::drawbox(0,0,640,480,0x00A100);	
      wf->draw();
      screen::show();
    }
  
  delete wf;
}
