/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


class keyboard
{
 public:
  static bool charkeys[65536];
  static u_int16 last_key;
  static void update_keyboard();
  static u_int16 getkeypressed();
  static bool is_pushed(u_int16 key);
};

int keyboard_init(void *data);

#define Left_Key 57396
#define Right_Key 57397
#define Up_Key 57394
#define Down_Key 57395
#define Shift_Key 58112
#define Ctrl_Key 58113
#define LeftAlt_Key 58114
#define RightAlt_Key 57387
#define Escape_Key 27
#define Enter_Key 13
#define Space_Key 32
#define Backspace_Key 8
#define KPPlus_Key 43
#define KPMinus_Key 45
#define Inser_Key 57378
#define Home_Key 57400
#define PgUp_Key 57398
#define Del_Key 127
#define End_Key 57401
#define PgDown_Key 57399
#define F1_Key 57601
#define F2_Key 57602
#define F3_Key 57603
#define F4_Key 57604
#define F5_Key 57605
#define F6_Key 57606
#define F7_Key 57607
#define F8_Key 57608
#define F9_Key 57609
#define F10_Key 57610
#define F11_Key 57611
#define F12_Key 57612
#define N0_Key 48
#define N1_Key 49
#define N2_Key 50
#define N3_Key 51
#define N4_Key 52
#define N5_Key 53
#define N6_Key 54
#define N7_Key 55
#define N8_Key 56
#define N9_Key 57
#define a_Key 65
#define b_Key 66
#define c_Key 67
#define A_Key 97
#define B_Key 98
#define C_Key 99
#define D_Key 100
#define E_Key 101
#define F_Key 102
#define G_Key 103
#define H_Key 104
#define I_Key 105
#define J_Key 106
#define K_Key 107
#define L_Key 108
#define M_Key 109
#define N_Key 110
#define O_Key 111
#define P_Key 112
#define Q_Key 113
#define R_Key 114
#define S_Key 115
#define T_Key 116
#define U_Key 117
#define V_Key 118
#define W_Key 119
#define X_Key 120
#define Y_Key 121
#define Z_Key 122

#define CtrlS_Combo 19
#define CtrlL_Combo 12
#define CtrlC_Combo 3
#define CtrlV_Combo 22

#endif
