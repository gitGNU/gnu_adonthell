/*
   $Id$

   (C) Copyright 1998/2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

/**
 * @file   yarg.h
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Yet Another Random Number Genarator.
 * 
 * 
 */

#include <string>

class yarg
{
public:
    static int get (int = 1);                // Return next (nth) random number
    static void randomize ();                // Mix with current time
    static void init (std::string, int = 0, int = 100);// Seed the generator
    static void range (int, int);            // Change the range
    
private:
    static void folgezustand ();             // Calculate next state of register
    static int	zufallszahl ();              // Calculate the number from the register

    static unsigned char schieberegister[16];// Here the random numbers are created
    static int min;                          // Smallest possible numbe
    static int max;                          // Largest possible number
};
