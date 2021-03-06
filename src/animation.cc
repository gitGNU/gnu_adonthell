/*
   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/



/**
 * @file   animation.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the animationframe and animation classes.
 * 
 * 
 */
 
 
#include "animation.h"


using namespace std; 



// animationframe class.


// Public methods.


animationframe::animationframe ()
{
    clear ();
}

animationframe::~animationframe ()
{
}

void animationframe::clear ()
{
    imagenbr = 0;
    is_masked_ = false;
    set_alpha (255);
    gapx = 0;
    gapy = 0;
    delay_ = 0;
    nextframe_ = 0;
}

s_int8 animationframe::get (igzstream& file)
{
    imagenbr << file;
    is_masked_ << file;
    alpha_ << file;
    set_alpha (alpha_);
    gapx << file;
    gapy << file;
    delay_ << file;
    nextframe_ << file; 
    
    return (0);
}

s_int8 animationframe::put (ogzstream& file) const
{
    image_nbr () >> file;
    is_masked () >> file;
    alpha () >> file;
    offx () >> file;
    offy () >> file;
    delay () >> file;
    nextframe () >> file; 
    
    return (0);
}

 


//  animation class.


// Public methods.


animation::animation () : drawable () 
{
    clear (); 
}

void animation::clear ()
{
    vector <image *>::iterator i;
    for (i = t_frame.begin (); i != t_frame.end (); i++)
        delete (*i);
    t_frame.clear ();
    frame.clear ();
    currentframe_ = 0;
    speedcounter = 0;
    play_flag = STOP;
    xoffset_ = 0;
    yoffset_ = 0;
    set_length (0);
    set_height (0); 
}

animation::~animation ()
{
    clear ();
}


bool animation::update ()
{
    if ((!play_flag) || (!nbr_of_frames ()))
        return true;
    if (frame[currentframe ()].delay () == 0)
        return true;
    if (nbr_of_frames () <= 1)
        return true;

    speedcounter++;
    if (speedcounter >= frame[currentframe ()].delay ())
        next_frame ();

    return true; 
}

void animation::next_frame ()
{
    currentframe_ = frame[currentframe ()].nextframe ();
    speedcounter = 0;
}
 
void animation::draw (s_int16 x, s_int16 y, const drawing_area * da_opt, 
                      surface *target) const
{
    t_frame[frame[currentframe ()].image_nbr ()]->
        set_mask (frame[currentframe ()].is_masked ());
    t_frame[frame[currentframe ()].image_nbr ()]->
        set_alpha (frame[currentframe ()].alpha ());
    
    t_frame[frame[currentframe ()].image_nbr ()]->draw (x + xoffset () +
                                                        frame[currentframe ()].offx (),
                                                        y + yoffset () + frame[currentframe ()].offy (), da_opt,
                                                        target);
}
 
s_int8 animation::get (igzstream& file)
{
    u_int16 i;
    u_int16 nbr_images;
    u_int16 nbr_frames;
    u_int16 t_xoffset, t_yoffset;
 
    clear ();

    t_xoffset << file;
    t_yoffset << file; 


    // TODO: Remove this! (length and height are calculated later) 
    u_int16 dummy;
    dummy << file;
    dummy << file;

    // Read images
    nbr_images << file; 
    for (i = 0; i < nbr_images; i++)
    {
        t_frame.push_back (new image);
        t_frame.back ()->get_raw (file);
    }
    
    // Read frames
    nbr_frames << file; 
    animationframe aftemp;
    
    for (i = 0; i < nbr_frames; i++)
    {
        frame.push_back (aftemp);
        frame.back ().get (file);
    }
    
    currentframe_ = 0;

    set_offset (t_xoffset, t_yoffset);

    calculate_dimensions ();
    
    return (0);
}

s_int8 animation::load (string fname)
{
    igzstream file (fname);
    u_int8 retvalue;

    if (!file.is_open ())
        return (-1);
    retvalue = get (file);
    file.close (); 
    return (retvalue);
}

s_int8 animation::put (ogzstream& file) const
{
    u_int16 i;
 
    xoffset () >>  file;
    yoffset () >>  file; 


    // TODO: Remove this! (length and height are calculated later) 
    u_int16 dummy = 0;
    dummy >> file;
    dummy >> file;

    // Write images
    nbr_of_images () >> file;

    for (i = 0; i < nbr_of_images (); i++)
    {
        t_frame[i]->put_raw (file); 
    }
    
    // Write frames
    nbr_of_frames () >> file; 
    
    for (i = 0; i < nbr_of_frames (); i++)
    {
        frame[i].put (file);
    }
    
    return (0);
}

s_int8 animation::save (string fname) const
{
    ogzstream file (fname);
    u_int8 retvalue;

    if (!file.is_open ())
        return (-1);
    retvalue = put (file);
    file.close (); 
    return (retvalue);
}

s_int8 animation::insert_image (const image * im, u_int16 pos)
{
    vector <image *>::iterator i;
    vector <animationframe>::iterator j;

    if (pos > nbr_of_images ())
        return -2;

    i = t_frame.begin (); 
    while (pos--)
        i++;
    
    t_frame.insert (i, (image *) im);
    
    for (j = frame.begin (); j != frame.end (); j++)
        if (j->image_nbr () >= pos)
            j->set_image_nbr (j->image_nbr () + 1);
    
    return 0;
}

s_int8 animation::delete_image (u_int16 pos)
{
    vector <image *>::iterator i;
    vector <animationframe>::iterator j;

    if (pos > nbr_of_images () - 1)
        return -2;
    
    i = t_frame.begin ();
    while (pos--)
        i++;
    
    delete (*i); 
    t_frame.erase (i);
 
    for (j = frame.begin (); j != frame.end (); j++)
        if (j->image_nbr () >= pos)
            j->set_image_nbr (j->image_nbr () - 1);
    
    return 0;
}

s_int8 animation::insert_frame (const animationframe af, u_int16 pos)
{
    vector <animationframe>::iterator i;

    if (pos > nbr_of_frames ())
        return -2;

    i = frame.begin ();
    while (pos--)
        i++;
    
    frame.insert (i, af);
    
    for (i = frame.begin (); i != frame.end (); i++)
        if (i->nextframe () >= pos)
            i->set_nextframe (i->nextframe () + 1);
    
    return 0;
}

s_int8 animation::delete_frame (u_int16 pos)
{
    vector <animationframe>::iterator i;
    
    if (pos > nbr_of_frames () - 1)
        return -2;
    
    for (i = frame.begin (); i != frame.end (); i++)
        if (i->nextframe () >= pos)
            i->set_nextframe (frame[i->nextframe ()].nextframe ());
    
    i = frame.begin ();
    while (pos--)
        i++;
    
    frame.erase (i);
    
    if (!nbr_of_frames ())
        currentframe_ = 0;
    
    return 0;
}
 
void animation::zoom (u_int16 sx, u_int16 sy, const animation * src)
{
    static u_int16 i;

    clear ();
    for (i = 0; i < src->nbr_of_images (); i++)
    {
        image *im = new image;

        im->resize ((src->t_frame[i]->length () * sx) / src->length (),
                    (src->t_frame[i]->height () * sy) / src->height ());
        im->zoom ((*src->t_frame[i]));
        t_frame.push_back (im);
    }

    for (i = 0; i < src->nbr_of_frames (); i++)
    {
        frame.push_back (src->frame[i]);
        frame.back ().set_offset ((src->frame[i].offx () * sx) / src->length (),
                                  (src->frame[i].offy () * sy) / src->height ());
    }
}

animation& animation::operator = (const animation & src)
{ 
    clear (); 
    (drawable&) (*this) = (drawable&) src; 

    // Copy images
    vector <image *>::iterator imit; 
    for (imit = src.t_frame.begin (); imit != src.t_frame.end (); imit++)
    {
        image * im = new image;
        *im = *(*imit);
        t_frame.push_back (im); 
    }

    // Copy frames
    vector <animationframe>::iterator frit; 
    for (frit = src.frame.begin (); frit != src.frame.end (); frit++)
    {
        frame.push_back (*frit); 
    }
    
    // Copy properties
    currentframe_ = src.currentframe_;
    speedcounter = src.speedcounter;
    play_flag = src.play_flag; 

    set_length (src.length ());
    set_height (src.height ());
    set_offset (src.xoffset (), src.yoffset ());
    return *this;
}



// Private methods


void animation::calculate_dimensions ()
{
    for (u_int16 i = 0; i < nbr_of_frames (); i++)
    {
        u_int16 tl, th;
        
        if ((tl =
             t_frame[frame[i].image_nbr ()]->length () + frame[i].offx ()) >
            length ())
            set_length (tl + xoffset ());
        
        if ((th =
             t_frame[frame[i].image_nbr ()]->height () + frame[i].offy ()) >
            height ())
            set_height (th + yoffset ()); 
    }
}
