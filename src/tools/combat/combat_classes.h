#ifndef _combat_classes_h
#define _combat_classes_h

#include "../../types.h"

class melee {
	
	private:
	//Character 'A' attributes affecting melee hit determination
	float a_str;	//strength
	float a_dex; //dexterity
	float a_luck; //character's luck
	float a_attack_skill; // character's attack skill
	float a_defense_skill; //character's defense skill
	float a_defense_range;
   float a_attack_range;
		
	//Character 'B' attributes affecting melee hit determination
	float b_str;
	float b_dex;
	float b_luck;
	float b_attack_skill;
	float b_defense_skill;
	float b_defense_range;
	float b_attack_range;
	
	// compiled total ranges
	float a_attack_total_allotment;
	float b_attack_total_allotment;
	float a_attack_luck_allotment;
	float b_attack_luck_allotment;
	float a_attack_a_real_luck;
	float a_attack_b_real_luck;
	float b_attack_a_real_luck;
	float b_attack_b_real_luck;
	//generated random number
	float dice;
	
	public:

  //generate 2 random character stats for testing
  void create_characters();

  //Calculate relevant stat ranges
  void calc_stats();

  	//*TEMPORARY, FOR TESTING PURPOSES
	void show_results(int);

	friend void show_results (int);
	friend float get_dice (int);
};

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
#endif
