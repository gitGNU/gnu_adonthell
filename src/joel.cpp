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
#include "win_font.h"
#include "win_label.h"
#include "win_write.h"
#include "win_border.h"
#include "win_image.h"
#include "win_select.h"
#include "win_container.h"
#include "win_file.h"

int main(int argc,char **argv)
{
  
  config myconfig (argc > 1 ? argv[1] : "");
  
  // try to read adonthellrc
  myconfig.read_adonthellrc ();
  
  // change into data directory
  chdir (myconfig.datadir.data ());
  
  //clavier
  screen::set_video_mode(640,480);
  input::init();
  
  //load font
  win_font * font = new win_font(WIN_THEME_ORIGINAL);
  
  //load border
  win_border * bord_norm = new win_border(WIN_THEME_ORIGINAL);
  win_border * bord_mini = new win_border(WIN_THEME_ORIGINAL,WIN_BORDER_MINI_SIZE);
  
  

  //set the first container 
  win_container * cont1=new win_container(20,20,400,400);
  cont1->set_border(bord_norm);
 
  //set the first wselect
  win_select * wselect1=new win_select(cont1,WIN_SELECT_JUST_SELECT);
  wselect1->activate_selection=true;//if false you can browse the selection
  
  win_label * lab;
  
  //set the second container
  win_container * cont2=cont1->add_container(10,10,70,70);
  cont2->set_border(bord_mini);//add border
  
  //create a new container which is attach to cont2.
  win_select * wselect2=wselect1->add_select(cont2);
  wselect2->set_border(bord_mini);
  
  //create label
  lab=cont2->add_label(10,10,50,20,font);
  lab->set_auto_size(true);
  lab->set_text("11111");
  wselect2->add(lab,WIN_SELECT_MODE_BORDER);
  wselect2->set_default_obj(lab);//set label like default select
  
  lab=NULL;
  lab=cont1->add_label(10,30,50,20,font);
  lab->set_auto_size(true);
  lab->set_text("22222");
  wselect2->add(lab,WIN_SELECT_MODE_BORDER);
  
  
  //create a new container
  win_container * cont3=cont1->add_container(10,100,70,70);
  cont3->set_border(bord_mini);
  
  //create a new win_select and attach on cont3
  win_select * wselect3=wselect1->add_select(cont3);
  wselect3->set_border(bord_mini);
  
  //create label
  lab=cont3->add_label(10,10,50,20,font);
  lab->set_auto_size(true);
  lab->set_text("33333");
  wselect3->add(lab,WIN_SELECT_MODE_BORDER);//attach label on the new win_select
  wselect3->set_default_obj(lab);
  
  lab=NULL;
  lab=cont3->add_label(10,30,50,20,font);
  lab->set_auto_size(true);
  lab->set_text("4444");
  wselect3->add(lab,WIN_SELECT_MODE_BORDER);//attach label on the new win_select
  
  wselect1->set_default_obj(cont2);//set the default object to wselect1!!!!
  
  /******************************************/
  /*****************************************/
  /* You can change default key to browse selection, so you can change default in win_types.h
     or you can change the value of key in the program see win_select.h
  */

  cont1->show_all();
  while(!(input::has_been_pushed(SDLK_ESCAPE)))
    {    
      input::update();
      wselect1->update(); //very important to update wselect1
      cont1->update();    
      cont1->draw();
      screen::show();
      screen::drawbox(0,0,640,480,0x00A100);	
    }
  
  cont1->hide_all();
  
  //delete wfile;
  //  delete wselect1;
;
  
 delete cont1;
 delete wselect1;
 delete font;
 delete bord_norm;
 delete bord_mini;
}




