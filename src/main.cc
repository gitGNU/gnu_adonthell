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

int main (int argc, char * argv[]) 
{
    screen::init (); 
    screen::set_video_mode(640, 480);

    while (1)
    {
        input_manager::update();
    }
//     input::init (); 

//     screen::set_video_mode (640, 480, 16);

//     landmap lmap;
//     map_character_with_gfx mchar; 
    
//     lmap.resize (16, 12);
//     mchar.set_position (6, 8); 
//     lmap.put (&mchar); 
//     mchar.set_limits (15, 11); 
    
//     mchar.set_speed (1.0); 

//     map_object_with_gfx mobj;
//     map_coordinates mobjmc (5, 5, 0, 0); 

//     lmap.put (&mobj, mobjmc); 

//     mobjmc.set_position (7, 6);
//     mobjmc.set_offset (20, 20);
//     lmap.put (&mobj, mobjmc); 
    
    
//     while (!input::has_been_pushed (SDLK_ESCAPE)) 
//     {
//         u_int16 i, j;

//         static float v;

//         if (input::is_pushed (SDLK_r))
//             v = mchar.speed () * 2;
//         else v = mchar.speed (); 
        
//         mchar.set_velocity (0.0, 0.0); 
//         if (input::is_pushed (SDLK_LEFT))
//         {
//             mchar.set_velocity (-v, mchar.vy ());
//             mchar.update_state (); 
//         }
//         if (input::is_pushed (SDLK_RIGHT))
//         {
//             mchar.set_velocity (v, mchar.vy ());
//             mchar.update_state (); 
//         }
//         if (input::is_pushed (SDLK_UP))
//         {
//             mchar.set_velocity (mchar.vx (), -v); 
//             mchar.update_state (); 
//         }
//         if (input::is_pushed (SDLK_DOWN))
//         {
//             mchar.set_velocity (mchar.vx (), v); 
//             mchar.update_state (); 
//         }

//         lmap.remove (&mchar); 
//         for (int i = 0; i < gametime::frames_to_skip (); i++) mchar.update ();  
//         lmap.put (&mchar); 
        

//         // Rendering phase

//         for (j = 0; j < lmap.height (); j++)
//         {
//             vector <const mapsquare_info *> drawqueue; 
//             for (i = 0; i < lmap.length (); i++) 
//             {
//                 mapsquare * sq = lmap.get (i, j); 
//                 for (vector <mapsquare_info>::iterator it = sq->begin ();
//                      it != sq->end (); it++)
//                 {
//                     if (it->x () == i && it->y () == j) 
//                         drawqueue.push_back (&(*it)); 
//                 }
//             }

//             sort (drawqueue.begin (), drawqueue.end (), boah); 
            
//             for (vector <const mapsquare_info *>::iterator it = drawqueue.begin ();
//                  it != drawqueue.end (); it++)
//             {
//                 switch ((*it)->obj->type ()) 
//                 {
//                     case CHARACTER:
//                         ((map_character_with_gfx *)
//                          (*it)->obj)->draw ((*it)->x () * mapsquare_size + (*it)->ox (),
//                                            (*it)->y () * mapsquare_size + (*it)->oy (),
//                                            ((map_character_with_gfx *) (*it)->obj));
//                         break; 
                        
//                     case OBJECT:
//                         ((map_object_with_gfx *)
//                          (*it)->obj)->draw ((*it)->x () * mapsquare_size + (*it)->ox (),
//                                            (*it)->y () * mapsquare_size + (*it)->oy (),
//                                            ((map_object_with_gfx *) (*it)->obj));
                        
//                         break;

//                     default:
//                         break; 
//                 }
//             }

//             drawqueue.clear (); 
//         }


        
//         for (i = 0; i < screen::length (); i += mapsquare_size) 
//             screen::display.fillrect (i, 0, 1, screen::height (), 0xFFFF00); 
//         for (i = 0; i < screen::height (); i += mapsquare_size) 
//             screen::display.fillrect (0, i, screen::length (), 1, 0xFFFF00); 

//         input::update (); 
//         gametime::update (); 
//         screen::show ();
//         screen::clear (); 
//     }
    
//     input::shutdown (); 
    
    return 0; 
}
