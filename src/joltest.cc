#include <iostream>
#include <stdio.h>

#include "image.h"
#include "screen.h"
#include "atk_font.h"

#include "atk_widget.h"
#include "atk_box.h"
#include "atk_fixed.h"
#include "atk_manager.h"
#include "atk_window.h"
#include "gametime.h"


int main (int argc, char * argv[]) 
{
    screen::init ();
    screen::set_video_mode (640, 480, 16); 
    screen::clear (); 
    
    
    atk_manager manager;  
    

    atk_window * wnd =  new atk_window; 
    
    wnd->set_visible (true);
    wnd->set_position (20, 40);
    wnd->set_size (350, 200);
    wnd->on_delete.connect (new callback_slot (makeFunctor (manager, &atk_manager::shutdown))); 
    

    atk_box * box = new atk_box; 
    box->set_border_width (10); 
    box->set_spacing (3); 
    box->set_visible (true); 
    box->set_size (200, 60);
    box->set_position (50, 50); 
    box->set_geometry (atk_box::VERTICAL); 
    
    
    atk_widget * wid; 

    wid = new atk_widget;
    wid->set_size (30, 50);
    wid->set_visible (true);
    wid->realize (); 
    box->add_start (wid, true, true, 0); 

    
    wid = new atk_widget;
    wid->set_size (30, 20);
    wid->set_visible (true);
    wid->realize (); 
    box->add_start (wid, true, true, 0); 

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
        input_manager::update(); 

        gametime::update (); 
        
        screen::display.fillrect (0, 0, 640, 480, 127, 127, 127);
        

        manager.draw (); 
        
        screen::show ();
        
        screen::clear ();     
    }

    input_manager::cleanup();
    
    
    /* 
       atk_font font;     
       if (!font.load (argv[1])) exit (1);  
       
       font.info ();
       
       font.set_size (80);  
       font.set_color (0, 0, 0);  
       font.build (); 
       font.draw ("Adonthell Power !!!", 40, 70); 
       
       
       font.set_size (26);  
       font.set_color (45, 167, 100);  
       font.build (); 
       font.draw ("It's working ...", 200, 120); 
       
       font.set_size (20);  
       font.set_color (145, 200, 45);  
       font.build (); 
       font.draw ("Kay,\n Ben,\n  Alex,\n   James\n     I know ...", 140, 200); 
       
       
       font.set_size (36);  
       font.set_color (239, 16, 53);  
       font.build (); 
       font.draw ("I'm Fired !!!;=) ", 170, 320); 
       
       font.set_size (16);  
       font.set_color (200, 200, 200);  
       font.build ();  
       font.draw (", ? ; . : / ! � % � $ � � ^ } { # ~ @ � * - + & ' < > ", 10,350 );
    */


    

    
/*
    atk_box * box = new atk_box;
    
    atk_fixed * fixed = new atk_fixed;

    fixed->set_size (100, 100);
    fixed->set_position (300, 300); 
    fixed->set_visible(true);	
    fixed->draw (); 
    
    box->set_border_width (10); 
    box->set_spacing (3); 
    box->set_visible (true); 
    box->set_size (200, 60);
    box->set_position (50, 50); 
    box->set_geometry (atk_box::VERTICAL); 
    
    atk_widget * wid;

    wid = new atk_widget;
    wid->set_size (30, 50);
    wid->set_visible (true);
    wid->realize (); 
    box->add_start (wid, true, true, 0); 

    wid = new atk_widget;
    wid->set_size (30, 20);
    wid->set_visible (true);
    wid->realize (); 
    box->add_start (wid, true, true, 0); 

    wid = new atk_widget;
    wid->set_size (10, 50);
    wid->set_visible (true);
    wid->realize (); 
    box->add_end (wid, true, true, 15) ; 


    box->realize (); 
    box->draw ();  

    
    screen::show ();


    
    delete box; 
*/
    return 0; 
}


