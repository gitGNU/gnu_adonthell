#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <list>
#include "types.h"
#include "animation.h"
#include "prefs.h"
#include "input.h"
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_image.h"
#include "win_container.h"
#include "win_select.h"

int main(int argc, char * argv[])
{
  config myconfig (argc > 1 ? argv[1] : "");

  // try to read adonthellrc
  myconfig.read_adonthellrc ();
  
  // change into data directory
  chdir (myconfig.datadir.data ());

  screen::set_video_mode(320,200);

  win_font * font=new win_font(WIN_THEME_ORIGINAL);
  win_border * border=new win_border(WIN_THEME_ORIGINAL);
  win_border * border2=new win_border(WIN_THEME_ORIGINAL);
  win_background * background=new win_background(WIN_THEME_ORIGINAL);
  win_container * container=new win_container(10,10,150,100);
  win_label * label=container->add_label(70,20,100,60,font);
  label->set_text("Test label");
  label->set_border(border);
  container->set_border(border2);
  container->set_background(background);
  label->set_background(background);
  container->show_all();

  while(!input::is_pushed(Escape_Key))
    {
      
      screen::drawbox(0,0,320,200,0);
      input::update();
      container->update();
      container->draw();
      
      if(input::is_pushed(A_Key)) label->set_text("Ca marche!");
      if(input::is_pushed(Z_Key)) label->set_text("Test Label...");
      screen::show();
    }
  delete label;
  delete font;
  delete border;
  delete border2;
  delete background;
  delete container;
  return(0);
}
