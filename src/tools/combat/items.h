/***************************************************************************
                          items.h  -  Armor, Weapon attributes which affect melee combat
                             -------------------
    begin                : Wed Feb 16 2000
    copyright            : (C) 2000 by Adonthell Development team
    email                : adonthell@onelist.com
***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



/***************************************************************************

Item Class ranges, item IDS and item property definitions

***************************************************************************/




//Blade IDs, and properties
/////////////////////////////////////


#define  WEAP 100
//Blade  IDs  1..

#define DAGGER 0
#define SHORTSWORD 1
#define RAPIER 2
#define CUTLASS 3
#define BROADSWORD 4
#define SAMURAISWORD 5
#define TWOHANDEDSWORD 6
#define BASTARDSWORD 7
#define MASSIVESWORD 8

#define CLUB 9
#define CUDGELL 10
#define SPIKEDCLUB 11
#define MACE 12
#define MORNINGSTAR 13
#define WARMACE 14
#define WARHAMMER 15
#define TWOHANDEDMACE 16
#define WIDOWMAKER 17


#define MAX_WEAPONS 18//

//Dagger peoperties
#define DAGGER_BASE 10

//Short sword properties
#define SHORT_BASE 20

// Rapier properties
#define RAPIER_BASE 25

//Cutlass properties
#define CUTLASS_BASE 30

//Broadsword properties
#define BROAD_BASE 45

//Samurai sword properties
#define SAMURAI_BASE 50

//Two-handed sword properties
#define TWOHANDED_BASE 60

//Bastard sword properties
#define BASTARD_BASE 75

//Massive sword properties
#define MASSIVE_BASE 80

//Club properties
#define CLUB_BASE 5

//Cudgell properties
#define CUDGELL_BASE 10

//Spiked club properties
#define SPIKEDCLUB_BASE 15

//Mace properties
#define MACE_BASE 25

//Morning start properties
#define MORNINGSTAR_BASE 40

//War mace properties
#define WARMACE_BASE 50

//War hammer properties
#define WARHAMMER_BASE 60

//Two-handed mace properties
#define TWOHANDEDMACE_BASE 75

//Widowmaker properties
#define WIDOWMAKER_BASE 90





//Armor IDs and properties
////////////////////////////////////
#define ARMOR 0
//Armor  IDs  1..99
#define CLOTH 0
#define LEATHER 1
#define STUDDED 2
#define RINGMAIL 3
#define CHAINMAIL 4
#define CHAIN_PLATE 5
#define PLATEMAIL 6
#define MAGIC_CHAIN 7
#define MAGIC_PLATE 8
#define ARMOR_MAX 9 //number items in this item class


//Armor item properties


//Cloth properties
#define CLOTH_AR 5
#define CLOTH_THRUST 0
#define CLOTH_CHOP 1
#define CLOTH_SMASH 0
#define CLOTH_DEX_PEANALTY 0

//Leather properties
#define LEATHER_AR 15
#define LEATHER_THRUST 1
#define LEATHER_CHOP 2
#define LEATHER_SMASH 1
#define LEATHER_DEX_PEANALTY 1

//Studded properties
#define STUDDED_AR 25
#define STUDDED_THRUST 1
#define STUDDED_CHOP 2
#define STUDDED_SMASH 2
#define STUDDED_DEX_PEANALTY 2

//Ringmail properties
#define RINGMAIL_AR 40
#define RINGMAIL_THRUST 2
#define RINGMAIL_CHOP 2
#define RINGMAIL_SMASH 2
#define RINGMAIL_DEX_PEANALTY 3

//Chainmail properties
#define CHAINMAIL_AR 60
#define CHAINMAIL_THRUST 3
#define CHAINMAIL_CHOP 3
#define CHAINMAIL_SMASH 2
#define CHAINMAIL_DEX_PEANLATY 4

//Chain-plate properties
#define CHAIN_PLATE_AR 70
#define CHAIN_PLATE_THRUST 3
#define CHAIN_PLATE_CHOP 3
#define CHAIN_PLATE_SMASH 3
#define CHAIN_PLATE_DEX_PEANLATY 5

//Platemail properties
#define PLATEMAIL_AR 90
#define PLATEMAIL_THRUST 4
#define PLATEMAIL_CHOP 4
#define PLATEMAIL_SMASH 4
#define PLATEMAIL_DEX_PEANALTY 7

//Magic chain properties
#define MAGIC_CHAIN_AR 90
#define MAGIC_CHAIN_THRUST 4
#define MAGIC_CHAIN_CHOP 4
#define MAGIC_CHAIN_SMASH 4
#define MAGIC_CHAIN_DEX_PEANLATY 3

//properties
#define MAGIC_PLATE_AR 100
#define MAGIC_PLATE_THRUST 5
#define MAGIC_PLATE_CHOP 5
#define MAGIC_PLATE_SMASH 5
#define MAGIC_PLATE_DEX_PEANLATY 5

