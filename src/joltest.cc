#include <iostream>
#include <stdio.h>

#include "gfx/image.h"
#include "gfx/screen.h"
#include "atk_font.h"

#include "atk_widget.h"
#include "atk_box.h"
#include "atk_fixed.h"
#include "atk_manager.h"
#include "atk_window.h"
#include "atk_button_template.h"
#include "atk_border_template.h"
#include "atk_background_template.h"
#include "atk_theme.h"
#include "atk_button.h"
#include "gametime.h"


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
    
    /**
     * Font 
     */
    atk_font font;
    if (!font.load (argv[1]))
      {
	std::cout << "Error in loading font ...\n";
	exit (1);
      }
    font.set_size (12);
    font.build ();


    atk_theme theme;
    theme.load ("adontest/original.theme");  
    
    atk_manager manager;  

    /**
       CREATE BUTTON
     */
    atk_border_template * bd_press = new atk_border_template; 
    bd_press->set_name ("pressed"); 
    bd_press->set (atk_border_template::B_TOP, "widgets/button_pressed_top.pnm");
    bd_press->set (atk_border_template::B_RIGHT, "widgets/button_pressed_right.pnm");
    bd_press->set (atk_border_template::B_LEFT, "widgets/button_pressed_left.pnm");
    bd_press->set (atk_border_template::B_BOTTOM, "widgets/button_pressed_bot.pnm");
    bd_press->set (atk_border_template::C_TL, "widgets/button_pressed_tl.pnm");
    bd_press->set (atk_border_template::C_TR, "widgets/button_pressed_tr.pnm");
    bd_press->set (atk_border_template::C_BL, "widgets/button_pressed_bl.pnm");
    bd_press->set (atk_border_template::C_BR, "widgets/button_pressed_br.pnm");
    bd_press->build();
    
    atk_border_template * bd_released = new atk_border_template; 
    bd_released->set_name ("released"); 
    bd_released->set (atk_border_template::B_TOP, "widgets/button_top.pnm");
    bd_released->set (atk_border_template::B_RIGHT, "widgets/button_right.pnm");
    bd_released->set (atk_border_template::B_LEFT, "widgets/button_left.pnm");
    bd_released->set (atk_border_template::B_BOTTOM, "widgets/button_bot.pnm");
    bd_released->set (atk_border_template::C_TL, "widgets/button_tl.pnm");
    bd_released->set (atk_border_template::C_TR, "widgets/button_tr.pnm");
    bd_released->set (atk_border_template::C_BL, "widgets/button_bl.pnm");
    bd_released->set (atk_border_template::C_BR, "widgets/button_br.pnm");
    bd_released->build();

   

    atk_background_template * ba_tmp = new atk_background_template();
    ba_tmp->set_color (192, 165, 57);

    atk_button_template bt_tmp;
    bt_tmp.set_released (bd_released);
    bt_tmp.set_pressed (bd_press);
    bt_tmp.set_background (ba_tmp);


    

    atk_window * wnd =  new atk_window; 
    
    
    wnd->set_visible (true);
    wnd->set_position (20, 40);
    wnd->set_size (350, 200);
    wnd->on_delete.connect (new callback_slot (makeFunctor (manager, &atk_manager::shutdown))); 
    wnd->set_border_ui (theme.get_border("normal"));
    
    

    atk_box * box = new atk_box; 
    box->set_border_width (10); 
    box->set_spacing (3); 
    box->set_visible (true); 
    box->set_size (200, 60);
    box->set_position (50, 50); 
    box->set_geometry (atk_box::VERTICAL); 
    box->set_border_ui (theme.get_border("mini"));
    
    atk_widget * wid; 
    
    wid = new atk_widget;
    wid->set_size (30, 50);
    wid->set_visible (true);
    wid->realize (); 
    box->add_start (wid, true, true, 0); 

    
    /*wid = new atk_widget;
    wid->set_size (30, 20);
    wid->set_visible (true);
    wid->realize (); 
    box->add_start (wid, true, true, 0); 
*/
    atk_button * but;
    but = new atk_button;
    but->set_size (30, 20);
    but->set_visible (true);
    but->realize (); 
    but->set_button_ui ( &bt_tmp);
    but->set_text ("Quit ", &font);
    box->add_start (but, true, true, 0); 
    
    wid = new atk_widget;
    wid->set_size (10, 50);
    wid->set_visible (true);
    wid->realize (); 
    box->add_end (wid, true, true, 15) ; 
 
    box->realize ();  
    

    wnd->add (box);
    wnd->set_resizeable (true); 
    wnd->realize (); 
    
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
      atk_theme theme;
    theme.set_name ("silverleaf");
    
    atk_border_template *  border;
    border =  new atk_border_template; 
    border->set_name ("mini"); 
    border->set (atk_border_template::B_TOP, "silverleaf/mini/h_border.pnm");
    border->set (atk_border_template::B_BOTTOM, "silverleaf/mini/h_border.pnm");
    border->set (atk_border_template::B_LEFT, "silverleaf/mini/v_border.pnm");
    border->set (atk_border_template::B_RIGHT, "silverleaf/mini/v_border.pnm");
    border->set (atk_border_template::C_TL, "silverleaf/mini/corner_top_left.pnm");
    border->set (atk_border_template::C_TR, "silverleaf/mini/corner_top_right.pnm");
    border->set (atk_border_template::C_BL, "silverleaf/mini/corner_bottom_left.pnm");
    border->set (atk_border_template::C_BR, "silverleaf/mini/corner_bottom_right.pnm");
    border->set (atk_border_template::C_EXIT, "silverleaf/mini/corner_top_left.pnm");
    border->set (atk_border_template::C_RESIZE, "silverleaf/mini/corner_bottom_right.pnm");
    theme.add_border (border);
    
    border =  new atk_border_template; 
    border->set_name ("normal"); 
    border->set (atk_border_template::B_TOP, "silverleaf/normal/h_border.pnm");
    border->set (atk_border_template::B_BOTTOM, "silverleaf/normal/h_border.pnm");
    border->set (atk_border_template::B_LEFT, "silverleaf/normal/v_border.pnm");
    border->set (atk_border_template::B_RIGHT, "silverleaf/normal/v_border.pnm");
    border->set (atk_border_template::C_TL, "silverleaf/normal/corner_top_left.pnm");
    border->set (atk_border_template::C_TR, "silverleaf/normal/corner_top_right.pnm");
    border->set (atk_border_template::C_BL, "silverleaf/normal/corner_bottom_left.pnm");
    border->set (atk_border_template::C_BR, "silverleaf/normal/corner_bottom_right.pnm");
    border->set (atk_border_template::C_EXIT, "silverleaf/normal/corner_top_left.pnm");
    border->set (atk_border_template::C_RESIZE, "silverleaf/normal/corner_bottom_right.pnm");
    theme.add_border (border); 


    
    theme.save ("adontest/silverleaf.theme"); 

    theme.display_info (); 
    
    */
    
    /**
     * load a theme
     **/
    /*
      atk_theme theme;

      theme.load ("adontest/original.theme");  
      theme.display_info (); 

      std::cout << "\n\nLoad another theme\n\n"; 
      
      theme.load ("adontest/silverleaf.theme");
      theme.display_info (); 
    */
    return 0; 
    

}



