#include "landmap.h"
#include "animation.h"
#include "gametime.h"
#include "input_manager.h"

#include <algorithm>

struct mipcmp
{
    bool operator () (const mapsquare_info * mi1,
                      const mapsquare_info * mi2) 
    {
        return (*mi1) < (*mi2); 
    }
}; 

mipcmp boah; 

class game_client
{
public:
    map_character_with_gfx mchar;
    bool letsexit;

    game_client()
    {
        letsexit = false;
    }
    
    int callback_func(input_event * ev)
    {
        keyboard_event * kev = (keyboard_event *)ev;

        if (kev->type() == keyboard_event::KEY_PUSHED)
        {
            if (kev->key() == keyboard_event::H_KEY)
            {
                mchar.stop();
                mchar.set_state("say_hi");
                return 1;
            }
            if (kev->key() == keyboard_event::R_KEY)
            {
                mchar.run();
            }            
            if (kev->key() == keyboard_event::LEFT_KEY)
            {
                mchar.add_direction(mchar.WEST);
            }
            if (kev->key() == keyboard_event::RIGHT_KEY)
            {
                mchar.add_direction(mchar.EAST);
            }
            if (kev->key() == keyboard_event::UP_KEY)
            {
                mchar.add_direction(mchar.NORTH);
            }
            if (kev->key() == keyboard_event::DOWN_KEY)
            {
                mchar.add_direction(mchar.SOUTH);
            }
            if (kev->key() == keyboard_event::ESCAPE_KEY)
            {
                letsexit = true;
            }            
        }
        else
        {
            if (kev->key() == keyboard_event::H_KEY)
            {
                mchar.update_state();
            }
            if (kev->key() == keyboard_event::R_KEY)
            {
                mchar.walk();
            }            
            if (kev->key() == keyboard_event::LEFT_KEY)
            {
                mchar.remove_direction(mchar.WEST);
            }
            if (kev->key() == keyboard_event::RIGHT_KEY)
            {
                mchar.remove_direction(mchar.EAST);
            }
            if (kev->key() == keyboard_event::UP_KEY)
            {
                mchar.remove_direction(mchar.NORTH);
            }
            if (kev->key() == keyboard_event::DOWN_KEY)
            {
                mchar.remove_direction(mchar.SOUTH);
            }            
        }   

        mchar.update_state();

        return true;
    }

    int callback_mouse(input_event * ev)
    {
        mouse_event * me = (mouse_event *)ev;
        
        cout << me->x() << " " << me->y() << " ";
        if (me->type() == mouse_event::BUTTON_PUSHED) cout  << me->button_symbol()<< " pushed";
        else if (me->type() == mouse_event::BUTTON_RELEASED) cout  << me->button_symbol() << " released";
        cout << endl;

        return 1;
    }
};

int main (int argc, char * argv[]) 
{
    screen::init (); 
    screen::set_video_mode(640, 480);

    game_client gc;

    input_listener il;

    il.listen_to(input_event::KEYBOARD_EVENT);
    il.listen_to(input_event::MOUSE_EVENT);

    Functor1wRet<input_event *, int> fwr = makeFunctor(&fwr, gc, &game_client::callback_func);
    il.connect_function(input_event::KEYBOARD_EVENT, 
                        fwr);

    fwr = makeFunctor(&fwr, gc, &game_client::callback_mouse);
    il.connect_function(input_event::MOUSE_EVENT, 
                        fwr);

    input_manager::add(&il);

    screen::set_video_mode (640, 480, 16);

    landmap lmap;
    
    lmap.resize (16, 12);
    gc.mchar.set_position (6, 8); 
    lmap.put (&gc.mchar); 
    gc.mchar.set_limits (15, 11); 
    
    gc.mchar.set_speed (1.0); 

    map_object_with_gfx mobj;
    map_coordinates mobjmc (5, 5, 0, 0); 

    lmap.put (&mobj, mobjmc); 

    mobjmc.set_position (7, 6);
    mobjmc.set_offset (20, 20);
    lmap.put (&mobj, mobjmc); 
    
    
    while (!gc.letsexit) 
    {
        u_int16 i, j;

        input_manager::update();
        
        lmap.remove (&gc.mchar); 
        for (int i = 0; i < gametime::frames_to_skip (); i++) gc.mchar.update ();  
        lmap.put (&gc.mchar); 
        
        
        // Rendering phase
        
        for (j = 0; j < lmap.height (); j++)
        {
            vector <const mapsquare_info *> drawqueue; 
            for (i = 0; i < lmap.length (); i++) 
            {
                mapsquare * sq = lmap.get (i, j); 
                for (vector <mapsquare_info>::iterator it = sq->begin ();
                     it != sq->end (); it++)
                {
                    if (it->x () == i && it->y () == j) 
                        drawqueue.push_back (&(*it)); 
                }
            }
            
            sort (drawqueue.begin (), drawqueue.end (), boah); 
            
            for (vector <const mapsquare_info *>::iterator it = drawqueue.begin ();
                 it != drawqueue.end (); it++)
            {
                switch ((*it)->obj->type ()) 
                {
                    case CHARACTER:
                        ((map_character_with_gfx *)
                         (*it)->obj)->draw ((*it)->x () * mapsquare_size + (*it)->ox (),
                                           (*it)->y () * mapsquare_size + (*it)->oy (),
                                           ((map_character_with_gfx *) (*it)->obj));
                        break; 
                        
                    case OBJECT:
                        ((map_object_with_gfx *)
                         (*it)->obj)->draw ((*it)->x () * mapsquare_size + (*it)->ox (),
                                           (*it)->y () * mapsquare_size + (*it)->oy (),
                                           ((map_object_with_gfx *) (*it)->obj));
                        
                        break;

                    default:
                        break; 
                }
            }

            drawqueue.clear (); 
        }


        
        for (i = 0; i < screen::length (); i += mapsquare_size) 
            screen::display.fillrect (i, 0, 1, screen::height (), 0xFFFF00); 
        for (i = 0; i < screen::height (); i += mapsquare_size) 
            screen::display.fillrect (0, i, screen::length (), 1, 0xFFFF00); 

        gametime::update (); 
        screen::show ();
        screen::clear (); 
    }
    
    return 0; 
}
