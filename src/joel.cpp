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

class A
{
public:
  
  bool count()
  {
    static u_int16 cpt=0;
    return(cpt++<400);
  }
};

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
  

  win_label * test=new win_label(10,10,0,0,&th,&font);
  test->set_text("I wait the death !!!!");
  test->set_auto_size(true);
  A a;
  Functor0wRet<bool> tmp;
  test->set_callback_destroy(makeFunctor(&tmp,a,&A::count));

  win_container * wc = new win_container(20,20,170,170,&th);  
  wc->add(test);
  wc->set_visible_all(true);
  wc->set_border_visible(true);
  
  while(!(input::has_been_pushed(SDLK_ESCAPE)))
    {    
      wc->update();
      input::update();
      screen::drawbox(0,0,640,480,0x00A100);	
      wc->draw();
      screen::show();
    }
  
  delete wc;
}





