/***************************************************************************
                          combat-test.cpp  -  description
                             -------------------
    begin                : Fri Feb 11 2000
    copyright            : (C) 2000 by Adonthell Development team
    email                : adonthell@onelist.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "combat_classes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void show_results(int);
float get_dice(int);



//create melee and yarg classes
class yarg roler;
class melee test;



//Main test loop

int main (void) {
	
	char seed;
   //Initialize roller for first 1000 random numbers
	srandom((unsigned int)time((time_t *)NULL));
   	seed = ((rand()%100000000));
	roler.init(&seed, 0, 10000);
	roler.randomize();

	char garbage;
	int foo = 1;
	
	while (foo > 0) {
		//create 2 random characters, crunch players stats, show results.
		test.create_characters();
		test.calc_stats();
		show_results(foo);
		printf("\n\npaused...");
		garbage = getc(stdin);
	   	foo++;
	}
}


//Wrapper for roler.get() to avert repetition.

float get_dice (int pass) {
	char seed;
	float number = 1.1;
	
	//Total allotments depends on who is attacking whom.
	// Determine this and roll until we are within range.
	if ((pass % 2) > 0) {
		while (number > test.a_attack_total_allotment) {
	    	   number = float (roler.get(pass)) / 10000;
		}
	} else {
		while (number > test.b_attack_total_allotment) {
			number = float (roler.get(pass)) / 10000;
		}
	}

	//Re-seed roler after 1000 uses.
	if (pass % 1000 == 0) {
   		srandom((unsigned int)time((time_t *)NULL));
   		seed = ((rand()%100000000));
		roler.init(&seed, 0, 10000);
		roler.randomize();
	}
   return number;
}


//******Temporary, for testing purposes*******
void show_results(int parity) {
	float dice;
   // get random number for this turn
	dice = get_dice(parity);

	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\nCharacter Statistics:\n=====================\n");
	printf("Character A:\n------------\n");
	printf("Dexterity: %d\n", int(test.a_dex));
	printf("Luck: %d\n", int(test.a_luck));
	printf("Attack Skill: %d\n", int(test.a_attack_skill));
	printf("Defense Skill: %d\n\n", int(test.a_defense_skill));
	printf("Chatacter B:\n------------\n");
	printf("Dexterity: %d\n", int(test.b_dex));
	printf("Luck: %d\n", int(test.b_luck));
	printf("Attack Skill: %d\n", int(test.b_attack_skill));
	printf("Defense Skill: %d\n\n", int(test.b_defense_skill));
	if ((parity % 2) > 0) {
		printf("***************\n* A attacks B *\n***************\n\n");
		printf("Character A Attack Range: 0-->%4.4f\n",test.a_attack_range);
		printf("Character B Defense Range: %4.4f-->%4.4f)\n", test.a_attack_range, (test.a_attack_range + test.b_defense_range));
		printf("Character A Percent of alloted luck: %4.2f\%\n", (test.a_attack_a_real_luck / test.a_attack_luck_allotment) * 100);
	   printf("Character B Percent of alloted luck: %4.2f\%\n", (test.a_attack_b_real_luck / test.a_attack_luck_allotment) * 100);
	   printf("\n\nResults:\n--------\n");
	   printf("Total Allotment: %4.4f\n", test.a_attack_total_allotment);
	   printf("Chance for A to hit (not including luck): %4.2f\%\n", test.a_attack_range / (test.a_attack_total_allotment - test.a_attack_luck_allotment) * 100);
	   printf("Number Rolled: %4.4f\n", dice);
	   if(dice <= test.a_attack_range)
	  		printf("HIT!\n\n");
	  	if(dice > test.a_attack_range && (dice < (test.a_attack_total_allotment - test.a_attack_luck_allotment)))
	  		printf("MISS!\n\n");
	  	if(dice > (test.a_attack_range + test.b_defense_range) && (dice < test.a_attack_total_allotment - test.a_attack_b_real_luck))
	  		printf("CTITICAL HIT!!\n\n");
	  	if(dice	> (test.a_attack_total_allotment - test.a_attack_b_real_luck))
	  		printf("CRITICAL MISS!!");
	} else {
	 	printf("***************\n* B attacks A *\n***************\n\n");
		printf("Character B Attack Range: 0-->%4.4f\n",test.b_attack_range);
		printf("Character A Defense Range: %4.4f-->%4.4f)\n", test.b_attack_range, (test.b_attack_range + test.a_defense_range));
		printf("Character B Percent of alloted luck: %4.2f\%\n", (test.b_attack_b_real_luck / test.b_attack_luck_allotment) * 100);
	  	printf("Character A Percent of alloted luck: %4.2f\%\n", (test.b_attack_a_real_luck / test.b_attack_luck_allotment) * 100);
	  	printf("\n\nResults:\n--------\n");
	  	printf("Total Allotment: %4.4f\n", test.b_attack_total_allotment);
	  	printf("Chance for B to hit (not including luck): %4.2f\%\n", test.b_attack_range / (test.b_attack_total_allotment - test.b_attack_luck_allotment) * 100);
	  	printf("Number Rolled: %4.4f\n", dice);
	  	if(dice <= test.b_attack_range)
	  		printf("HIT!\n\n");
	  	if(dice > test.b_attack_range && (dice < (test.b_attack_total_allotment - test.b_attack_luck_allotment)))
	  		printf("MISS!\n\n");
	  	if(dice > (test.b_attack_range + test.a_defense_range) && (dice < test.b_attack_total_allotment - test.b_attack_b_real_luck))
	  		printf("CTITICAL HIT!!\n\n");
	  	if(dice	> (test.b_attack_total_allotment - test.b_attack_b_real_luck))
	  		printf("CRITICAL MISS!!\n\n");
	}

}	

