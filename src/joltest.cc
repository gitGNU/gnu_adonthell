#include <iostream>
#include <stdio.h>

#include "landmap.h"
#include "image.h"
#include "screen.h"
#include "atk_font.h"

#include "atk_widget.h"
#include "atk_box.h"
#include "atk_fixed.h"
#include "callback.h"
#include "callback_slot.h"
#include "callback_sig.h"

class A
{
public : 
    void toto () { cout << "toto" <<  endl; }
    
    void aff (char * c) { cout <<  c <<  endl;} 
}; 


int main (int argc, char * argv[]) 
{
 /*   if (argc != 2)
    {
    cout << argv[0] << " <path/file.ttf>\n";   
        exit (0); 
    }
   */ 
    screen::init (); 
    screen::set_video_mode (640, 480, 16); 
    screen::clear (); 

    
    image im;
    im.resize (640, 480);
    im.fillrect (0, 0, 640, 480, 127, 127, 127);
    im.draw (0, 0); 

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
       font.draw (", ? ; . : / ! § % ù $ £ ¤ ^ } { # ~ @ µ * - + & ' < > ", 10,350 );
    */


    

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
     
       
       /***************************************/
    /***************************************/
    /************CALLBACK STUFF*************/
    /***************************************/
    /***************************************/
    // cout << "/***************************************/\n"; 
    // cout << "/***************************************/\n"; 
    // cout << "CALLBACK STUFF : \n"; 
    /*
    A a;
    callback_sig cbs; 

    char *ch = "Kewl it's working"; 

    cbs.connect (new  callback_slot (makeFunctor (a, &A::toto)));    
    cbs.connect (new  callback_slot1 <char* > (makeFunctor (a, &A::aff),ch));
    
    cbs ();
    */
    getchar (); 


    delete box; 

    return 0; 
}



