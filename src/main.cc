#if 0

#include "lmap/landmap.h"
#include "gfx/animation.h"
#include "gametime.h"
#include "input/manager.h"

class game_client
{
public:
    lmap::landmap lmap;
    lmap::character_with_gfx  * mchar;
    lmap::character_with_gfx  * mchar2;
    bool letsexit;
    bool draw_grid;

    game_client()
    {
        letsexit = false;
        draw_grid = false;
    }

    int control_func(input::event * ev)
    {
        input::control_event * cev = (input::control_event *)ev;

        if (cev->type() == input::control_event::BUTTON_PUSHED)
        {
            if (cev->button() == input::control_event::A_BUTTON)
            {
                mchar->run();
            }            
            if (cev->button() == input::control_event::B_BUTTON)
            {
                mchar->jump();
            }            
            if (cev->button() == input::control_event::LEFT_BUTTON)
            {
                mchar->add_direction(mchar->WEST);
            }
            if (cev->button() == input::control_event::RIGHT_BUTTON)
            {
                mchar->add_direction(mchar->EAST);
            }
            if (cev->button() == input::control_event::UP_BUTTON)
            {
                mchar->add_direction(mchar->NORTH);
            }
            if (cev->button() == input::control_event::DOWN_BUTTON)
            {
                mchar->add_direction(mchar->SOUTH);
            }            
        }
        else
        {
            if (cev->button() == input::control_event::A_BUTTON)
            {
                mchar->walk();
            }            
            if (cev->button() == input::control_event::LEFT_BUTTON)
            {
                mchar->remove_direction(mchar->WEST);
            }
            if (cev->button() == input::control_event::RIGHT_BUTTON)
            {
                mchar->remove_direction(mchar->EAST);
            }
            if (cev->button() == input::control_event::UP_BUTTON)
            {
                mchar->remove_direction(mchar->NORTH);
            }
            if (cev->button() == input::control_event::DOWN_BUTTON)
            {
                mchar->remove_direction(mchar->SOUTH);
            }            

        }

        return 1;
    }
    
    int callback_func(input::event * ev)
    {
        input::keyboard_event * kev = (input::keyboard_event *)ev;

        if (kev->type() == input::keyboard_event::KEY_PUSHED)
        {
            if (kev->key() == input::keyboard_event::ESCAPE_KEY)
            {
                letsexit = true;
            }            
            if (kev->key() == input::keyboard_event::G_KEY)
            {
                draw_grid = !draw_grid;
            }            
            if (kev->key() == input::keyboard_event::O_KEY)
            {
                lmap.output_occupation();
            }            

            if (kev->key() == input::keyboard_event::J_KEY)
            {
                mchar2->add_direction(mchar2->SOUTH);
            }            
            if (kev->key() == input::keyboard_event::U_KEY)
            {
                mchar2->add_direction(mchar2->NORTH);
            }            
            if (kev->key() == input::keyboard_event::H_KEY)
            {
                mchar2->add_direction(mchar2->WEST);
            }            
            if (kev->key() == input::keyboard_event::K_KEY)
            {
                mchar2->add_direction(mchar2->EAST);
            }            
        }
        else
        {
            if (kev->key() == input::keyboard_event::R_KEY)
            {
                mchar->walk();
            }            

            if (kev->key() == input::keyboard_event::J_KEY)
            {
                mchar2->remove_direction(mchar2->SOUTH);
            }            
            if (kev->key() == input::keyboard_event::U_KEY)
            {
                mchar2->remove_direction(mchar2->NORTH);
            }            
            if (kev->key() == input::keyboard_event::H_KEY)
            {
                mchar2->remove_direction(mchar2->WEST);
            }            
            if (kev->key() == input::keyboard_event::K_KEY)
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
        mchar = (lmap::character_with_gfx *)lmap.add_character();
        mchar->load("adontest/chrono.mdl");
        mchar->set_speed(1.0);
        mchar->set_position(6, 8);
        mchar->set_limits(16,12);

        // Quick fix for the character size.
        for (lmap::placeable::iterator it = mchar->begin(); it != mchar->end(); ++it)
            it->second.zsize += 5;

        // Adding map objects
        lmap::object_with_gfx * mobj;
        
        mobj = (lmap::object_with_gfx *)lmap.add_object();
        mobj->load("adontest/tree.mobj");
        
        mobj = (lmap::object_with_gfx *)lmap.add_object();
        mobj->load("adontest/sandy.mobj");

        mobj = (lmap::object_with_gfx *)lmap.add_object();
        mobj->load("adontest/rug.mobj");
        
        mobj = (lmap::object_with_gfx *)lmap.add_object();
        mobj->load("adontest/platform.mobj");
        
        mobj = (lmap::object_with_gfx *)lmap.add_object();
        mobj->load("adontest/pillar_l.mobj");
        
        mobj = (lmap::object_with_gfx *)lmap.add_object();
        mobj->load("adontest/pillar_r.mobj");
        
        
        lmap::coordinates mc;
        
        for (int i = 0; i < lmap.length(); i++)
            for (int j = 0; j < lmap.height(); j++)
            {
                lmap::coordinates mc(i, j, 0, 0, 0);
                lmap.put_object(1, mc); 
            }
        
        mc.set_position(10, 5);
        lmap.put_object(4, mc); 
        mc.set_position(11, 5);
        lmap.put_object(5, mc); 
        mc.set_position(10, 3);
        lmap.put_object(4, mc); 
        mc.set_position(11, 3);
        lmap.put_object(5, mc); 

        for (int i = 10; i < 12; i++)
            for (int j = 4; j < 6; j++)
            {
                lmap::coordinates mc(i, j, 40);
                lmap.put_object(3, mc); 
            }
        
        mc.set_position(7, 6);
        lmap.put_object(4, mc); 
        mc.set_altitude(40);
        lmap.put_object(4, mc); 
        mc.set_position(7, 3);
        mc.set_altitude(0);
        lmap.put_object(4, mc); 
        mc.set_altitude(40);
        lmap.put_object(4, mc); 
        
        mc.set_altitude(0);
        mc.set_position(8, 6);
        lmap.put_object(5, mc); 
        mc.set_altitude(40);
        lmap.put_object(5, mc); 
        mc.set_position(8, 3);
        mc.set_altitude(0);
        lmap.put_object(5, mc); 
        mc.set_altitude(40);
        lmap.put_object(5, mc); 
        
        for (int i = 7; i < 9; i++)
            for (int j = 4; j < 7; j++)
            {
                lmap::coordinates mc(i, j, 80);
                lmap.put_object(3, mc); 
            }
        
        for (int i = 0; i < 13; i++)
        {
            lmap::coordinates mc(i + 1, 9, 5 * (i + 1));
            lmap.put_object(3, mc); 
        }        
    }
};

int main(int argc, char * argv[]) 
{
    gfx::screen::init (); 
    gfx::screen::set_video_mode(640, 480);

    input::manager::init();

    input::listener il;

    input::manager::add(&il);

    
    input::control_event::map_keyboard_key(input::keyboard_event::LCTRL_KEY, input::control_event::A_BUTTON);
    input::control_event::map_keyboard_key(input::keyboard_event::R_KEY, input::control_event::A_BUTTON);
    input::control_event::map_joystick_button(0, input::joystick_event::BUTTON_0, input::control_event::A_BUTTON);
    input::control_event::map_mouse_button(input::mouse_event::RIGHT_BUTTON, input::control_event::A_BUTTON);

    input::control_event::map_keyboard_key(input::keyboard_event::E_KEY, input::control_event::B_BUTTON);
    input::control_event::map_joystick_button(0, input::joystick_event::BUTTON_1, input::control_event::B_BUTTON);

    input::control_event::map_keyboard_key(input::keyboard_event::UP_KEY, input::control_event::UP_BUTTON);
    input::control_event::map_keyboard_key(input::keyboard_event::DOWN_KEY, input::control_event::DOWN_BUTTON);
    input::control_event::map_keyboard_key(input::keyboard_event::LEFT_KEY, input::control_event::LEFT_BUTTON);
    input::control_event::map_keyboard_key(input::keyboard_event::RIGHT_KEY, input::control_event::RIGHT_BUTTON);

    input::control_event::map_joystick_button(0, input::joystick_event::AXIS1_FORE, input::control_event::UP_BUTTON);
    input::control_event::map_joystick_button(0, input::joystick_event::AXIS1_BACK, input::control_event::DOWN_BUTTON);
    input::control_event::map_joystick_button(0, input::joystick_event::AXIS0_FORE, input::control_event::LEFT_BUTTON);
    input::control_event::map_joystick_button(0, input::joystick_event::AXIS0_BACK, input::control_event::RIGHT_BUTTON);

    game_client gc;

    Functor1wRet<input::event *, int> fwr;
    fwr = makeFunctor(&fwr, gc, &game_client::control_func);
    il.connect_function(input::event::CONTROL_EVENT, 
                        fwr);

    fwr = makeFunctor(&fwr, gc, &game_client::callback_func);
    il.connect_function(input::event::KEYBOARD_EVENT, 
                        fwr);

    gc.create_map();

    while (!gc.letsexit) 
    {
        u_int16 i, j;
        
        for (int i = 0; i < gametime::frames_to_skip (); i++) 
        {
            input::manager::update();
            gc.lmap.update();
        }

        std::list <lmap::square_info> drawqueue; 

        // Rendering phase        
        for (j = 0; j < gc.lmap.height (); j++)
        {

            for (i = 0; i < gc.lmap.length (); i++) 
            {
                lmap::square * sq = gc.lmap.get (i, j); 
                lmap::square::iterator it = sq->end();
                while (it != sq->begin())
                {
                    --it;
                    if (!it->oy())
                    {
                        if (it->x () == i && it->y () == j) 
                            drawqueue.push_back(*it);
                    }
                    else
                    {
                        if (it->x () == i && it->y () == j - 1) 
                            drawqueue.push_back(*it);
                    }

                }
            }
//                 for (mapsquare::iterator it = sq->begin ();
//                      it != sq->end (); it++)
//                 {
//                     if (it->x () == i && it->y () == j) 
//                         drawqueue.push_back (*it); 
//                 }
//             }            
//         }

        drawqueue.sort();
            
        for (std::list <lmap::square_info>::iterator it = drawqueue.begin ();
             it != drawqueue.end (); it++)
        {
            switch ((*it).obj->type ()) 
            {
                case lmap::CHARACTER:
                    ((lmap::character_with_gfx *)
                     (*it).obj)->draw ((*it).x () * lmap::square_size + (*it).ox (),
                                       (*it).y () * lmap::square_size + (*it).oy ());
                    break; 
                    
                case lmap::OBJECT:
                    ((lmap::object_with_gfx *)
                     (*it).obj)->draw ((*it).x () * lmap::square_size + (*it).ox (),
                                       (*it).y () * lmap::square_size + (*it).oy () - (*it).z());
                    break;
                    
                default:
                    break; 
            }
        }
        
        drawqueue.clear ();             
        }
        
        
        if (gc.draw_grid)
        {
            for (i = 0; i < gfx::screen::length (); i += lmap::square_size) 
                gfx::screen::display.fillrect (i, 0, 1, gfx::screen::height (), 0xFFFF00); 
            for (i = 0; i < gfx::screen::height (); i += lmap::square_size) 
                gfx::screen::display.fillrect (0, i, gfx::screen::length (), 1, 0xFFFF00); 
        }

        gametime::update (); 
        gfx::screen::display.draw_line(10, 10, 40, 100, 0xFFFFFF);
        gfx::screen::show ();
        gfx::screen::clear (); 
    }
    
    input::manager::cleanup();
    
    return 0; 
}

#else
#include "gfx/image.h"
#include "input/manager.h"

#include <unistd.h>
#include <iostream>

class point
{
public:
    point(const s_int16 x, const s_int16 y)
    {
        X = x;
        Y = y;
    }

    s_int16 x () const { return X; }
    s_int16 y () const { return Y; }

    void draw () const
    {
        gfx::screen::display.lock ();
        gfx::screen::display.put_pix (x (), y (), 0x0000FF);
        gfx::screen::display.unlock ();
    }

    s_int16 X, Y;
};

class vector : public point
{
public:
    vector (const s_int16 x, const s_int16 y) : point (x, y)
    {
    }

    s_int16 crossproduct (vector v) const
    {
        return (x () * v.x () - y () * v.y ());
    }
};

class line
{
public:
    line (const point & p1, const point & p2) : P1 (p1), P2 (p2), Respoint (0, 0)
    {
    }

    line (const point p, const vector v) : P1 (p), P2 (p.x ()+ v.x (), p.y ()+ v.y ()), Respoint (0, 0)
    {
    }

    void draw () const
    {
        gfx::screen::display.lock ();
        gfx::screen::display.draw_line (P1.x (), P1.y (), P2.x (), P2.y (), 0xFFFFFF);
        gfx::screen::display.unlock ();
    }

    bool intersect (const line & l) const
    {
        s_int16 t = (P2.x () - P1.x ()) * (l.P2.y () - l.P1.y ()) - 
            (P2.y() - P1.y()) * (l.P2.x () - l.P1.x());
        
        if (!t) return false;

        float denom = 1.0 / t;

        s_int16 Tx = P1.x () - l.P1.x ();
        s_int16 Ty = P1.y () - l.P1.y ();

        float r = (Ty * (l.P2.x () - l.P1.x ()) - Tx * (l.P2.y () - l.P1.y ())) * denom;
        float s = (Ty * (P2.x () - P1.x ()) - Tx * (P2.y () - P1.y ())) * denom;

        std::cout << r << " " << s << std::endl;

        if (s >= 0 && s <= 1 && r >= 0 && r <= 1)
        {
            Respoint = point ((s_int16)P1.x () + r * (P2.x () - P1.x ()),
                              (s_int16)P1.y () + r * (P2.y () - P1.y ()));
            return true;
        }

        return false;
    }

    point respoint () const { return Respoint; }

    point P1, P2;
    mutable point Respoint;
};

int main(int argc, char * argv[]) 
{
    gfx::screen::init (); 
    gfx::screen::set_video_mode(640, 480);

    input::manager::init();

    input::listener il;

    line l1 (point (10, 10), point (50, 70));
    line l2 (point (60, 40), point (20, 80));

    l1.draw();
    l2.draw();
    gfx::screen::show();

    if (l1.intersect (l2))
        std::cout << "Intersection point: " << l1.respoint ().x () << ", " << l1.respoint ().y () << std::endl;

    sleep(1);

    input::manager::cleanup();
    
    return 0; 
}
#endif
