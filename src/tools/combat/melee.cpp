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
 #include <stdlib.h>
 #include <time.h>

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




