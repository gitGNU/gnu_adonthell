/*
   $Id:

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


#include "mapsquare_obj_area.h"

void mapsquare_obj_area::resize (u_int16 nx, u_int16 ny)
{
    area.resize (nx);
    for (vector <vector <mapsquare_walkable_info> >::iterator i = area.begin ();
         i != area.end (); i++)
        i->resize (ny);
}
