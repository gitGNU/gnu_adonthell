/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


#ifndef LANDMAP_H
#define LANDMAP_H

#include "map_character_with_gfx.h"
#include "map_object_with_gfx.h"
#include <vector>

class mapsquare_info : public map_coordinates
{
public:
    s_int32 zground;

    map_placeable * obj;
    
    mapsquare_info (map_coordinates & pos);

    bool operator < (const mapsquare_info & mi) const;

    bool operator == (const mapsquare_info & mi) const
    {
        // We call map_coordinates::operator == here
        return ((map_coordinates)*this == (map_coordinates)mi && 
                obj == mi.obj);
    }
}; 

class mapsquare
{
private:
    vector <mapsquare_info> objects; 

public:
    typedef vector <mapsquare_info>::iterator iterator;

    iterator begin ()
    {
        return objects.begin (); 
    }

    iterator end ()
    {
        return objects.end (); 
    }

    bool add (map_placeable * obj, map_coordinates & pos); 
    bool add (map_moving * obj);
    bool remove (map_placeable * obj, map_coordinates & pos); 
    bool exist (map_placeable * obj, map_coordinates & pos); 

    mapsquare_info * project_moving(map_moving & obj)
    {
        //        map_placeable_area * movstat = obj.current_state();

        mapsquare::iterator it = begin();
        while (it != end())
        {
            map_placeable_area * objstat = it->obj->current_state();
            if (obj.z() + obj.climb_capability() > it->z() + objstat->zsize) break;
            ++it;
        }
        
        if (it == end()) return NULL;
        return NULL;
    }
}; 

class landmap
{
private:
    template<class T, class T_gfx>
    class map_thing_manager
    {
    protected:
        vector<T *> objects;

        bool handle_gfx;
        
    public:
        map_thing_manager()
        {
            handle_gfx = true;
        }

        ~map_thing_manager()
        {
            clear();
        }

        void clear()
        {
            if(handle_gfx)
                for (vector<T *>::iterator i = objects.begin();
                     i != objects.end(); i++)
                    // FIXME: check whether it works or not.
                    delete ((T_gfx*)*i);
            else
                for (vector<T *>::iterator i = objects.begin();
                     i != objects.end(); i++)
                    delete *i;

            objects.clear();
        }

        bool update()
        {
            if (handle_gfx) return update_gfx();
            else return update_nogfx();
        }

        bool update_nogfx()
        {
            for (vector<T *>::iterator i = objects.begin();
                 i != objects.end(); i++)
                (*i)->update();

            return true;
        }

        bool update_gfx()
        {
            for (vector<T *>::iterator i = objects.begin();
                 i != objects.end(); i++)
            {
                (*i)->update();
                ((T_gfx *)(*i))->map_placeable_gfx::update();
            }
            return true;
        }

        T * add(landmap & lm)
        {
            T * obj;
            if (handle_gfx) obj = new T_gfx(lm);
            else obj = new T(lm);

            objects.push_back(obj);
            
            return (obj);
        }


        T * operator [] (const int index)
        {
            return objects[index];
        }
    };

    /**
     * Objects that take place on this map.
     * 
     */
    map_thing_manager<map_object, map_object_with_gfx> objects;

    /**
     * Pointers to the characters that are on this
     * map.
     * 
     */
    map_thing_manager<map_character, map_character_with_gfx> characters;

    vector <vector <mapsquare> > area;

    bool put (map_placeable * obj, map_coordinates & pos); 
    bool put (map_moving * obj); 
    bool remove (map_placeable * obj, map_coordinates & pos); 
    bool remove (map_moving * obj); 

public:
    landmap()
    {
    }

    ~landmap();

    void clear();

    u_int16 length () const
    {
        return area.size ();
    }
    
    u_int16 height () const
    {
        if (area.size ()) return area[0].size ();
        else return 0; 
    }

    void resize (const u_int16 nx, const u_int16 ny);

    mapsquare * get (const u_int16 x, const u_int16 y); 


    void update();

    /** 
     * Adds a map_object to this map.
     * @warning Ownership of the map_object will be given
     * to the map, which will delete it. DO NOT use the map_object
     * separately after having called this method!
     * 
     * @todo this function should load objects from files, being given
     * a map_object file name. That way, there won't be any ownership problem
     * anymore.
     *
     * @param mobj map_object to add to this map.
     * 
     * @return index of the object in case of success, \e -1 otherwise.
     */
    map_object * add_map_object();

    /** 
     * Tell a map to handle a map_character. The map will take
     * ownership of the character.
     * 
     * @warning Ownership of the map_character will be given
     * to the map, which will delete it. DO NOT use the map_character
     * separately after having called this method!
     * 
     * @todo this function should load objects from files, being given
     * a map_character file name. That way, there won't be any ownership problem
     * anymore.
     * @param mchar map_character to handle.
     * 
     * @return \e true in case of success, \e false otherwise.
     */
    map_character * add_map_character();

    bool put_map_object(u_int32 index, map_coordinates & pos);

    void output_occupation();

    friend class map_moving;
}; 


#endif
