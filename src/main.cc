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
    map_character_with_gfx  * mchar;
    map_character_with_gfx  * mchar2;
    bool letsexit;

    game_client(landmap & mymap)
    {
        letsexit = false;
    }

    int control_func(input_event * ev)
    {
        control_event * cev = (control_event *)ev;

        if (cev->type() == control_event::BUTTON_PUSHED)
        {
            if (cev->button() == control_event::A_BUTTON)
            {
                mchar->run();
            }            
            if (cev->button() == control_event::LEFT_BUTTON)
            {
                mchar->add_direction(mchar->WEST);
            }
            if (cev->button() == control_event::RIGHT_BUTTON)
            {
                mchar->add_direction(mchar->EAST);
            }
            if (cev->button() == control_event::UP_BUTTON)
            {
                mchar->add_direction(mchar->NORTH);
            }
            if (cev->button() == control_event::DOWN_BUTTON)
            {
                mchar->add_direction(mchar->SOUTH);
            }            
        }
        else
        {
            if (cev->button() == control_event::A_BUTTON)
            {
                mchar->walk();
            }            
            if (cev->button() == control_event::LEFT_BUTTON)
            {
                mchar->remove_direction(mchar->WEST);
            }
            if (cev->button() == control_event::RIGHT_BUTTON)
            {
                mchar->remove_direction(mchar->EAST);
            }
            if (cev->button() == control_event::UP_BUTTON)
            {
                mchar->remove_direction(mchar->NORTH);
            }
            if (cev->button() == control_event::DOWN_BUTTON)
            {
                mchar->remove_direction(mchar->SOUTH);
            }            

        }

        return 1;
    }
    
    int callback_func(input_event * ev)
    {
        keyboard_event * kev = (keyboard_event *)ev;

        if (kev->type() == keyboard_event::KEY_PUSHED)
        {
            if (kev->key() == keyboard_event::ESCAPE_KEY)
            {
                letsexit = true;
            }            

            if (kev->key() == keyboard_event::J_KEY)
            {
                mchar2->add_direction(mchar2->SOUTH);
            }            
            if (kev->key() == keyboard_event::U_KEY)
            {
                mchar2->add_direction(mchar2->NORTH);
            }            
            if (kev->key() == keyboard_event::H_KEY)
            {
                mchar2->add_direction(mchar2->WEST);
            }            
            if (kev->key() == keyboard_event::K_KEY)
            {
                mchar2->add_direction(mchar2->EAST);
            }            
        }
        else
        {
            if (kev->key() == keyboard_event::R_KEY)
            {
                mchar->walk();
            }            

            if (kev->key() == keyboard_event::J_KEY)
            {
                mchar2->remove_direction(mchar2->SOUTH);
            }            
            if (kev->key() == keyboard_event::U_KEY)
            {
                mchar2->remove_direction(mchar2->NORTH);
            }            
            if (kev->key() == keyboard_event::H_KEY)
            {
                mchar2->remove_direction(mchar2->WEST);
            }            
            if (kev->key() == keyboard_event::K_KEY)
            {
                mchar2->remove_direction(mchar2->EAST);
            }            
        }   

        return true;
    }
};

int main (int argc, char * argv[]) 
{
    screen::init (); 
    screen::set_video_mode(640, 480);

    input_manager::init();

    input_listener il;

    input_manager::add(&il);

    
    control_event::map_keyboard_key(keyboard_event::LCTRL_KEY, control_event::A_BUTTON);
    control_event::map_keyboard_key(keyboard_event::R_KEY, control_event::A_BUTTON);
    control_event::map_joystick_button(0, joystick_event::BUTTON_0, control_event::A_BUTTON);
    control_event::map_mouse_button(mouse_event::RIGHT_BUTTON, control_event::A_BUTTON);

    control_event::map_keyboard_key(keyboard_event::UP_KEY, control_event::UP_BUTTON);
    control_event::map_keyboard_key(keyboard_event::DOWN_KEY, control_event::DOWN_BUTTON);
    control_event::map_keyboard_key(keyboard_event::LEFT_KEY, control_event::LEFT_BUTTON);
    control_event::map_keyboard_key(keyboard_event::RIGHT_KEY, control_event::RIGHT_BUTTON);

    control_event::map_joystick_button(0, joystick_event::AXIS1_FORE, control_event::UP_BUTTON);
    control_event::map_joystick_button(0, joystick_event::AXIS1_BACK, control_event::DOWN_BUTTON);
    control_event::map_joystick_button(0, joystick_event::AXIS0_FORE, control_event::LEFT_BUTTON);
    control_event::map_joystick_button(0, joystick_event::AXIS0_BACK, control_event::RIGHT_BUTTON);

    landmap lmap;

    game_client gc(lmap);

    Functor1wRet<input_event *, int> fwr;
    fwr = makeFunctor(&fwr, gc, &game_client::control_func);
    il.connect_function(input_event::CONTROL_EVENT, 
                        fwr);

    fwr = makeFunctor(&fwr, gc, &game_client::callback_func);
    il.connect_function(input_event::KEYBOARD_EVENT, 
                        fwr);
    
    lmap.resize (16, 12);

    // Adding the map characters
    gc.mchar = (map_character_with_gfx *)lmap.add_map_character();
    gc.mchar->load("adontest/chrono.mdl");
    gc.mchar->set_position (6, 8); 
    gc.mchar->set_limits (16, 12);
    gc.mchar->set_speed (1.0);

    gc.mchar2 = (map_character_with_gfx *)lmap.add_map_character();
    gc.mchar2->load("adontest/nakedguy.mdl");
    gc.mchar2->set_position (10, 7); 
    gc.mchar2->set_limits (16, 12);
    gc.mchar2->set_speed (1.1);

    // Adding map objects
    map_object_with_gfx * mobj;

    mobj = (map_object_with_gfx *)lmap.add_map_object();
    mobj->load("adontest/tree.mobj");

    mobj = (map_object_with_gfx *)lmap.add_map_object();
    mobj->load("adontest/sandy.mobj");

    mobj = (map_object_with_gfx *)lmap.add_map_object();
    mobj->load("adontest/rug.mobj");

    for (int i = 0; i < lmap.length(); i++)
        for (int j = 0; j < lmap.height(); j++)
        {
            map_coordinates mc(i, j, 0, 0);
            lmap.put_map_object(1, mc, true); 
        }


    map_coordinates rmc(7, 4);
    lmap.put_map_object(2, rmc);

    map_coordinates mobjmc (6, 6, 0, 0); 
    lmap.put_map_object (0, mobjmc); 

    mobjmc.set_position (8, 7);
    mobjmc.set_offset (20, 20);
    lmap.put_map_object (0, mobjmc); 
        
    while (!gc.letsexit) 
    {
        u_int16 i, j;
        
        for (int i = 0; i < gametime::frames_to_skip (); i++) 
        {
            input_manager::update();
            lmap.update();
        }

        // Rendering phase
        
        for (j = 0; j < lmap.height (); j++)
        {
            // Current level of flat objects
            u_int32 l = 0;
            vector <const mapsquare_info *> drawqueue; 
            do
            {
                bool one_extra_pass = false;
                
                for (i = 0; i < lmap.length (); i++) 
                {
                    mapsquare * sq = lmap.get (i, j); 
                    if (sq->nbr_flat() > l + 1) one_extra_pass = true;
                    if (sq->nbr_flat() > l)
                    {
                        mapsquare_info * mqi = sq->get_flat(l);
                        if (mqi->x() == i && mqi->y() == j)
                            drawqueue.push_back(mqi);
                    }
                }

                if(!one_extra_pass) break;
                l++;
            } while(1);
        
            sort (drawqueue.begin (), drawqueue.end (), boah); 
            
            for (vector <const mapsquare_info *>::iterator it = drawqueue.begin ();
                 it != drawqueue.end (); it++)
            {
                switch ((*it)->obj->type ()) 
                {
                    case CHARACTER:
                        ((map_character_with_gfx *)
                         (*it)->obj)->draw ((*it)->x () * mapsquare_size + (*it)->ox (),
                                           (*it)->y () * mapsquare_size + (*it)->oy ());
                        break; 
                        
                    case OBJECT:
                        ((map_object_with_gfx *)
                         (*it)->obj)->draw ((*it)->x () * mapsquare_size + (*it)->ox (),
                                            (*it)->y () * mapsquare_size + (*it)->oy ());
                        
                        break;
                        
                    default:
                        break; 
                }
            }
            
            drawqueue.clear ();             

            for (i = 0; i < lmap.length (); i++) 
            {
                mapsquare * sq = lmap.get (i, j); 
                for (mapsquare::iterator it = sq->begin ();
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
                                           (*it)->y () * mapsquare_size + (*it)->oy ());
                        break; 
                        
                    case OBJECT:
                        ((map_object_with_gfx *)
                         (*it)->obj)->draw ((*it)->x () * mapsquare_size + (*it)->ox (),
                                            (*it)->y () * mapsquare_size + (*it)->oy ());
                        
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

    input_manager::cleanup();
    
    return 0; 
}
