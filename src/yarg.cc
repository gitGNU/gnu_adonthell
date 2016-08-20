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
 * @file   yarg.cc
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Yet Another Random Number Genarator.
 * 
 * 
 */

#include <time.h>
#include "yarg.h"

unsigned char yarg::schieberegister[16];// Here the random numbers are created
int yarg::min;                          // Smallest possible numbe
int yarg::max;                          // Largest possible number

// Init the generator with a seed and a range 
void yarg::init (std::string str, int mn, int mx)
{
	int s = str.length ();

	for (int i = 0; i < 16; i++)
   		schieberegister[i] = str[i%s];

    range (mn, mx);
}

// change the range of the generator
void yarg::range (int mn, int mx)
{
	min = (mx < mn ? mx : mn);
	max = (mn > mx ? mn : mx);
}

// Apply the time to get a bit extra variation
void yarg::randomize ()
{
    long int clk = clock ();
  
	int	s = sizeof (clk);

	for (int i = 0; i < 16; i++)
   		schieberegister[i] = schieberegister[i]^(clk >> (8+i)%s);
}

// Retrieve the next/nth random number
int yarg::get (int steps)
{
	for (int i = 0; i < steps; i++)
	folgezustand();

	return zufallszahl();
}

// Here the dice get rolled
void yarg::folgezustand ()
{
	unsigned char tmp = schieberegister[0];

	for (int i = 0; i < 15; i++)
   	{
		if (i%3 == 0) schieberegister[i] = schieberegister[i+1]^tmp;
		else schieberegister[i] = schieberegister[i+1];
	}

    schieberegister[15] = tmp;
}

// Here we generate the actual random number
int yarg::zufallszahl ()
{
    unsigned int zahl = 0;

	for (int i = 0; i < 16; i++)
   	    zahl += schieberegister[i] << (i*8)%(sizeof(zahl)*8);

    zahl = zahl >> 1;

    return (zahl%(max - min + 1)) + min;
}
