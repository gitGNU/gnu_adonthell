/***************************************************************************
                          melee.cpp  -  tests for melee hit determination.
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
#include <iostream.h>
#include <stdlib.h>
 #include <time.h>
 #include "items.h"


//Definitions for structures containing item properties.
struct weapon {   int base; //weapon base damage
							};
								
struct armor {	int ar; //armor AR
							int thrust; //thrust defense bonus
							int chop; //chop defense bonus
							int smash; //smash defense bonus
							int dexhit; //dexterity peanlaty
						};


struct weapon weapons[MAX_WEAPONS] ={
	{DAGGER_BASE},
	{SHORT_BASE},
	{RAPIER_BASE},
	{CUTLASS_BASE},
	{BROAD_BASE},
	{SAMURAI_BASE},
	{TWOHANDED_BASE},
	{BASTARD_BASE},
	{MASSIVE_BASE},
	{CLUB_BASE},
	{CUDGELL_BASE},
	{SPIKEDCLUB_BASE},
	{MACE_BASE},
	{MORNINGSTAR_BASE},
	{WARMACE_BASE},
	{WARHAMMER_BASE},
	{TWOHANDEDMACE_BASE},
	{WIDOWMAKER_BASE}
};

struct armor armors[ARMOR_MAX] = {
	{CLOTH_AR, CLOTH_THRUST, CLOTH_CHOP, CLOTH_SMASH, CLOTH_DEX_PEANALTY},
	{LEATHER_AR, LEATHER_THRUST, LEATHER_CHOP, LEATHER_SMASH, LEATHER_DEX_PEANALTY},
	{STUDDED_AR, STUDDED_THRUST, STUDDED_CHOP, STUDDED_SMASH, STUDDED_DEX_PEANALTY},
	{RINGMAIL_AR, RINGMAIL_THRUST, RINGMAIL_CHOP, RINGMAIL_SMASH, RINGMAIL_DEX_PEANALTY},
	{CHAINMAIL_AR, CHAINMAIL_THRUST, CHAINMAIL_CHOP, CHAINMAIL_SMASH, CHAINMAIL_DEX_PEANLATY},
	{CHAIN_PLATE_AR, CHAIN_PLATE_THRUST, CHAIN_PLATE_CHOP,  CHAIN_PLATE_SMASH, CHAIN_PLATE_DEX_PEANLATY},
	{PLATEMAIL_AR, PLATEMAIL_THRUST, PLATEMAIL_CHOP, PLATEMAIL_SMASH,  PLATEMAIL_DEX_PEANALTY},
	{MAGIC_CHAIN_AR, MAGIC_CHAIN_THRUST, MAGIC_CHAIN_CHOP, MAGIC_CHAIN_SMASH, MAGIC_CHAIN_DEX_PEANLATY},
	{MAGIC_PLATE_AR, MAGIC_PLATE_THRUST, MAGIC_PLATE_CHOP, MAGIC_PLATE_SMASH, MAGIC_PLATE_DEX_PEANLATY}
};







//Create 2 characters' stats for use in the test.

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
	a_defense_range = (a_dex + a_defense_skill) / 200 * .4;
	b_attack_range = (b_dex + b_attack_skill) / 200 * .6;
	b_defense_range = (b_dex + b_defense_skill) / 200 * .4;
	a_attack_total_allotment = (a_attack_range + b_defense_range) / .9;
	b_attack_total_allotment = (b_attack_range + a_defense_range) / .9;
	a_attack_luck_allotment = a_attack_total_allotment - (a_attack_range + b_defense_range);
	b_attack_luck_allotment = b_attack_total_allotment - (b_attack_range + a_defense_range);
	a_attack_a_real_luck = (a_luck / (a_luck + b_luck)) * a_attack_luck_allotment;
   a_attack_b_real_luck = a_attack_luck_allotment - a_attack_a_real_luck;
	b_attack_a_real_luck = (a_luck / (a_luck + b_luck)) * b_attack_luck_allotment;
	b_attack_b_real_luck = b_attack_luck_allotment - b_attack_a_real_luck;
}

	//Calculate damage inflicted by a sucessful melee blow
	int melee::calc_damage (
		int parity,		// who is attacking whom
		int action,		// hit = 0, critical_hit = 1, critical_miss = 2
		int method		//  Attack method
	) {
	
	float a_modifier;  //calculated value for character A
	float b_modifier; //calculated value for character B
   float a_ratio;
	float b_ratio;
	int result; 	//returned value
	
	//A is attacking, time for the number crunching.
	if ( (parity % 2) > 0) {
		switch (method) {
			case  0:			//thrust
				a_modifier = (weapons[a_weapon].base + a_str  + armors[b_armor].dexhit) * (a_attack_range / .6);
				printf("\na_modifier: %3.4f\n", a_modifier);
				b_modifier = (armors[b_armor].ar * .5 + armors[b_armor].thrust + b_dex) * (b_defense_range / .4);	
   				printf("b_modifier: %3.4f\n", b_modifier);
				a_ratio = a_modifier / (a_modifier + b_modifier) ;
				printf("a_ratio: %3.4f\n", a_ratio);
				b_ratio = b_modifier / (a_modifier + b_modifier);
				printf("b_ratio: %3.4f\n", b_ratio);
				break;
			case 1: 				//chop
          	a_modifier = (weapons[a_weapon].base + a_str  + armors[b_armor].dexhit) * (a_attack_range / .6);
				printf("\na_modifier: %3.4f\n", a_modifier);
				b_modifier = (armors[b_armor].ar * .5 + armors[b_armor].chop + b_dex) * (b_defense_range / .4);	
   				printf("b_modifier: %3.4f\n", b_modifier);
				a_ratio = a_modifier / (a_modifier + b_modifier) ;
				printf("a_ratio: %3.4f\n", a_ratio);
				b_ratio = b_modifier / (a_modifier + b_modifier);
				printf("b_ratio: %3.4f\n", b_ratio);
				break;
			case 2: 				//smash
          	a_modifier = (weapons[a_weapon].base + a_str  + armors[b_armor].dexhit) * (a_attack_range / .6);
				printf("\na_modifier: %3.4f\n", a_modifier);
				b_modifier = (armors[b_armor].ar * .5 + armors[b_armor].smash + b_dex) * (b_defense_range / .4);	
   				printf("b_modifier: %3.4f\n", b_modifier);
				a_ratio = a_modifier / (a_modifier + b_modifier) ;
				printf("a_ratio: %3.4f\n", a_ratio);
				b_ratio = b_modifier / (a_modifier + b_modifier);
				printf("b_ratio: %3.4f\n", b_ratio);
				break;
		}
		switch (action) {
			case 0:			//hit
			result = int(weapons[a_weapon].base * a_ratio) ;
				return result;
			case 1:		//critical hit
				result = int(weapons[a_weapon].base * a_ratio);
				return int(result * 1.5);
			case 2:		//criitcal miss
 		    	
				return result;
		}
	} else {
		switch (method) {
			case  0:			//thrust
				b_modifier = (weapons[b_weapon].base + b_str  + armors[a_armor].dexhit) * (b_attack_range / .6);
				printf("\nb_modifier: %3.4f\n", b_modifier);
				a_modifier = (armors[a_armor].ar * .5 + armors[a_armor].thrust + a_dex) * (a_defense_range / .4);	
   				printf("a_modifier: %3.4f\n", a_modifier);
				a_ratio = a_modifier / (a_modifier + b_modifier) ;
				printf("a_ratio: %3.4f\n", a_ratio);
				b_ratio = b_modifier / (a_modifier + b_modifier);
				printf("b_ratio: %3.4f\n", b_ratio);
				break;
			case 1: 				//chop
          	b_modifier = (weapons[b_weapon].base + b_str  + armors[a_armor].dexhit) * (b_attack_range / .6);
				printf("\na_modifier: %3.4f\n", b_modifier);
				a_modifier = (armors[a_armor].ar * .5 + armors[a_armor].chop + a_dex) * (a_defense_range / .4);	
   				printf("a_modifier: %3.4f\n", a_modifier);
				a_ratio = a_modifier / (a_modifier + b_modifier) ;
				printf("a_ratio: %3.4f\n", a_ratio);
				b_ratio = b_modifier / (a_modifier + b_modifier);
				printf("b_ratio: %3.4f\n", b_ratio);
				break;
			case 2: 				//smash
          	b_modifier = (weapons[b_weapon].base + b_str  + armors[a_armor].dexhit) * (b_attack_range / .6);
				printf("\na_modifier: %3.4f\n", b_modifier);
				a_modifier = (armors[a_armor].ar * .5 + armors[a_armor].smash + a_dex) * (a_defense_range / .4);	
   				printf("a_modifier: %3.4f\n", a_modifier);
				a_ratio = a_modifier / (a_modifier + b_modifier) ;
				printf("a_ratio: %3.4f\n", a_ratio);
				b_ratio = b_modifier / (a_modifier + b_modifier);
				printf("b_ratio: %3.4f\n", b_ratio);
				break;
		}
		switch (action) {
			case 0: 			//hit
				result = int(weapons[b_weapon].base * b_ratio) ;
				return result;
			case 1:		//critical hit
				result = int(weapons[b_weapon].base * b_ratio);
				return int(result * 1.5);
			case 2:		//criitcal miss
 		    	
				return result;
		}
	}
}
		
 		

