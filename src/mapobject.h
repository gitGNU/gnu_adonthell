/*
   $Id$

   Copyright (C) 1999/2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


#ifndef _MAPOBJ_H
#define _MAPOBJ_H

#define MAPOBJECTS_DIR "gfx/mapobjects/"

#include <string>
#include <vector>
#include "fileops.h"
#include "animation.h"
#include "maptpl.h"

#ifdef USE_PYTHON
#include "py_inc.h"
#include "data.h"
struct PyCodeObject;
#endif


class mapobject:public maptpl
{
public:
    mapobject ();
    void clear ();
    ~mapobject ();

    u_int16 length ()
    {
        return length_;
    }

    u_int16 height ()
    {
        return height_;
    }

    void play ();
    void stop ();
    void rewind ();

    void update ();

    // FIXME: draw -> draw_from_base, draw_free -> draw
    void draw (s_int16 x, s_int16 y, drawing_area * da_opt = NULL);
    void draw_free (s_int16 x, s_int16 y, drawing_area * da_opt = NULL); 
    void draw_border_free (u_int16 x, u_int16 y, drawing_area * da_opt = NULL);
    void draw_border (u_int16 x, u_int16 y, drawing_area * da_opt = NULL);

    s_int8 get (igzstream & file);
    s_int8 load (string fname);

#ifdef USE_PYTHON
    void set_schedule (string file);
    bool is_schedule_activated ()
    {
        return schedule_activated;
    }

    void set_schedule_active (bool a)
    {
        schedule_activated = a;
    }
#endif
    
    u_int16 nbr_of_animations ()
    {
        return anim.size ();
    }
    
    animation *get_animation (u_int16 nbr)
    {
        return anim[nbr];
    }

    s_int8 insert_animation (animation * an, u_int16 pos);
    s_int8 delete_animation (u_int16 pos);
 
    mapobject & operator = (const mapobject & mo);

#ifdef _EDIT_
    s_int8 put (ogzstream& file);
    s_int8 save (string fname);

    void editor ();
#endif

protected:
#ifdef USE_PYTHON
    PyCodeObject * schedule;
#endif
    bool schedule_activated;
    u_int16 length_, height_;
    vector <animation *>anim;

    void init ();

private:
#ifdef _DEBUG_
    static u_int16 a_d_diff;
#endif
    string schedule_file; 
#ifdef _EDIT_
    string file_name; 
    win_label *info_win_label;
    win_container *info_win;
    u_int8 info_win_count;

    win_font *font;
    win_theme *th;
    image *bg;
    win_container *container;
    win_label *label_status;
    win_label *label_part;

    string label_txt;
    u_int16 currentpart;
    bool must_upt_label_part;
    bool show_grid;
    bool in_editor;
#endif

    void calculate_dimensions ();

#ifdef _EDIT_
public:
    void zoom (u_int16 sx, u_int16 sy, mapobject * src);
    void zoom_to_fit (u_int16 sm, mapobject * src);
    void init_parts ();
    u_int16 increase_part (u_int16 c);
    u_int16 decrease_part (u_int16 c);
    void set_part_xoffset (u_int16 p, s_int16 xoff);
    void set_part_yoffset (u_int16 p, s_int16 xoff);
    void update_label_part ();
    void set_currentpart (u_int16 p);
    void quick_save ();
    void quick_load ();
    void save ();
    void load ();
    void new_animation ();
    void add_animation ();
    void set_part_offset (u_int16 partnbr, u_int16 x, u_int16 y);
    void resize_grid ();
    void update_editor_keys ();
    void update_editor ();
    void draw_editor ();
    void update_and_draw ();
    void set_info_win (string text);
#endif
};

#endif
