/*
   (C) Copyright 1998/2002 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   yarg.h
 * @author Kai Sterker <kai.sterker@gmail.com>
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
