/*
   $Id$

   (C) Copyright 1998 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

// yet another random generator :)
class yarg
{
public:
    ~yarg () { }
    yarg ();
    yarg (char *s, int a, int e);          // Random seed and number range
   		
    int get (int = 1);                     // Return next (nth) random number
    void randomize ();                     // Mix with current time
    void init (char*, int, int);           // Like the constructor

private:
    void folgezustand ();                  // Calculate next state of register
    int	zufallszahl ();                    // Calculate the number from the register

    unsigned char schieberegister[16];     // Here the random numbers are created
    int min;                               // Smallest possible numbe
    int max;                               // Largest possible number
};
