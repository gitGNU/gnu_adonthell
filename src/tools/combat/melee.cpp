/***************************************************************************
                          tests.h  -  tests for melee algorithms
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Adonthell Development Team
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

 //Create 2 characters' stats for use int he test.

void melee::create_characters() {
	float dice;
	int count = 10;
	  srandom((unsigned int)time((time_t *)NULL));
	while (count--) {
		//populate character variables
	  switch (count) {
	  	case 10:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		a_str = dice;
	  		break;
	  	case 9:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		a_dex = dice;
	  		break;
	  	case 8:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		a_luck = dice;
	  		break;
	  	case 7:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		a_attack_skill = dice;
	  		break;
	  	case 6:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		a_defense_skill = dice;
	  		break;
	  	case 5:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		b_str = dice;
	  		break;
	  	case 4:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		b_dex = dice;
	  		break;
	  	case 3:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		b_luck = dice;
	  		break;
	  	case 2:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		b_attack_skill = dice;
	  		break;
	  	case 1:
	  		dice = float(random()%100);
	  		if (dice < 10)
	  			dice = 10;
	  		b_defense_skill = dice;
	  		break;
	  }
	}		
}
  		
// Calculate character attack, defense and luck ratios
void melee::calc_stats() {

	/*
	*
	* Calculate number ranges for hit, miss and critical hit/miss
	* for both characters  in 'A attack B' and 'B attack A' scenarios
	*
	* See docs on the list for explanation
	*/
	a_attack_range = (a_dex + a_attack_skill) / 200 * .6;
	a_defense_range = (a_dex + a_defense_skill) / 200 * .3;
	b_attack_range = (b_dex + b_attack_skill) / 200 * .6;
	b_defense_range = (b_dex + b_defense_skill) / 200 * .3;
	a_attack_total_allotment = (a_attack_range + b_defense_range) / .9;
	b_attack_total_allotment = (b_attack_range + a_defense_range) / .9;
	a_attack_luck_allotment = a_attack_total_allotment - (a_attack_range + b_defense_range);
	b_attack_luck_allotment = b_attack_total_allotment - (b_attack_range + a_defense_range);
	a_attack_a_real_luck = (a_attack_range / (a_attack_range + b_attack_range)) * a_attack_luck_allotment;
   a_attack_b_real_luck = a_attack_luck_allotment - a_attack_a_real_luck;
	b_attack_a_real_luck = (a_attack_range / (a_attack_range + b_attack_range)) * b_attack_luck_allotment;
	b_attack_b_real_luck = b_attack_luck_allotment - b_attack_a_real_luck;
}



// Roll some random numbers until we are within range
void melee::roll_dice() {
	float random_num = 1.1;
	srandom((unsigned int)time((time_t *)NULL));
	while (random_num > a_attack_total_allotment || random_num > b_attack_total_allotment) {
		// Seed rand()
		random_num = float((rand()%10000)) / 10000;
	}
	dice = random_num;
}

//******Temporary, for testing purposes*******
void melee::show_results(int parity) {
	//spew stats
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\nCharacter Statistics:\n=====================\n");
	printf("Character A:\n------------\n");
	printf("Dexterity: %d\n", int(a_dex));
	printf("Luck: %d\n", int(a_luck));
	printf("Attack Skill: %d\n", int(a_attack_skill));
	printf("Defense Skill: %d\n\n", int(a_defense_skill));
	printf("Chatacter B:\n------------\n");
	printf("Dexterity: %d\n", int(b_dex));
	printf("Luck: %d\n", int(b_luck));
	printf("Attack Skill: %d\n", int(b_attack_skill));
	printf("Defense Skill: %d\n\n", int(b_defense_skill));
	if ((parity % 2) > 0) {
		printf("***************\n* A attacks B *\n***************\n\n");
		printf("Character A Attack Range: 0-->%4.4f\n",a_attack_range);
		printf("Character B Defense Range: %4.4f-->%4.4f)\n", a_attack_range, (a_attack_range + b_defense_range));
		printf("Character A Percent of alloted luck: %4.2f\%\n", (a_attack_a_real_luck / a_attack_luck_allotment) * 100);
	  printf("Character B Percent of alloted luck: %4.2f\%\n", (a_attack_b_real_luck / a_attack_luck_allotment) * 100);
	  printf("\n\nResults:\n--------\n");
	  printf("Total Allotment: %4.4f\n", a_attack_total_allotment);
	  printf("Chance for A to hit (not including luck): %4.2f\%\n", a_attack_range / (a_attack_total_allotment - a_attack_luck_allotment) * 100);
	  printf("Number Rolled: %4.4f\n", dice);
	  if(dice <= a_attack_range)
	  	printf("HIT!\n\n");
	  if(dice > a_attack_range && (dice < (a_attack_total_allotment - a_attack_luck_allotment)))
	  	printf("MISS!\n\n");
	  if(dice > (a_attack_range + b_defense_range) && (dice < a_attack_total_allotment - a_attack_b_real_luck))
	  	printf("CTITICAL HIT!!\n\n");
	  if(dice	> (a_attack_total_allotment - a_attack_b_real_luck))
	  	printf("CRITICAL MISS!!");
	} else {
	 	printf("***************\n* B attacks A *\n***************\n\n");
		printf("Character B Attack Range: 0-->%4.4f\n",b_attack_range);
		printf("Character A Defense Range: %4.4f-->%4.4f)\n", b_attack_range, (b_attack_range + a_defense_range));
		printf("Character B Percent of alloted luck: %4.2f\%\n", (b_attack_b_real_luck / b_attack_luck_allotment) * 100);
	  printf("Character A Percent of alloted luck: %4.2f\%\n", (b_attack_a_real_luck / b_attack_luck_allotment) * 100);
	  printf("\n\nResults:\n--------\n");
	  printf("Total Allotment: %4.4f\n", b_attack_total_allotment);
	  printf("Chance for B to hit (not including luck): %4.2f\%\n", b_attack_range / (b_attack_total_allotment - b_attack_luck_allotment) * 100);
	  printf("Number Rolled: %4.4f\n", dice);
	  if(dice <= b_attack_range)
	  	printf("HIT!\n\n");
	  if(dice > b_attack_range && (dice < (b_attack_total_allotment - b_attack_luck_allotment)))
	  	printf("MISS!\n\n");
	  if(dice > (b_attack_range + a_defense_range) && (dice < b_attack_total_allotment - b_attack_b_real_luck))
	  	printf("CTITICAL HIT!!\n\n");
	  if(dice	> (b_attack_total_allotment - b_attack_b_real_luck))
	  	printf("CRITICAL MISS!!\n\n");
	}

}	

int main() {
	char garbage;
	int foo = 1;
	class melee test;
	while (foo > 0) {
		test.create_characters();
		test.calc_stats();
		test.roll_dice();
		test.show_results(foo);
		printf("\n\npaused...");
		garbage = getc(stdin);
		foo++;
	}
}
