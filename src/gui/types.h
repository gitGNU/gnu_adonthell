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

#ifndef GUI_TYPES_H_
#define GUI_TYPES_H_

#include <SDL/SDL_keysym.h>

#define NB_TABLE_CHAR 127
#define TEXT_MAX_LENGTH 5000
#define FONT_HEIGHT 8
#define FONT_LENGHT 8
#define SPACE_LENGHT 8

//directory
#define DIRECTORY "gfx/window/"
#define FONT_DIRECTORY "font/"
#define BORDER_DIRECTORY "border/"
#define BACKGROUND_DIRECTORY "background/"
#define SCROLLBAR_DIRECTORY "scrollbar/"
#define CURSOR_DIRECTORY "cursor/"

//font filename
#define FONT_FILE_IDX "font.idx"
#define FONT_FILE_PIC "font.pnm"
#define FONT_FILE "font.font"

//border filename
#define V_BORDER_TEMPLATE_FILE "v_border.pnm"
#define H_BORDER_TEMPLATE_FILE "h_border.pnm"
#define CORNER_TOP_LEFT_FILE "corner_top_left.pnm"
#define CORNER_TOP_RIGHT_FILE "corner_top_right.pnm"
#define CORNER_BOTTOM_LEFT_FILE "corner_bottom_left.pnm"
#define CORNER_BOTTOM_RIGHT_FILE "corner_bottom_right.pnm"

//background filename
#define BACKGROUND_FILE "background.pnm"

//scrollbar filename
#define SCROLLBAR_BAR_TOP "scroll_bar_top.pnm"
#define SCROLLBAR_BAR_MID "scroll_bar_mid.pnm"
#define SCROLLBAR_BAR_BOT "scroll_bar_bot.pnm"
#define SCROLLBAR_BAR_FLEX "scroll_bar_flex.pnm"
#define SCROLLBAR_BACK_TOP "scroll_back_top.pnm"
#define SCROLLBAR_BACK_MID "scroll_back_mid.pnm"
#define SCROLLBAR_BACK_BOT "scroll_back_bot.pnm"


//cursor filename
#define CURSOR_FILE "cursor.pnm"

//directory size
#define BORDER_NORMAL_SIZE "normal/"
#define BORDER_MINI_SIZE "mini/"
 
#define BRIGHTNESS_LEVEL 130

#endif
