/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef TYPES_WIN_H_
#define TYPES_WIN_H_

#include <SDL/SDL_keysym.h>

#define WIN_NB_TABLE_CHAR 255
#define WIN_TEXT_MAX_LENGTH 5000
#define WIN_FONT_HEIGHT 8
#define WIN_FONT_LENGHT 8
#define WIN_SPACE_LENGHT 8

//justify
#define WIN_JUSTIFY_LEFT 0
#define WIN_JUSTIFY_RIGHT 1
#define WIN_JUSTIFY_CENTER 2
#define WIN_JUSTIFY_NO 3

//layout
#define WIN_LAYOUT_NO 0
#define WIN_LAYOUT_LIST 1


//directory
#define WIN_DIRECTORY "gfx/window/"
#define WIN_FONT_DIRECTORY "font/"
#define WIN_BORDER_DIRECTORY "border/"
#define WIN_BACKGROUND_DIRECTORY "background/"
#define WIN_SCROLLBAR_DIRECTORY "scrollbar/"
#define WIN_CURSOR_DIRECTORY "cursor/"

//font filename
#define WIN_FONT_FILE_IDX "font.idx"
#define WIN_FONT_FILE_PIC "font.pnm"


//CURSOR LABEL
#define WIN_CURSOR_BLINKING 75


//border filename
#define WIN_V_BORDER_TEMPLATE_FILE "v_border.pnm"
#define WIN_H_BORDER_TEMPLATE_FILE "h_border.pnm"
#define WIN_CORNER_TOP_LEFT_FILE "corner_top_left.pnm"
#define WIN_CORNER_TOP_RIGHT_FILE "corner_top_right.pnm"
#define WIN_CORNER_BOTTOM_LEFT_FILE "corner_bottom_left.pnm"
#define WIN_CORNER_BOTTOM_RIGHT_FILE "corner_bottom_right.pnm"


//background filename
#define WIN_BACKGROUND_FILE "background.pnm"

//scrollbar filename
#define WIN_SCROLLBAR_BAR_TOP "scroll_bar_top.pnm"
#define WIN_SCROLLBAR_BAR_MID "scroll_bar_mid.pnm"
#define WIN_SCROLLBAR_BAR_BOT "scroll_bar_bot.pnm"
#define WIN_SCROLLBAR_BAR_FLEX "scroll_bar_flex.pnm"
#define WIN_SCROLLBAR_BACK_TOP "scroll_back_top.pnm"
#define WIN_SCROLLBAR_BACK_MID "scroll_back_mid.pnm"
#define WIN_SCROLLBAR_BACK_BOT "scroll_back_bot.pnm"

//default key For the selection
#define WIN_SELECT_DEFAULT_KEY_NEXT SDLK_DOWN
#define WIN_SELECT_DEFAULT_KEY_PREVIOUS SDLK_UP
#define WIN_SELECT_DEFAULT_KEY_ACTIVATE SDLK_SPACE
#define WIN_SELECT_DEFAULT_KEY_BACK SDLK_b

//cursor filename
#define WIN_CURSOR_FILE "cursor.pnm"

//directory size
#define WIN_BORDER_NORMAL_SIZE "normal/"
#define WIN_BORDER_MINI_SIZE "mini/"

//just a start to build a theme
#define WIN_THEME_ORIGINAL "original/" //dont forget '/'
#define WIN_THEME_ELFE "silverleaf/"

//win_OBJECT
#define WIN_OBJ_BASE 0
#define WIN_OBJ_LABEL 1
#define WIN_OBJ_WRITE 2
#define WIN_OBJ_IMAGE 3
#define WIN_OBJ_CONTAINER 4
#define WIN_OBJ_SCROLLED 5
#define WIN_OBJ_SELECT 6

//win_signal
#define WIN_SIG_ACTIVATE 0
#define WIN_SIG_UNACTIVATE 1
#define WIN_SIG_UPDATE 2
#define WIN_SIG_DRAW 3
//this signal is call only is objet is visible
#define WIN_SIG_DRAW_ONLY_VISIBLE 11 
#define WIN_SIG_ACTIVATE_KEY 4
#define WIN_SIG_NEXT_KEY 5
#define WIN_SIG_PREVIOUS_KEY 6
#define WIN_SIG_SCROLL_UP 7
#define WIN_SIG_SCROLL_DOWN 8
#define WIN_SIG_SELECT 9
#define WIN_SIG_UNSELECT 10


//win_select
#define WIN_SELECT_MODE_BRIGHTNESS 0
#define WIN_SELECT_MODE_BORDER 1
#define WIN_SELECT_MODE_CURSOR 2 

#define WIN_SIZE_NORMAL 0
#define WIN_SIZE_MINI 1

//WIN SCROLL BAR
#define WIN_SCROLLBAR_PAD_DEFAULT 10;
#define WIN_SPACE_BETWEEN_BORDER 10;
#define WIN_SPACE_BETWEEN_OBJECT 5;

#endif













