#include <iostream>
#include <stdio.h>

#include "gfx/image.h"
#include "gfx/screen.h"
#include "gui/font.h"

#include "gui/widget.h"
#include "gui/box.h"
#include "gui/fixed.h"
#include "gui/manager.h"
#include "gui/window.h"
#include "gui/button_template.h"
#include "gui/border_template.h"
#include "gui/background_template.h"
#include "gui/theme.h"
#include "gui/button.h"
#include "gametime.h"

using namespace gui;

int main (int argc, char * argv[]) 
{

  gfx::screen::init ();
  gfx::screen::set_video_mode (640, 480, 16); 
  gfx::screen::clear (); 
    
  if (argc != 2) 
    {
      std::cout << "Please need TTF font as argument\n";
      exit (1);
    }
    
  /***********************************************/
  /*********************FONT  ********************/
  /***********************************************/
  gui::font font;
  if (!font.load (argv[1]))
    {
      std::cout << "Error in loading font ...\n";
      exit (1);
    }
  
  font.set_size (20);
  font.build ();
  
  FILE * fd = fopen ("/home/jol/toto.utf8", "r");
  char t[255];
  fgets(t, 255, fd);
  fclose (fd);
  

  /***********************************************/
  /********************* THEME *******************/
  /***********************************************/
  gui::theme theme;
  theme.load ("adontest/original.theme");  
 
   /**
     CREATE BUTTON THEME
  */
  gui::border_template * bd_press = new gui::border_template; 
  bd_press->set_name ("pressed"); 
  bd_press->set (border_template::B_TOP, "widgets/button_pressed_top.pnm");
  bd_press->set (border_template::B_RIGHT, "widgets/button_pressed_right.pnm");
  bd_press->set (border_template::B_LEFT, "widgets/button_pressed_left.pnm");
  bd_press->set (border_template::B_BOTTOM, "widgets/button_pressed_bot.pnm");
  bd_press->set (border_template::C_TL, "widgets/button_pressed_tl.pnm");
  bd_press->set (border_template::C_TR, "widgets/button_pressed_tr.pnm");
  bd_press->set (border_template::C_BL, "widgets/button_pressed_bl.pnm");
  bd_press->set (border_template::C_BR, "widgets/button_pressed_br.pnm");
  bd_press->build();
  
  gui::border_template * bd_released = new gui::border_template; 
  bd_released->set_name ("released"); 
  bd_released->set (border_template::B_TOP, "widgets/button_top.pnm");
  bd_released->set (border_template::B_RIGHT, "widgets/button_right.pnm");
  bd_released->set (border_template::B_LEFT, "widgets/button_left.pnm");
  bd_released->set (border_template::B_BOTTOM, "widgets/button_bot.pnm");
  bd_released->set (border_template::C_TL, "widgets/button_tl.pnm");
  bd_released->set (border_template::C_TR, "widgets/button_tr.pnm");
  bd_released->set (border_template::C_BL, "widgets/button_bl.pnm");
  bd_released->set (border_template::C_BR, "widgets/button_br.pnm");
  bd_released->build();
  
   
  /* create the background associated at the button */
  gui::background_template * ba_tmp = new gui::background_template();
  ba_tmp->set_color (192, 165, 57);
  
  button_template bt_tmp;
  bt_tmp.set_released (bd_released);
  bt_tmp.set_pressed (bd_press);
  bt_tmp.set_background (ba_tmp);
  bt_tmp.set_name ("standart");
  

  /***********************************************/
  /********************* MANAGER******************/
  /***********************************************/
  gui::manager manager; 
  
  
  /***********************************************/
  /********************* INTERFACE ***************/
  /***********************************************/
   
  /* define the window */
  gui::window * wnd =  new gui::window; 
  wnd->set_visible (true);
  wnd->set_position (20, 40);
  wnd->set_size (350, 200);
  wnd->on_delete.connect (new callback_slot (makeFunctor (manager, &manager::shutdown))); 
  wnd->set_border_ui (theme.get_border("normal"));
  
  /* create a box */
  gui::box * box = new gui::box; 
  box->set_border_width (10); 
  box->set_spacing (3); 
  box->set_visible (true); 
  box->set_size (200, 60);
  box->set_position (0, 0); 
  box->set_geometry (box::VERTICAL); 
  box->set_border_ui (theme.get_border("mini"));
  
  
  /* create some widget and add them at the box */
  gui::widget * wid; 
  wid = new gui::widget;
  wid->set_size (30, 50);
  wid->set_visible (true);
  wid->realize (); 
  box->add_start (wid, true, true, 0); 
  
  wid = new widget;
  wid->set_size (30, 20);
  wid->set_visible (true);
  wid->realize (); 
  box->add_start (wid, true, true, 0); 

  
  /* create a button */
  gui::button * but;
  but = new gui::button;
  but->set_size (30, 20);
  but->set_visible (true);
  but->set_button_ui ( &bt_tmp);
  but->set_text ("Quit ", &font);
  but->realize ();
  box->add_start (but, false, false); 

  /* build the box */
  box->realize ();  
  
  /* add the box and realize the window */
  wnd->add (box);
  wnd->set_resizeable (true); 
  wnd->realize (); 
  
  /* add the window to the window manager */
  manager.add (wnd); 
  
  while (manager.update () )
    {
      input::manager::update(); 
      
      gametime::update (); 
      
      gfx::screen::display.fillrect (0, 0, 640, 480, 127, 127, 127);
      
      manager.draw (); 
      
      gfx::screen::show ();
      
      gfx::screen::clear ();     
    }
  
  input::manager::cleanup();
  








  /**
   * SAVE A THEME
   */
  /*
    theme theme;
    theme.set_name ("silverleaf");
    
    border_template *  border;
    border =  new border_template; 
    border->set_name ("mini"); 
    border->set (border_template::B_TOP, "silverleaf/mini/h_border.pnm");
    border->set (border_template::B_BOTTOM, "silverleaf/mini/h_border.pnm");
    border->set (border_template::B_LEFT, "silverleaf/mini/v_border.pnm");
    border->set (border_template::B_RIGHT, "silverleaf/mini/v_border.pnm");
    border->set (border_template::C_TL, "silverleaf/mini/corner_top_left.pnm");
    border->set (border_template::C_TR, "silverleaf/mini/corner_top_right.pnm");
    border->set (border_template::C_BL, "silverleaf/mini/corner_bottom_left.pnm");
    border->set (border_template::C_BR, "silverleaf/mini/corner_bottom_right.pnm");
    border->set (border_template::C_EXIT, "silverleaf/mini/corner_top_left.pnm");
    border->set (border_template::C_RESIZE, "silverleaf/mini/corner_bottom_right.pnm");
    theme.add_border (border);
    
    border =  new border_template; 
    border->set_name ("normal"); 
    border->set (border_template::B_TOP, "silverleaf/normal/h_border.pnm");
    border->set (border_template::B_BOTTOM, "silverleaf/normal/h_border.pnm");
    border->set (border_template::B_LEFT, "silverleaf/normal/v_border.pnm");
    border->set (border_template::B_RIGHT, "silverleaf/normal/v_border.pnm");
    border->set (border_template::C_TL, "silverleaf/normal/corner_top_left.pnm");
    border->set (border_template::C_TR, "silverleaf/normal/corner_top_right.pnm");
    border->set (border_template::C_BL, "silverleaf/normal/corner_bottom_left.pnm");
    border->set (border_template::C_BR, "silverleaf/normal/corner_bottom_right.pnm");
    border->set (border_template::C_EXIT, "silverleaf/normal/corner_top_left.pnm");
    border->set (border_template::C_RESIZE, "silverleaf/normal/corner_bottom_right.pnm");
    theme.add_border (border); 


    
    theme.save ("adontest/silverleaf.theme"); 

    theme.display_info (); 
    
  */
    
  /**
   * load a theme
   **/
  /*
    theme theme;

    theme.load ("adontest/original.theme");  
    theme.display_info (); 

    std::cout << "\n\nLoad another theme\n\n"; 
      
    theme.load ("adontest/silverleaf.theme");
    theme.display_info (); 
  */
  return 0; 
    

}



