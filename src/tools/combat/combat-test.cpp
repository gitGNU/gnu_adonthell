/***************************************************************************
                          combat-test.cpp  -  description
                             -------------------
    begin                : Fri Feb 11 2000
    copyright            : (C) 2000 by Adonthell Development team
    email                : adonthell@onelist.com
    $Id$ 
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
#include <iostream.h>
#include <time.h>
#include "items.h"
void show_results(int);
float get_dice(int);
int attack_method = 4;


//create melee and yarg classes
class yarg roler;
class melee test;

int a_hit = 0;
int b_hit = 0;

//Main test loop

int main (void) {
  int random_chars = 2;
	int rounds = 0;	
	char seed;
   //Initialize roller for first 1000 random numbers
	srandom((unsigned int)time((time_t *)NULL));
   	seed = ((rand()%100000000));
	roler.init(&seed, 0, 10000);
	roler.randomize();

	char garbage;
	printf("\n\nWeapon Properties\n===============\n\n");
	printf("WEAPON_ID	WEAPON NAME	BASE_DAMAGE\n-------------------------------------------\n");
	printf("%-8d	Dagger              %-10d\n", DAGGER, DAGGER_BASE);
  printf("%-8d	Short Sword         %-10d\n", SHORTSWORD, SHORT_BASE);
	printf("%-8d	Rapier              %-10d\n", RAPIER, RAPIER_BASE);
	printf("%-8d	Cutlass             %-10d\n", CUTLASS, CUTLASS_BASE);
	printf("%-8d	Broad Sword         %-10d\n", BROADSWORD, BROAD_BASE);
  printf("%-8d	Samurai Sword       %-10d\n", SAMURAISWORD, SAMURAI_BASE);
	printf("%-8d	Two Handed Sword    %-10d\n", TWOHANDEDSWORD, TWOHANDED_BASE);
	printf("%-8d	Bastard Sword       %-10d\n", BASTARDSWORD, BASTARD_BASE);
	printf("%-8d	Massive Sword       %-10d\n", MASSIVESWORD, MASSIVE_BASE);
  printf("\n\nEnter desired weapon ID for Character A:");
	cin >> test.a_weapon;
	garbage = getc(stdin);
	printf("Enter desired weapon ID for Character B:");
	cin >> test.b_weapon;
	garbage = getc(stdin);
	printf("\n\n*****\nNOTE: Consult items.h for Armor properties, I'm not going to list them here.\n*****\n");
  printf("Current Armor IDs range from 0 to %d\n", ARMOR_MAX - 1);
	printf("Enter Armor ID for character A:");
	cin >> test.a_armor;
	garbage = getc(stdin);
	printf("Enter Armor ID for Character B:");
  cin >> test.b_armor;
	garbage = getc(stdin);
  while (random_chars != 0 && random_chars != 1){
		printf("\nCreate random character stats?[0/1]:");
		cin >> random_chars;
	}
	if (random_chars == 0) {
		// get char A's stats
		cout << "\na_luck:";
	 	cin >> test.a_luck;
		garbage = getc(stdin);
		cout << "\na_str:";
		cin >> test.a_str;
		garbage = getc(stdin);
		cout << "\na_sdex:";
		cin >> test.a_dex;
		garbage = getc(stdin);
		cout << "\na_attack_skill:";
		cin >> test.a_attack_skill;
		garbage = getc(stdin);
		cout << "\na_defense_skill:";
		cin >> test.a_defense_skill;
	 	garbage = getc(stdin);
		
		//get char B's stats:
		cout << "\nb_luck:";
	 	cin >> test.b_luck;
		garbage = getc(stdin);
		cout << "\nb_str:";
		cin >> test.b_str;
		garbage = getc(stdin);
		cout << "\nb_sdex:";
		cin >> test.b_dex;
		garbage = getc(stdin);
		cout << "\nb_attack_skill:";
		cin >> test.b_attack_skill;
		garbage = getc(stdin);
		cout << "\nb_defense_skill:";
		cin >> test.b_defense_skill;
	 	garbage = getc(stdin);
	}
	
	while (1) {
  	//create 2 random characters, crunch players stats, show results.
		if (random_chars == 1) {
			test.create_characters();
		}
		test.calc_stats();
		do {
			while (attack_method < 0 || attack_method > 3) {
				cout << "\nChoose attack method[0/1/2]:";
				cin >> attack_method;
				garbage = getc(stdin);
			}
			show_results(++rounds);
			attack_method = 4;
		} while (a_hit < 20 && b_hit < 20);
	
		printf ("%s has won after %i rounds", (a_hit < b_hit ? "A" : "B"), rounds/2);
		printf("\npaused...");
		garbage = getc(stdin);
		rounds = 0;
		a_hit = 0;
		b_hit = 0;
	}
}


//Wrapper for roler.get() to avert repetition.

float get_dice (int pass) {
	char seed;
	float number = 1.2;
	
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
		number = float (roler.get(pass)) / 10000;
	}
   return number;
}


//******Temporary, for testing purposes*******
void show_results(int parity) {
	float dice;
  int outcome;
  int a_armor;
	int a_weapon;
	int b_weapon;
	int b_armor;
	char garbage;
	// get random number for this turn
	dice = get_dice(parity);
 	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\n\nCharacter Statistics:\n=====================\n");
	printf("Character A:\n------------\n");
	printf("Strength: %d\n", int(test.a_str));
	printf("Hit Points: %d\n", int(test.a_str * 4));
	printf("Dexterity: %d\n", int(test.a_dex));
	printf("Luck: %d\n", int(test.a_luck));
	printf("Attack Skill: %d\n", int(test.a_attack_skill));
	printf("Defense Skill: %d\n\n", int(test.a_defense_skill));
	printf("Chatacter B:\n------------\n");
	printf("Strength: %d\n", int(test.b_str));
	printf("Hit Points: %d\n", int(test.b_str * 4));
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
	  if(dice <= test.a_attack_range) {
	  	printf("HIT!\n\n");
			outcome = test.calc_damage(parity, 0, attack_method); //parity, action, attack method
			printf("\ncalcdamage results: %d\n", outcome);
			b_hit += 1;
		}
	  if(dice > test.a_attack_range && (dice < (test.a_attack_total_allotment - test.a_attack_luck_allotment)))
	  	printf("MISS!\n\n");
			outcome = test.calc_damage(parity, 3, attack_method);
	  if(dice > (test.a_attack_range + test.b_defense_range) && (dice < test.a_attack_total_allotment - test.a_attack_b_real_luck)) {
	  	printf("CTITICAL HIT!!\n\n");
		outcome = test.calc_damage(parity, 1, attack_method); //parity, action, attack method
		printf("\ncalcdamage results: %d\n", outcome);
		b_hit += 3; }
	  if(dice	> (test.a_attack_total_allotment - test.a_attack_b_real_luck) && dice <= test.a_attack_total_allotment) {
	  	printf("CRITICAL MISS!!\n\n");
			outcome = test.calc_damage(parity, 2, attack_method); //parity, action, attack method
			a_hit += 1;
		}
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
	  if(dice <= test.b_attack_range) {
	  	printf("HIT!\n\n");
			outcome = test.calc_damage(parity, 0, attack_method); //parity, action, attack method
			printf("\ncalcdamage results: %d\n", outcome);
			a_hit += 1;
		}
	  if(dice > test.b_attack_range && (dice < (test.b_attack_total_allotment - test.b_attack_luck_allotment)))
	  	printf("MISS!\n\n");
			outcome = test.calc_damage(parity, 3, attack_method);
	  if(dice > (test.b_attack_range + test.a_defense_range) && (dice < test.b_attack_total_allotment - test.b_attack_b_real_luck)) {
	  	printf("CTITICAL HIT!!\n\n");
			outcome = test.calc_damage(parity, 1, attack_method); //parity, action, attack method
			printf("\ncalcdamage results: %d\n", outcome);
			a_hit += 3;
		}
	  if(dice	> (test.b_attack_total_allotment - test.b_attack_b_real_luck) && dice <= test.b_attack_total_allotment) {
	  	printf("CRITICAL MISS!!\n\n");
			outcome = test.calc_damage(parity, 2, attack_method); //parity, action, attack method
			b_hit += 1;
		}
	}
	printf("A IS HIT: %i\nB IS HIT: %i\n\n", a_hit, b_hit);
}	

