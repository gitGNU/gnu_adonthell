#include <stdio.h>
#include <unistd.h>
#include <list>
#include <stdlib.h>
#include "SDL_thread.h"
#include "types.h"
#include "image.h"
#include "input.h"
#include "prefs.h"
#include "game.h"
#include "win_types.h"
#include "win_base.h"
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_image.h"
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

  screen::set_video_mode(640,480);
  image background;
  image ti;
  
  background.load("gfxtree/window/editor/back_test.pnm");
  ti.load("gfxtree/window/editor/testimage.pnm");
  
  //load font
  win_font * testfont = new win_font(WIN_THEME_ORIGINAL);

  //load border
  win_border * bord1 = new win_border(WIN_THEME_ORIGINAL);
  win_border * bord2 = new win_border(WIN_THEME_ORIGINAL);
  
  //load background
  win_background * back1 = new win_background(WIN_THEME_ORIGINAL);

  //new container
  win_container * cont1=new win_container(50,50,120,120);

  
  //new label and configuration
  win_label * lab1=cont1->add_label(70,70,50,50,testfont);
  lab1->set_text("Ca Fonctionne, ouais !!!!!!!!!!!!!!!");
  lab1->set_border(bord1);
  lab1->set_background(back1);
  cont1->set_border(bord2);
  //cont1->set_background(back1);
 
  cont1->show_all();

  while(!input::is_pushed(Escape_Key))
    {
      
      input::update();
      cont1->update();
      background.putbox(160,40);
      cont1->draw();
      
      if(input::is_pushed(A_Key))
	{
	 lab1->move(20,20);
	}
      if(input::is_pushed(Z_Key))
	{
	  lab1->resize(70,70);
	}
      if(input::is_pushed(E_Key))
	{
	  cont1->move(180,100);
	}
      if(input::is_pushed(R_Key))
	{
	  cont1->resize(123,123);
	}
      
      
      screen::show();
    
      screen::drawbox(0,0,640,480,0x000000);	
    
    }

}
