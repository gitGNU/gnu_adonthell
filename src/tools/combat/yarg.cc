// yet another random generator
// (C) 1998 Kai Sterker

#include <time.h>
#include "combat_classes.h"
yarg::yarg ()
{ 
   init(" ", 0, 100); 
}

yarg::yarg (char *s, int a, int e) 
{ 
   init(s, a, e);   
}

// Init the generator with a seed and a range 
void yarg::init (char *str, int mn, int mx)
{
	int s = strlen (str);

	for (int i = 0; i < 16; i++)
   		schieberegister[i] = str[i%s];

	min = (mx < mn ? mx : mn);
	max = (mn > mx ? mn : mx);
}

// Apply the time to get a bit extra variation
void yarg::randomize ()
{
    long int clk = clock ();
	int	s = sizeof (clk);

	for (int i = 0; i < 16; i++)
   		schieberegister[i] = schieberegister[i]^(clk >> (8 * i%s));
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
   	    zahl += schieberegister[i] << (i*2);

    return (zahl%(max - min + 1)) + min;
}