/*
   $Id$

   Copyright (C) 2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   input_manager_SDL.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com
 * 
 * @brief  Declares the input_manager class.
 *         This file is dependant of SDL.
 * 
 * 
 */


#include "input_manager.h"
#include <vector>
#include "SDL.h"

list <input_listener *> input_manager::listeners;

/**
 * Translation table for SDL keys symbols.
 * 
 */
static keyboard_event::key_type sdl_key_trans[SDLK_LAST] = 
{
    /* 0 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 5 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::BACKSPACE_KEY,
    keyboard_event::TAB_KEY,
    /* 10 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::CLEAR_KEY,
    keyboard_event::RETURN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 15 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::PAUSE_KEY,
    /* 20 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 25 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::ESCAPE_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 30 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::SPACE_KEY,
    keyboard_event::EXCLAIM_KEY,
    keyboard_event::DBLQUOTE_KEY,
    /* 35 */
    keyboard_event::HASH_KEY,
    keyboard_event::DOLLAR_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::AMPERSAND_KEY,
    keyboard_event::QUOTE_KEY,
    /* 40 */
    keyboard_event::LEFTPAREN_KEY,
    keyboard_event::RIGHTPAREN_KEY,
    keyboard_event::ASTERISK_KEY,
    keyboard_event::PLUS_KEY,
    keyboard_event::COMA_KEY,
    /* 45 */
    keyboard_event::MINUS_KEY,
    keyboard_event::PERIOD_KEY,
    keyboard_event::SLASH_KEY,
    keyboard_event::N0_KEY,
    keyboard_event::N1_KEY,
    /* 50 */
    keyboard_event::N2_KEY,
    keyboard_event::N3_KEY,
    keyboard_event::N4_KEY,
    keyboard_event::N5_KEY,
    keyboard_event::N6_KEY,
    /* 55 */
    keyboard_event::N7_KEY,
    keyboard_event::N8_KEY,
    keyboard_event::N9_KEY,
    keyboard_event::COLON_KEY,
    keyboard_event::SEMICOLON_KEY,
    /* 60 */
    keyboard_event::LESS_KEY,
    keyboard_event::EQUALS_KEY,
    keyboard_event::GREATER_KEY,
    keyboard_event::QUESTION_KEY,
    keyboard_event::AT_KEY,
    /* 65 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 70 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 75 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 80 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 85 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 90 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::LEFTBRACKET_KEY,
    keyboard_event::BACKSLASH_KEY,
    keyboard_event::RIGHTBRACKET_KEY,
    keyboard_event::CARET_KEY,
    /* 95 */
    keyboard_event::UNDERSCORE_KEY,
    keyboard_event::BACKQUOTE_KEY,
    keyboard_event::A_KEY,
    keyboard_event::B_KEY,
    keyboard_event::C_KEY,
    /* 100 */
    keyboard_event::D_KEY,
    keyboard_event::E_KEY,
    keyboard_event::F_KEY,
    keyboard_event::G_KEY,
    keyboard_event::H_KEY,
    /* 105 */
    keyboard_event::I_KEY,
    keyboard_event::J_KEY,
    keyboard_event::K_KEY,
    keyboard_event::L_KEY,
    keyboard_event::M_KEY,
    /* 110 */
    keyboard_event::N_KEY,
    keyboard_event::O_KEY,
    keyboard_event::P_KEY,
    keyboard_event::Q_KEY,
    keyboard_event::R_KEY,
    /* 115 */
    keyboard_event::S_KEY,
    keyboard_event::T_KEY,
    keyboard_event::U_KEY,
    keyboard_event::V_KEY,
    keyboard_event::W_KEY,
    /* 120 */
    keyboard_event::X_KEY,
    keyboard_event::Y_KEY,
    keyboard_event::Z_KEY,
    keyboard_event::DELETE_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 125 */
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 160 */
    keyboard_event::WORLD_0_KEY,
    keyboard_event::WORLD_1_KEY,
    keyboard_event::WORLD_2_KEY,
    keyboard_event::WORLD_3_KEY,
    keyboard_event::WORLD_4_KEY,
    /* 165 */
    keyboard_event::WORLD_5_KEY,
    keyboard_event::WORLD_6_KEY,
    keyboard_event::WORLD_7_KEY,
    keyboard_event::WORLD_8_KEY,
    keyboard_event::WORLD_9_KEY,
    /* 170 */
    keyboard_event::WORLD_10_KEY,
    keyboard_event::WORLD_11_KEY,
    keyboard_event::WORLD_12_KEY,
    keyboard_event::WORLD_13_KEY,
    keyboard_event::WORLD_14_KEY,
    /* 175 */
    keyboard_event::WORLD_15_KEY,
    keyboard_event::WORLD_16_KEY,
    keyboard_event::WORLD_17_KEY,
    keyboard_event::WORLD_18_KEY,
    keyboard_event::WORLD_19_KEY,
    /* 180 */
    keyboard_event::WORLD_20_KEY,
    keyboard_event::WORLD_21_KEY,
    keyboard_event::WORLD_22_KEY,
    keyboard_event::WORLD_23_KEY,
    keyboard_event::WORLD_24_KEY,
    /* 185 */
    keyboard_event::WORLD_25_KEY,
    keyboard_event::WORLD_26_KEY,
    keyboard_event::WORLD_27_KEY,
    keyboard_event::WORLD_28_KEY,
    keyboard_event::WORLD_29_KEY,
    /* 190 */
    keyboard_event::WORLD_30_KEY,
    keyboard_event::WORLD_31_KEY,
    keyboard_event::WORLD_32_KEY,
    keyboard_event::WORLD_33_KEY,
    keyboard_event::WORLD_34_KEY,
    /* 195 */
    keyboard_event::WORLD_35_KEY,
    keyboard_event::WORLD_36_KEY,
    keyboard_event::WORLD_37_KEY,
    keyboard_event::WORLD_38_KEY,
    keyboard_event::WORLD_39_KEY,
    /* 200 */
    keyboard_event::WORLD_40_KEY,
    keyboard_event::WORLD_41_KEY,
    keyboard_event::WORLD_42_KEY,
    keyboard_event::WORLD_43_KEY,
    keyboard_event::WORLD_44_KEY,
    /* 205 */
    keyboard_event::WORLD_45_KEY,
    keyboard_event::WORLD_46_KEY,
    keyboard_event::WORLD_47_KEY,
    keyboard_event::WORLD_48_KEY,
    keyboard_event::WORLD_49_KEY,
    /* 210 */
    keyboard_event::WORLD_50_KEY,
    keyboard_event::WORLD_51_KEY,
    keyboard_event::WORLD_52_KEY,
    keyboard_event::WORLD_53_KEY,
    keyboard_event::WORLD_54_KEY,
    /* 215 */
    keyboard_event::WORLD_55_KEY,
    keyboard_event::WORLD_56_KEY,
    keyboard_event::WORLD_57_KEY,
    keyboard_event::WORLD_58_KEY,
    keyboard_event::WORLD_59_KEY,
    /* 220 */
    keyboard_event::WORLD_60_KEY,
    keyboard_event::WORLD_61_KEY,
    keyboard_event::WORLD_62_KEY,
    keyboard_event::WORLD_63_KEY,
    keyboard_event::WORLD_64_KEY,
    /* 225 */
    keyboard_event::WORLD_65_KEY,
    keyboard_event::WORLD_66_KEY,
    keyboard_event::WORLD_67_KEY,
    keyboard_event::WORLD_68_KEY,
    keyboard_event::WORLD_69_KEY,
    /* 230 */
    keyboard_event::WORLD_70_KEY,
    keyboard_event::WORLD_71_KEY,
    keyboard_event::WORLD_72_KEY,
    keyboard_event::WORLD_73_KEY,
    keyboard_event::WORLD_74_KEY,
    /* 235 */
    keyboard_event::WORLD_75_KEY,
    keyboard_event::WORLD_76_KEY,
    keyboard_event::WORLD_77_KEY,
    keyboard_event::WORLD_78_KEY,
    keyboard_event::WORLD_79_KEY,
    /* 240 */
    keyboard_event::WORLD_80_KEY,
    keyboard_event::WORLD_81_KEY,
    keyboard_event::WORLD_82_KEY,
    keyboard_event::WORLD_83_KEY,
    keyboard_event::WORLD_84_KEY,
    /* 245 */
    keyboard_event::WORLD_85_KEY,
    keyboard_event::WORLD_86_KEY,
    keyboard_event::WORLD_87_KEY,
    keyboard_event::WORLD_88_KEY,
    keyboard_event::WORLD_89_KEY,
    /* 250 */
    keyboard_event::WORLD_90_KEY,
    keyboard_event::WORLD_91_KEY,
    keyboard_event::WORLD_92_KEY,
    keyboard_event::WORLD_93_KEY,
    keyboard_event::WORLD_94_KEY,
    /* 255 */
    keyboard_event::WORLD_95_KEY,
    keyboard_event::KP0_KEY,
    keyboard_event::KP1_KEY,
    keyboard_event::KP2_KEY,
    keyboard_event::KP3_KEY,
    /* 260 */
    keyboard_event::KP4_KEY,
    keyboard_event::KP5_KEY,
    keyboard_event::KP6_KEY,
    keyboard_event::KP7_KEY,
    keyboard_event::KP8_KEY,
    /* 265 */
    keyboard_event::KP9_KEY,
    keyboard_event::KP_PERIOD_KEY,
    keyboard_event::KP_DIVIDE_KEY,
    keyboard_event::KP_MULTIPLY_KEY,
    keyboard_event::KP_MINUS_KEY,
    /* 270 */
    keyboard_event::KP_PLUS_KEY,
    keyboard_event::KP_ENTER_KEY,
    keyboard_event::KP_EQUALS_KEY,
    keyboard_event::UP_KEY,
    keyboard_event::DOWN_KEY,
    /* 275 */
    keyboard_event::RIGHT_KEY,
    keyboard_event::LEFT_KEY,
    keyboard_event::INSERT_KEY,
    keyboard_event::HOME_KEY,
    keyboard_event::END_KEY,
    /* 280 */
    keyboard_event::PAGEUP_KEY,
    keyboard_event::PAGEDOWN_KEY,
    keyboard_event::F1_KEY,
    keyboard_event::F2_KEY,
    keyboard_event::F3_KEY,
    /* 285 */
    keyboard_event::F4_KEY,
    keyboard_event::F5_KEY,
    keyboard_event::F6_KEY,
    keyboard_event::F7_KEY,
    keyboard_event::F8_KEY,
    /* 290 */
    keyboard_event::F9_KEY,
    keyboard_event::F10_KEY,
    keyboard_event::F11_KEY,
    keyboard_event::F12_KEY,
    keyboard_event::F13_KEY,
    /* 295 */
    keyboard_event::F14_KEY,
    keyboard_event::F15_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    keyboard_event::UNKNOWN_KEY,
    /* 300 */
    keyboard_event::NUMLOCK_KEY,
    keyboard_event::CAPSLOCK_KEY,
    keyboard_event::SCROLLLOCK_KEY,
    keyboard_event::RSHIFT_KEY,
    keyboard_event::LSHIFT_KEY,
    /* 305 */
    keyboard_event::RCTRL_KEY,
    keyboard_event::LCTRL_KEY,
    keyboard_event::RALT_KEY,
    keyboard_event::LALT_KEY,
    keyboard_event::RMETA_KEY,
    /* 310 */
    keyboard_event::LMETA_KEY,
    keyboard_event::LSUPER_KEY,
    keyboard_event::RSUPER_KEY,
    keyboard_event::MODE_KEY,
    keyboard_event::COMPOSE_KEY,
    /* 315 */
    keyboard_event::HELP_KEY,
    keyboard_event::PRINT_KEY,
    keyboard_event::SYSREQ_KEY,
    keyboard_event::BREAK_KEY,
    keyboard_event::MENU_KEY,
    /* 320 */
    keyboard_event::POWER_KEY,
    keyboard_event::EURO_KEY
};

/**
 * Translation table for SDL buttons symbols.
 * 
 */
static mouse_event::button_type sdl_button_trans[SDL_BUTTON_RIGHT + 3] = 
{
    mouse_event::NO_BUTTON,
    mouse_event::LEFT_BUTTON,
    mouse_event::MIDDLE_BUTTON,
    mouse_event::RIGHT_BUTTON,
    mouse_event::WHEEL_UP,
    mouse_event::WHEEL_DOWN
};

/**
 * Translation table for SDL joystick axis symbols.
 * 
 */
static joystick_event::button_type sdl_axis_trans[joystick_event::NBR_BUTTONS - 
                                                  joystick_event::AXIS0_FORE] =
{
    joystick_event::AXIS0_FORE, 
    joystick_event::AXIS0_BACK,
    joystick_event::AXIS1_FORE,
    joystick_event::AXIS1_BACK,
    joystick_event::AXIS2_FORE,
    joystick_event::AXIS2_BACK,
    joystick_event::AXIS3_FORE,
    joystick_event::AXIS3_BACK,
    joystick_event::AXIS4_FORE,
    joystick_event::AXIS4_BACK,
    joystick_event::AXIS5_FORE, 
    joystick_event::AXIS5_BACK,
    joystick_event::AXIS6_FORE,
    joystick_event::AXIS6_BACK,
    joystick_event::AXIS7_FORE,
    joystick_event::AXIS7_BACK,
    joystick_event::AXIS8_FORE,
    joystick_event::AXIS8_BACK,
    joystick_event::AXIS9_FORE,
    joystick_event::AXIS9_BACK
};

/**
 * Translation table for SDL joystick buttons symbols.
 * 
 */
static joystick_event::button_type sdl_joy_button_trans[joystick_event::AXIS0_FORE] =
{
    joystick_event::BUTTON_0,
    joystick_event::BUTTON_1,
    joystick_event::BUTTON_2,
    joystick_event::BUTTON_3,
    joystick_event::BUTTON_4,
    joystick_event::BUTTON_5,
    joystick_event::BUTTON_6,
    joystick_event::BUTTON_7,
    joystick_event::BUTTON_8,
    joystick_event::BUTTON_9
};

/**
 * Logical state of joysticks.
 * 
 */
static bool joystick_state_table[joystick_event::NBR_BUTTONS * 10] = { false };

/**
 * List of used SDL joysticks.
 * 
 */
static std::vector<SDL_Joystick *> joysticks;

void input_manager::init()
{
    SDL_Init(SDL_INIT_JOYSTICK);
    int nbr_joy = SDL_NumJoysticks();
    for (int i = 0; i < nbr_joy; i++)
        joysticks.push_back(SDL_JoystickOpen(i));
}

void input_manager::cleanup()
{
    for (std::vector<SDL_Joystick *>::iterator i = joysticks.begin(); 
         i != joysticks.end(); i++)
        SDL_JoystickClose(*i);
    
}

void input_manager::update()
{
    static SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
            {
                keyboard_event ke (keyboard_event::KEY_PUSHED, sdl_key_trans[event.key.keysym.sym]);
                raise_event (ke);

                if (control_event::keyboard_mapping(ke.key()) != control_event::NO_BUTTON)
                {
                    control_event ce(control_event::BUTTON_PUSHED, control_event::keyboard_mapping(ke.key()));
                    raise_event(ce);
                }

                break;
            }
            case SDL_KEYUP:
            {
                keyboard_event ke (keyboard_event::KEY_RELEASED, sdl_key_trans[event.key.keysym.sym]);
                raise_event (ke);

                if (control_event::keyboard_mapping(ke.key()) != control_event::NO_BUTTON)
                {
                    control_event ce(control_event::BUTTON_RELEASED, control_event::keyboard_mapping(ke.key()));
                    raise_event(ce);
                }

                break;
            }
            case SDL_MOUSEMOTION:
            {
                mouse_event me (mouse_event::MOUSE_MOTION, mouse_event::NO_BUTTON, 
                                event.motion.x, event.motion.y);
                raise_event(me);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                mouse_event me(mouse_event::BUTTON_PUSHED, sdl_button_trans[event.button.button],
                               event.button.x, event.button.y);
                raise_event(me);

                if (control_event::mouse_mapping(me.button()) != control_event::NO_BUTTON)
                {
                    control_event ce(control_event::BUTTON_PUSHED, control_event::mouse_mapping(me.button()));
                    raise_event(ce);
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                mouse_event me(mouse_event::BUTTON_RELEASED, sdl_button_trans[event.button.button],
                               event.button.x, event.button.y);
                raise_event(me);

                if (control_event::mouse_mapping(me.button()) != control_event::NO_BUTTON)
                {
                    control_event ce(control_event::BUTTON_RELEASED, control_event::mouse_mapping(me.button()));
                    raise_event(ce);
                }
                break;
            }
            case SDL_JOYAXISMOTION:
            {
                int value = event.jaxis.value > 0 ? event.jaxis.value : -event.jaxis.value;                
                int pushed = (value > 10000) ? 1 : 0;
                int joynbr = event.jaxis.which;
                int axisnbr = (event.jaxis.axis << 1) + (event.jaxis.value > 0);

                if (event.jaxis.axis >= joystick_event::NBR_BUTTONS - joystick_event::AXIS0_FORE) break;

                if (!pushed && !joystick_state_table[joynbr * joystick_event::NBR_BUTTONS
                                                     + axisnbr]) break;

                if (pushed && joystick_state_table[joynbr * joystick_event::NBR_BUTTONS
                                                     + axisnbr]) break;

                joystick_event je(event.jaxis.which, pushed ?
                                  joystick_event::BUTTON_PUSHED : 
                                  joystick_event::BUTTON_RELEASED, 
                                  sdl_axis_trans[axisnbr]);

                joystick_state_table[joynbr * joystick_event::NBR_BUTTONS + axisnbr] = pushed;

                raise_event(je);

                if (control_event::joystick_mapping(je.button()) != control_event::NO_BUTTON)
                {
                    control_event ce(je.type() == joystick_event::BUTTON_PUSHED ? 
                                     control_event::BUTTON_PUSHED : 
                                     control_event::BUTTON_RELEASED,
                                     control_event::joystick_mapping(je.button()));
                    raise_event(ce);
                }
                
                break;
            }
            case SDL_JOYBUTTONDOWN:
            {
                if (event.jbutton.button >= joystick_event::AXIS0_FORE) break;

                joystick_event je(event.jbutton.which, joystick_event::BUTTON_PUSHED,
                                  sdl_joy_button_trans[event.jbutton.button]);
                raise_event(je);
                
                if (control_event::joystick_mapping(je.button()) != control_event::NO_BUTTON)
                {
                    control_event ce(control_event::BUTTON_PUSHED, control_event::joystick_mapping(je.button()));
                    raise_event(ce);
                }
                break;
            }
            case SDL_JOYBUTTONUP:
            {
                if (event.jbutton.button >= joystick_event::AXIS0_FORE) break;

                joystick_event je(event.jbutton.which, joystick_event::BUTTON_RELEASED,
                                  sdl_joy_button_trans[event.jbutton.button]);
                raise_event(je);
                
                if (control_event::joystick_mapping(je.button()) != control_event::NO_BUTTON)
                {
                    control_event ce(control_event::BUTTON_RELEASED, control_event::joystick_mapping(je.button()));
                    raise_event(ce);
                }
                break;
            }
        }
    }
}
