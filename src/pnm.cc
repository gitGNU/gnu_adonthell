/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   pnm.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the pnm static class.
 * 
 * 
 */


#include "pnm.h"
#include <stdlib.h>
#include <string.h>

using namespace std; 

 
void *pnm::get (SDL_RWops * file, u_int16 * length, u_int16 * height)
{
    void *image;
    char sign[10];
    u_int16 l, h;
    u_int32 i = 0;

    SDL_RWread (file, sign, 1, 2);
    if ((sign[0] != 'P') || (sign[1] != '6'))
    {
        printf ("Invalid format.\n");
        return (NULL);
    }
    pnm_gotonextline (file);
    /* Getting height and length */
    while (pnm_checkforcomment (file));
    do
    {
        SDL_RWread (file, &sign[i], 1, 1);
        i++;
    }
    while (sign[i - 1] != ' ');
    sign[i - 1] = 0;
    l = atoi (sign);
    i = 0;
    do
    {
        SDL_RWread (file, &sign[i], 1, 1);
        i++;
    }
    while (sign[i - 1] != '\n');
    sign[i - 1] = 0;
    h = atoi (sign);
    /* Going to next line */
    pnm_gotonextline (file);
    /* Reading the image */
    image = calloc (l * h, 3);
    SDL_RWread (file, image, 1, l * h * 3);
    if (length)
        *length = l;
    if (height)
        *height = h;
    return (image);
}

void pnm::put (SDL_RWops * file, void *image, u_int16 length, u_int16 height)
{
    char s[30];

    sprintf (s, "P6\n%d %d\n255\n", length, height);
    SDL_RWwrite (file, s, sizeof (char), strlen (s));

    SDL_RWwrite (file, image, 1, length * height * 3);
}
 



// Private methods.



void pnm::pnm_gotonextline (SDL_RWops * file)
{
    char buff;

    do
    {
        SDL_RWread (file, &buff, 1, 1);
    }
    while (buff != '\n');
}

int pnm::pnm_checkforcomment (SDL_RWops * file)
{
    char buff;

    SDL_RWread (file, &buff, 1, 1);
    if (buff == '#')
    {
        pnm_gotonextline (file);
        return (1);
    }
    else
    {
        SDL_RWseek (file, -1, SEEK_CUR);
        return (0);
    }
}
