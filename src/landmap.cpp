/*
   $Id$

   Copyright (C) 1999 - 2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "landmap.h"
#include "mapview.h"
#include "fileops.h"

#if defined _DEBUG_ || defined _EDIT_
u_int16 mapsquare::a_d_diff = 0;
u_int16 landsubmap::a_d_diff = 0;
u_int16 landmap::a_d_diff = 0;
#endif

mapsquare_tile::mapsquare_tile ()
{
    objnbr = 0;
    is_base = false;
#ifdef _DEBUG_
    cout << "mapsquare_tile() called, " << ++a_d_diff
         << " objects currently allocated\n";
#endif
}

mapsquare_tile::~mapsquare_tile ()
{
#ifdef _DEBUG_
    cout << "~mapsquare_tile() called, " << --a_d_diff
         << " objects currently allocated\n";
#endif
}

void mapsquare_tile::draw (mapview * mv, u_int16 x, u_int16 y)
{
    if (is_base)
    {
        u_int16 rx, ry;

        rx = (mv->posx > 0) ? this->x - mv->posx : this->x;
        ry = (mv->posy > 0) ? this->y - mv->posy : this->y;
        mv->m_map->pattern[objnbr]->draw
            ((rx * MAPSQUARE_SIZE - mv->offx) + x - mv->draw_offx,
             (ry * MAPSQUARE_SIZE - mv->offy) + y - mv->draw_offy, mv->da);
    }
    else
        base_tile->draw (mv, x, y);
}

#ifdef _EDIT_
void mapsquare_tile::draw_border (mapview * mv)
{
    if (is_base)
    {
        u_int16 rx, ry;

        rx = (mv->posx > 0) ? x - mv->posx : x;
        ry = (mv->posy > 0) ? y - mv->posy : y;
        mv->m_map->pattern[objnbr]->draw_border
            (rx * MAPSQUARE_SIZE, ry * MAPSQUARE_SIZE, mv->da);
    }
    else
        base_tile->draw_border (mv);
}

void mapsquare_tile::draw_base_tile (mapview * mv)
{
    if (is_base)
    {
        u_int16 rx, ry;

        rx = (mv->posx > 0) ? x - mv->posx : x;
        ry = (mv->posy > 0) ? y - mv->posy : y;
        mv->m_map->pattern[objnbr]->draw_base_tile
            (rx * MAPSQUARE_SIZE, ry * MAPSQUARE_SIZE, mv->da);
    }
    else
        base_tile->draw_base_tile (mv);
}
#endif

mapsquare_char::mapsquare_char ()
{
    mchar = NULL;
    is_base = false;
#ifdef _DEBUG_
    cout << "~mapsquare_char() called, " << --a_d_diff
         << " objects currently allocated\n";
#endif
}

mapsquare_char::~mapsquare_char ()
{
#ifdef _DEBUG_
    cout << "~mapsquare_char() called, " << --a_d_diff
         << " objects currently allocated\n";
#endif
}

void mapsquare_char::draw (mapview * mv, u_int16 x, u_int16 y)
{
    if (is_base)
    {
        u_int16 rx, ry;

        rx = (mv->posx > 0) ? this->x - mv->posx : this->x;
        ry = (mv->posy > 0) ? this->y - mv->posy : this->y;
        mchar->draw
            ((rx * MAPSQUARE_SIZE - mv->offx) + x,
             (ry * MAPSQUARE_SIZE - mv->offy) + y, mv->da);
    }
    else
        base_tile->draw (mv, x, y);
}

mapsquare::mapsquare ()
{
    type = 0;
    walkable = ALL_WALKABLE;
    base_begin = tiles.end ();
#ifdef _DEBUG_
    cout << "mapsquare() called, " << ++a_d_diff
         << " objects currently allocated\n";
#endif
}

mapsquare::~mapsquare ()
{
#ifdef _DEBUG_
    cout << "~mapsquare() called, " << --a_d_diff
         << " objects currently allocated\n";
#endif
}

landsubmap::landsubmap ()
{
    length = height = 0;
    land = NULL;
    m_map = NULL;
#ifdef _DEBUG_
    cout << "landsubmap() called, " << ++a_d_diff
         << " objects currently allocated\n";
#endif
}

landsubmap::landsubmap (u_int16 l, u_int16 h)
{
    allocmap (l, h);
    m_map = NULL;
#ifdef _DEBUG_
    cout << "landsubmap() called, " << ++a_d_diff
         << " objects currently allocated\n";
#endif
}

landsubmap::~landsubmap ()
{
    destroymap ();
#ifdef _DEBUG_
    cout << "~landsubmap() called, " << --a_d_diff
         << " objects currently allocated\n";
#endif
}

landsubmap & landsubmap::operator = (landsubmap & sm)
{
    u_int16 i, j;
    bool difsize = false;

    if (length != sm.length || height != sm.height)
    {
        difsize = true;
        destroymap ();
    }
    memcpy (this, &sm, sizeof (landsubmap));
    if (difsize)
        allocmap (length, height);
    for (i = 0; i < length; i++)
        for (j = 0; j < height; j++)
        {
            land[i][j] = sm.land[i][j];
        }
    return *this;
}

inline void landsubmap::allocmap (u_int16 l, u_int16 h)
{
    u_int16 i;

    length = l;
    height = h;
    land = new (mapsquare *)[l];
    for (i = 0; i < l; i++)
        land[i] = new mapsquare[h];
}

inline void landsubmap::destroymap ()
{
    u_int16 i;

    for (i = 0; i < length; i++)
        delete[] land[i];
    if (land)
        delete[] land;
    length = 0;
    height = 0;
}

void landsubmap::resize (u_int16 l, u_int16 h)
{
    u_int16 i, j;

    list <mapsquare_tile>::iterator it;
    if (length == l && height == h)
        return;
    mapsquare **ltemp = land;
    mapsquare **newland = new (mapsquare *)[l];

    for (i = 0; i < l; i++)
        newland[i] = new mapsquare[h];

    i = length;
    j = height;
    length = l;
    height = h;
    l = i;
    h = j;
    land = newland;

    for (i = 0; i < l && i < length; i++)
        for (j = 0; j < h && j < height; j++)
            for (it = ltemp[i][j].tiles.begin ();
                 it != ltemp[i][j].tiles.end (); it++)
            {
                if (it->is_base)
                    put_mapobject (i, j, it->objnbr);
            }

    for (i = 0; i < l; i++)
        delete[] ltemp[i];
    delete[] ltemp;
}

s_int8 landsubmap::get (igzstream& file)
{
    u_int16 i, j;

    destroymap ();

    length << file;
    height << file; 
    allocmap (length, height);
    for (j = 0; j < height; j++)
        for (i = 0; i < length; i++)
        {
            u_int16 k, t;

            k << file; 
            while (k)
            {
                t << file; 
                put_mapobject (i, j, t);
                k--;
            }
        }
    return 0;
}

#ifdef _EDIT_
s_int8 landsubmap::put (ogzstream& file)
{
    u_int16 i, j;

    length >> file;
    height >> file; 
    for (j = 0; j < height; j++)
        for (i = 0; i < length; i++)
        {
            list < mapsquare_tile >::iterator it;
            u_int16 k = 0;

            for (it = land[i][j].tiles.begin ();
                 it != land[i][j].tiles.end (); it++)
                if (it->is_base)
                    k++;
            k >> file; 
            for (it = land[i][j].tiles.begin ();
                 it != land[i][j].tiles.end (); it++)
                if (it->is_base)
                    it->objnbr >> file; 
        }
    return 0;
}
#endif

u_int16 landsubmap::get_length ()
{
    return length;
}

u_int16 landsubmap::get_height ()
{
    return height;
}

s_int8 landsubmap::put_mapobject (u_int16 px, u_int16 py, u_int16 patnbr)
{
    u_int16 i, j;
    mapsquare_tile t;

    // FIXME: Bad placement when overflowing the map from bottom right when the 
    // base tile isn't at right of the object.
    // Bad initialisation of i0/j0/ie/je ( must be init like remove_obj)

    u_int16 ie = m_map->pattern[patnbr]->maptpl::length ();
    u_int16 je = m_map->pattern[patnbr]->maptpl::height ();

    ie = px - m_map->pattern[patnbr]->basex + ie > length ? length - px : ie;
    je = py - m_map->pattern[patnbr]->basey + je > height ? height - py : je;

    u_int16 i0 =
        (px - m_map->pattern[patnbr]->basex) >
        0 ? 0 : m_map->pattern[patnbr]->basex - px;
    u_int16 j0 =
        (py - m_map->pattern[patnbr]->basey) >
        0 ? 0 : m_map->pattern[patnbr]->basey - py;

    // Steps:
    // -Find out where to start/stop placing the object (screen overflow)
    // -For each square, place it in the right order
    //  Don't forget to update the base_begin iterator to refer to the
    //  first base tile that's on the square.
    list < mapsquare_tile >::iterator it;

    if (patnbr > m_map->nbr_of_patterns)
        return -1;
    // First place the base tile, as others refers to it...
    t.objnbr = patnbr;
    t.is_base = true;
    t.x = px;
    t.y = py;
    for (it = land[px][py].tiles.begin ();
         it != land[px][py].tiles.end () && *(it->base_tile) <= t; it++);
    land[px][py].tiles.insert (it, t);
    it--;
    it->base_tile = it;

    // Get the base tile iterator for the others tiles
    t.base_tile = it;
    t.is_base = false;

    if (ie > length)
        ie = length;
    if (je > height)
        je = height;

    for (j = j0; j < je; j++)
        for (i = i0; i < ie; i++)
        {
            t.x = px - m_map->pattern[patnbr]->basex + i;
            t.y = py - m_map->pattern[patnbr]->basey + j;
            land[t.x][t.y].walkable &=
                m_map->pattern[patnbr]->placetpl[i][j].walkable;
            if (m_map->pattern[patnbr]->basex != i
                || m_map->pattern[patnbr]->basey != j)
            {
                for (it = land[t.x][t.y].tiles.begin ();
                     it != land[t.x][t.y].tiles.end () &&
                         *(it->base_tile) <= *(t.base_tile); it++);
                land[t.x][t.y].tiles.insert (it, t);
            }
        }
    for (it = land[px][py].tiles.begin ();
         it != land[px][py].tiles.end () && *(it->base_tile) < *it; it++);
    land[px][py].base_begin = it;
    return 0;
}

landmap::landmap ()
{
    pattern = NULL;
#ifdef _EDIT_
    mini_pattern = NULL;
#endif
    nbr_of_patterns = 0;
    submap = NULL;
    nbr_of_submaps = 0;
#ifdef _DEBUG_
    cout << "landmap() called, " << ++a_d_diff
         << " objects currently allocated\n";
#endif
}

void landmap::clear ()
{
    u_int16 i;

    if (submap)
    {
        for (i = 0; i < nbr_of_submaps; i++)
            delete submap[i];

        delete[]submap;
        submap = NULL;
    }
    if (pattern)
    {
        for (i = 0; i < nbr_of_patterns; i++)
            delete pattern[i];

        delete[] pattern;
        pattern = NULL;
    }

    /*  for(i=0;i<mapchar.size();i++)
        delete mapchar[i]; */
    mapchar.clear ();

#ifdef _EDIT_
    if (mini_pattern)
    {
        for (i = 0; i < nbr_of_patterns; i++)
            delete mini_pattern[i];

        delete[]mini_pattern;
        mini_pattern = NULL;
    }
#endif
    nbr_of_patterns = nbr_of_submaps = 0;
}

landmap::~landmap ()
{
    clear ();
#ifdef _DEBUG_
    cout << "~landmap() called, " << --a_d_diff
         << " objects currently allocated\n";
#endif
}

landmap & landmap::operator = (const landmap & lm)
{
    u_int16 i;

    clear ();
    nbr_of_patterns = lm.nbr_of_patterns;
    pattern = new (mapobject *)[nbr_of_patterns];
    for (i = 0; i < nbr_of_patterns; i++)
    {
        pattern[i] = new mapobject;
        *(pattern[i]) = *(lm.pattern[i]);
    }
    nbr_of_submaps = lm.nbr_of_submaps;
    submap = new (landsubmap *)[nbr_of_submaps];
    for (i = 0; i < nbr_of_submaps; i++)
    {
        submap[i] = new landsubmap;
        *(submap[i]) = *(lm.submap[i]);
    }
#ifdef _EDIT_
    mini_pattern = new (mapobject *)[nbr_of_patterns];
    for (i = 0; i < nbr_of_patterns; i++)
    {
        mini_pattern[i] = new mapobject;
        *(mini_pattern[i]) = *(lm.mini_pattern[i]);
    }
#endif
    return *this;
}

s_int8 landmap::get (igzstream& file)
{
    u_int16 i, j;
    mapobject *tobj;
    string tstr; 
    
    clear ();
    // Getting all mapobjects
    i << file; 
    for (; i; i--)
    {
        u_int16 j = 0;
        char c; 
        //  FIXME:  replace this string loading function with the normal one!
        tstr = ""; 
        do
        {
            c << file;
            tstr += c; 
            j++; 
        }
        while (c); 
        tobj = new mapobject;
        tobj->load (tstr);
        insert_mapobject (tobj, nbr_of_patterns, tstr.c_str ());
    }
    // Getting all submaps
    i << file; 
    for (j = 0; j < i; j++)
    {
        add_submap ();
        submap[j]->get (file);
    }
    return 0;
}

s_int8 landmap::load (string fname)
{
    igzstream file;
    s_int8 retvalue = -1;
    string fdef;

    fdef = MAPS_DIR;
    fdef += fname;
    file.open (fdef);
    if (!file.is_open ())
        return -1;
    if (fileops::get_version (file, 1, 1, fdef))
        retvalue = get (file);
    file.close ();
    filename_ = fname;
    return retvalue;
}

#ifdef _EDIT_
s_int8 landmap::put (ogzstream& file)
{
    u_int16 i;

    // version information 
    fileops::put_version (file, 1);

    // Put all mapobjects
    nbr_of_patterns >> file; 
    for (i = 0; i < nbr_of_patterns; i++)
    {
        char c = 0;
        u_int16 j = 0;

        while (j < objsrc[i].size ())
        {
            objsrc[i][j] >> file; 
            j++;
        }
        c >> file; 
    }
    // Put all submaps
    nbr_of_submaps >> file; 
    for (i = 0; i < nbr_of_submaps; i++)
    {
        submap[i]->put (file);
    }
    return 0;
}

s_int8 landmap::save (string fname)
{ 
    ogzstream file (MAPS_DIR + fname);
    u_int8 retvalue;

    if (!file.is_open ())
        return -1;
    retvalue = put (file);
    file.close (); 
    filename_ = fname;
    return retvalue;
}

#endif

void landmap::put_mapchar (mapcharacter * mchar, u_int16 smap,
                           u_int16 px, u_int16 py)
{
    mapsquare_char mschar;

    list <mapsquare_char>::iterator it;
    u_int16 sx = (px - mchar->basex < 0) ? 0 : px - mchar->basex;
    u_int16 sy = (py - mchar->basey < 0) ? 0 : py - mchar->basey;
    u_int16 ex = (sx + mchar->maptpl::length () > submap[smap]->length) ?
        submap[smap]->length : sx + mchar->maptpl::length ();
    u_int16 ey = (sy + mchar->maptpl::height () > submap[smap]->height) ?
        submap[smap]->height : sy + mchar->maptpl::height ();
    u_int16 i, j;

    // Placing the base tile first
    mschar.mchar = mchar;
    mschar.is_base = true;
    mschar.x = px;
    mschar.y = py;
    mschar.walkable =
        mchar->maptpl::placetpl[mchar->basex][mchar->basey].walkable ==
        ALL_WALKABLE;
    submap[smap]->land[px][py].mapchars.push_back (mschar);
    it = --submap[smap]->land[px][py].mapchars.end ();
    it->base_tile = it;
    mschar.base_tile = it;
    mschar.is_base = false;
    // Ready to place the rest now
    for (i = sx; i < ex; i++)
        for (j = sy; j < ey; j++)
            if (i != px || j != py)
            {
                mschar.x = i;
                mschar.y = j;
                mschar.walkable =
                    mchar->maptpl::placetpl[sx + mchar->basex - px][sy +
                                                                    mchar->basey - py].walkable == ALL_WALKABLE;
                submap[smap]->land[i][j].mapchars.push_back (mschar);
            }
}

void landmap::remove_mapchar (mapcharacter * mchar, u_int16 smap,
                              u_int16 px, u_int16 py)
{
    list <mapsquare_char>::iterator it;
    list <mapsquare_char>::iterator e;
    u_int16 sx = (px - mchar->basex < 0) ? 0 : px - mchar->basex;
    u_int16 sy = (py - mchar->basey < 0) ? 0 : py - mchar->basey;
    u_int16 ex = (sx + mchar->maptpl::length () > submap[smap]->length) ?
        submap[smap]->length : sx + mchar->maptpl::length ();
    u_int16 ey = (sy + mchar->maptpl::height () > submap[smap]->height) ?
        submap[smap]->height : sy + mchar->maptpl::height ();
    u_int16 i, j;


    for (i = sx; i < ex; i++)
        for (j = sy; j < ey; j++)
        {
            it = submap[smap]->land[i][j].mapchars.begin ();
            e = submap[smap]->land[i][j].mapchars.end ();

            while (it != e && it->mchar != mchar)
                it++;
            if (it != e)
                submap[smap]->land[px][py].mapchars.erase (it);
        }
}

s_int8 landmap::add_submap ()
{
    u_int16 i;
    landsubmap **tmp = submap;

    if (nbr_of_submaps == 255)
        return -1;
    submap = new (landsubmap *)[++nbr_of_submaps];
    for (i = 0; i < nbr_of_submaps - 1; i++)
        submap[i] = tmp[i];
    if (tmp)
        delete[]tmp;
    submap[nbr_of_submaps - 1] = new landsubmap;
    submap[nbr_of_submaps - 1]->m_map = this;
    return 0;
}

s_int8 landmap::add_submap (u_int16 l, u_int16 h)
{
    s_int8 err = add_submap ();

    if (err)
        return err;
    submap[nbr_of_submaps - 1]->resize (l, h);
    return 0;
}

s_int8 landmap::remove_submap (u_int16 nbr)
{
    if (nbr > nbr_of_submaps)
        return -1;
    landsubmap **tmp = submap;
    u_int16 i;

    submap = new (landsubmap *)[--nbr_of_submaps];
    for (i = 0; i < nbr; i++)
        submap[i] = tmp[i];
    delete tmp[nbr];

    for (i = nbr; i < nbr_of_submaps; i++)
        submap[i] = tmp[i + 1];
    if (tmp)
        delete[]tmp;
    return 0;
}

void landmap::update ()
{
    static u_int16 i;

    for (i = 0; i < nbr_of_patterns; i++)
    {
        pattern[i]->update ();
#ifdef _EDIT_
        mini_pattern[i]->update ();
#endif
    }
    for (i = 0; i < mapchar.size (); i++)
    {
        mapchar[i]->update ();
    }
}

s_int8 landmap::put_mapobject (u_int16 smap, u_int16 px, u_int16 py,
                               u_int16 patnbr)
{
    return submap[smap]->put_mapobject (px, py, patnbr);
}

void landmap::remove_mapobject (u_int16 smap,
                                list < mapsquare_tile >::iterator obj)
{
    u_int16 i, j;

    list <mapsquare_tile>::iterator it;
    u_int16 ie = pattern[obj->objnbr]->maptpl::length ();
    u_int16 je = pattern[obj->objnbr]->maptpl::height ();

    u_int16 i0 = (obj->base_tile->x - pattern[obj->objnbr]->basex) > 0 ? 0 :
        pattern[obj->objnbr]->basex - obj->base_tile->x;
    u_int16 j0 = (obj->base_tile->y - pattern[obj->objnbr]->basey) > 0 ? 0 :
        pattern[obj->objnbr]->basey - obj->base_tile->y;

    ie =
        obj->base_tile->x - pattern[obj->objnbr]->basex + ie >
        submap[smap]->length ? submap[smap]->length - obj->base_tile->x : ie;
    je =
        obj->base_tile->y - pattern[obj->objnbr]->basey + je >
        submap[smap]->height ? submap[smap]->height - obj->base_tile->y : je;

    ie -= i0;
    je -= j0;
    i0 += obj->base_tile->x - pattern[obj->objnbr]->basex;
    j0 += obj->base_tile->y - pattern[obj->objnbr]->basey;
    ie += i0;
    je += j0;

    for (j = j0; j < je; j++)
        for (i = i0; i < ie; i++)
        {
            it = submap[smap]->land[i][j].tiles.begin ();
            while (it != submap[smap]->land[i][j].tiles.end () &&
                   it->base_tile != obj->base_tile)
                it++;
            if (it != submap[smap]->land[i][j].tiles.end ())
            {
                submap[smap]->land[i][j].tiles.erase (it);
                for (it = submap[smap]->land[i][j].tiles.begin ();
                     it != submap[smap]->land[i][j].tiles.end (); it++);
                for (it = submap[smap]->land[i][j].tiles.begin ();
                     it != submap[smap]->land[i][j].tiles.end () &&
                         *(it->base_tile) < *it; it++);
                submap[smap]->land[i][j].base_begin = it;
            }
        }
}

s_int8 landmap::insert_mapobject (mapobject * an, u_int16 pos,
                                  const char *srcfile = "")
{
    mapobject **oldpat = pattern;
    u_int16 i, j, k;

    list <mapsquare_tile>::iterator it;

    if (pos > nbr_of_patterns)
        return -2;
    pattern = new (mapobject *)[++nbr_of_patterns];
    for (i = 0; i < pos; i++)
        pattern[i] = oldpat[i];
    pattern[pos] = an;
    pattern[pos]->play ();
    for (i = pos + 1; i < nbr_of_patterns; i++)
        pattern[i] = oldpat[i - 1];
    delete[]oldpat;
#ifdef _EDIT_
    oldpat = mini_pattern;
    mini_pattern = new (mapobject *)[nbr_of_patterns];
    for (i = 0; i < pos; i++)
        mini_pattern[i] = oldpat[i];
    mini_pattern[pos] = new mapobject;
    mini_pattern[pos]->zoom_to_fit (OBJSMPLSIZE, an);
    mini_pattern[pos]->play ();
    for (i = pos + 1; i < nbr_of_patterns; i++)
        mini_pattern[i] = oldpat[i - 1];
    delete[]oldpat;
#endif

    vector <string>::iterator itv = &objsrc[pos];
    string tt (srcfile);

    objsrc.insert (itv, tt);

    if (pos != nbr_of_patterns - 1)
    {
        for (k = 0; k < nbr_of_submaps; k++)
            for (j = 0; j < submap[k]->height; j++)
                for (i = 0; i < submap[k]->length; i++)
                    for (it = submap[k]->land[i][j].tiles.begin ();
                         it != submap[k]->land[i][j].tiles.end (); it++)
                        if (it->objnbr >= pos)
                            it->objnbr++;
    }
#ifdef _DEBUG_
    cout << "Added mapobject: " << nbr_of_patterns << " total in landmap.\n";
#endif
    return 0;
}

s_int8 landmap::delete_mapobject (u_int16 pos)
{
    mapobject **oldpat = pattern;
    u_int16 i, j, k;

    list <mapsquare_tile>::iterator it;

    if (pos > nbr_of_patterns - 1)
        return -2;
    pattern = new (mapobject *)[--nbr_of_patterns];
    for (i = 0; i < pos; i++)
        pattern[i] = oldpat[i];
    delete oldpat[pos];

    for (i = pos; i < nbr_of_patterns; i++)
        pattern[i] = oldpat[i + 1];
    delete[]oldpat;

#ifdef _EDIT_
    oldpat = mini_pattern;
    mini_pattern = new (mapobject *)[nbr_of_patterns];

    for (i = 0; i < pos; i++)
        mini_pattern[i] = oldpat[i];
    delete oldpat[pos];

    for (i = pos; i < nbr_of_patterns; i++)
        mini_pattern[i] = oldpat[i + 1];
    delete[]oldpat;
#endif

    for (k = 0; k < nbr_of_submaps; k++)
        for (j = 0; j < submap[k]->height; j++)
            for (i = 0; i < submap[k]->length; i++)
            {
                for (it = submap[k]->land[i][j].tiles.begin ();
                     it != submap[k]->land[i][j].tiles.end (); it++)
                {
                shamegoto:
                    if (it->objnbr > pos)
                        it->objnbr--;
                    else if (it->objnbr == pos)
                    {
                        it = submap[k]->land[i][j].tiles.erase (it);
                        if (it != submap[k]->land[i][j].tiles.end ())
                            goto shamegoto;
                        else
                            goto endgoto;
                    }
                }
            endgoto:
                for (it = submap[k]->land[i][j].tiles.begin ();
                     it != submap[k]->land[i][j].tiles.end () &&
                         *(it->base_tile) < *it; it++);
                submap[k]->land[i][j].base_begin = it;
            }
    vector <string>::iterator itv = &objsrc[pos];
    objsrc.erase (itv);
#ifdef _DEBUG_
    cout << "Removed mapobject: " << nbr_of_patterns <<
        " total in landmap.\n";
#endif
    if (!nbr_of_patterns)
    {
        pattern = NULL;
    }
    return 0;
}

#ifdef _EDIT_
void landmap::reset_objs ()
{
    u_int16 i;

    for (i = 0; i < nbr_of_patterns; i++)
    {
        pattern[i]->rewind ();
        mini_pattern[i]->rewind ();
    }
}

void landmap::editor ()
{
    mapview mview;

    mview.attach_map (this);
    mview.set_current_submap (0);
    mview.resize (11 * MAPSQUARE_SIZE, 11 * MAPSQUARE_SIZE);
    mview.editor ();
}
#endif
