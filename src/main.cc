#include "landmap.h"
#include "animation.h"
#include "gametime.h"
#include "input_manager.h"

class game_client
{
public:
    landmap lmap;
    map_character_with_gfx  * mchar;
    map_character_with_gfx  * mchar2;
    bool letsexit;
    bool draw_grid;

    game_client()
    {
        letsexit = false;
        draw_grid = false;
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
            if (cev->button() == control_event::B_BUTTON)
            {
                mchar->jump();
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
            if (kev->key() == keyboard_event::G_KEY)
            {
                draw_grid = !draw_grid;
            }            
            if (kev->key() == keyboard_event::O_KEY)
            {
                lmap.output_occupation();
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

    void create_map()
    {
        lmap.resize (16, 12);
        
        // Adding the map characters
        mchar = (map_character_with_gfx *)lmap.add_map_character();
        mchar->load("adontest/chrono.mdl");
        mchar->set_speed(1.0);
        mchar->set_position(6, 8);
        mchar->set_limits(16,12);

        // Adding map objects
        map_object_with_gfx * mobj;
        
        mobj = (map_object_with_gfx *)lmap.add_map_object();
        mobj->load("adontest/tree.mobj");
        
        mobj = (map_object_with_gfx *)lmap.add_map_object();
        mobj->load("adontest/sandy.mobj");
        
        mobj = (map_object_with_gfx *)lmap.add_map_object();
        mobj->load("adontest/rug.mobj");
        
        mobj = (map_object_with_gfx *)lmap.add_map_object();
        mobj->load("adontest/platform.mobj");
        
        mobj = (map_object_with_gfx *)lmap.add_map_object();
        mobj->load("adontest/pillar_l.mobj");
        
        mobj = (map_object_with_gfx *)lmap.add_map_object();
        mobj->load("adontest/pillar_r.mobj");
        
        
        map_coordinates mc;
        
        for (int i = 0; i < lmap.length(); i++)
            for (int j = 0; j < lmap.height(); j++)
            {
                map_coordinates mc(i, j, 0, 0, 0);
                lmap.put_map_object(1, mc); 
            }
        
        mc.set_position(10, 5);
        lmap.put_map_object(4, mc); 
        mc.set_position(11, 5);
        lmap.put_map_object(5, mc); 
        mc.set_position(10, 3);
        lmap.put_map_object(4, mc); 
        mc.set_position(11, 3);
        lmap.put_map_object(5, mc); 

        for (int i = 10; i < 12; i++)
            for (int j = 4; j < 6; j++)
            {
                map_coordinates mc(i, j, 40);
                lmap.put_map_object(3, mc); 
            }
        
        mc.set_position(7, 6);
        lmap.put_map_object(4, mc); 
        mc.set_altitude(40);
        lmap.put_map_object(4, mc); 
        mc.set_position(7, 3);
        mc.set_altitude(0);
        lmap.put_map_object(4, mc); 
        mc.set_altitude(40);
        lmap.put_map_object(4, mc); 
        
        mc.set_altitude(0);
        mc.set_position(8, 6);
        lmap.put_map_object(5, mc); 
        mc.set_altitude(40);
        lmap.put_map_object(5, mc); 
        mc.set_position(8, 3);
        mc.set_altitude(0);
        lmap.put_map_object(5, mc); 
        mc.set_altitude(40);
        lmap.put_map_object(5, mc); 
        
        for (int i = 7; i < 9; i++)
            for (int j = 4; j < 7; j++)
            {
                map_coordinates mc(i, j, 80);
                lmap.put_map_object(3, mc); 
            }
        
        for (int i = 0; i < 13; i++)
        {
            map_coordinates mc(i + 1, 9, 5 * (i + 1));
            lmap.put_map_object(3, mc); 
        }        
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

    control_event::map_keyboard_key(keyboard_event::E_KEY, control_event::B_BUTTON);
    control_event::map_joystick_button(0, joystick_event::BUTTON_1, control_event::B_BUTTON);

    control_event::map_keyboard_key(keyboard_event::UP_KEY, control_event::UP_BUTTON);
    control_event::map_keyboard_key(keyboard_event::DOWN_KEY, control_event::DOWN_BUTTON);
    control_event::map_keyboard_key(keyboard_event::LEFT_KEY, control_event::LEFT_BUTTON);
    control_event::map_keyboard_key(keyboard_event::RIGHT_KEY, control_event::RIGHT_BUTTON);

    control_event::map_joystick_button(0, joystick_event::AXIS1_FORE, control_event::UP_BUTTON);
    control_event::map_joystick_button(0, joystick_event::AXIS1_BACK, control_event::DOWN_BUTTON);
    control_event::map_joystick_button(0, joystick_event::AXIS0_FORE, control_event::LEFT_BUTTON);
    control_event::map_joystick_button(0, joystick_event::AXIS0_BACK, control_event::RIGHT_BUTTON);

    game_client gc;

    Functor1wRet<input_event *, int> fwr;
    fwr = makeFunctor(&fwr, gc, &game_client::control_func);
    il.connect_function(input_event::CONTROL_EVENT, 
                        fwr);

    fwr = makeFunctor(&fwr, gc, &game_client::callback_func);
    il.connect_function(input_event::KEYBOARD_EVENT, 
                        fwr);

    gc.create_map();

    while (!gc.letsexit) 
    {
        u_int16 i, j;
        
        for (int i = 0; i < gametime::frames_to_skip (); i++) 
        {
            input_manager::update();
            gc.lmap.update();
        }

        list <mapsquare_info> drawqueue; 

        // Rendering phase        
        for (j = 0; j < gc.lmap.height (); j++)
        {

            for (i = 0; i < gc.lmap.length (); i++) 
            {
                mapsquare * sq = gc.lmap.get (i, j); 
                for (mapsquare::iterator it = sq->begin ();
                     it != sq->end (); it++)
                {
                    if (it->x () == i && it->y () == j) 
                        drawqueue.push_back (*it); 
                }
            }            
        }

        drawqueue.sort();
        //            sort (drawqueue.begin (), drawqueue.end (), boah); 
            
            for (list <mapsquare_info>::iterator it = drawqueue.begin ();
                 it != drawqueue.end (); it++)
            {
                switch ((*it).obj->type ()) 
                {
                    case CHARACTER:
                        ((map_character_with_gfx *)
                         (*it).obj)->draw ((*it).x () * mapsquare_size + (*it).ox (),
                                           (*it).y () * mapsquare_size + (*it).oy ());
//                         ((map_character_with_gfx *)
//                          (*it).obj)->draw_walkable ((*it).x () * mapsquare_size + (*it).ox (),
//                                            (*it).y () * mapsquare_size + (*it).oy ());
//                         ((map_character_with_gfx *)
//                          (*it).obj)->draw_border ((*it).x () * mapsquare_size + (*it).ox (),
//                                            (*it).y () * mapsquare_size + (*it).oy ());
                        break; 
                        
                    case OBJECT:
                        ((map_object_with_gfx *)
                         (*it).obj)->draw ((*it).x () * mapsquare_size + (*it).ox (),
                                            (*it).y () * mapsquare_size + (*it).oy () - (*it).z());
//                         ((map_object_with_gfx *)
//                          (*it).obj)->draw_walkable ((*it).x () * mapsquare_size + (*it).ox (),
//                                             (*it).y () * mapsquare_size + (*it).oy ());                        
//                         ((map_object_with_gfx *)
//                          (*it).obj)->draw_border ((*it).x () * mapsquare_size + (*it).ox (),
//                                             (*it).y () * mapsquare_size + (*it).oy ());                        
                        break;
                        
                    default:
                        break; 
                }
            }

            drawqueue.clear ();             


        if (gc.draw_grid)
        {
            for (i = 0; i < screen::length (); i += mapsquare_size) 
                screen::display.fillrect (i, 0, 1, screen::height (), 0xFFFF00); 
            for (i = 0; i < screen::height (); i += mapsquare_size) 
                screen::display.fillrect (0, i, screen::length (), 1, 0xFFFF00); 
        }

        gametime::update (); 
        screen::show ();
        screen::clear (); 

    }

    input_manager::cleanup();
    
    return 0; 
}
