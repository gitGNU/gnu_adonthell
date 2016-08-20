/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TYPES_WIN_H_
#define TYPES_WIN_H_

#include <SDL_keycode.h>

#define WIN_NB_TABLE_CHAR 127
#define WIN_TEXT_MAX_LENGTH 5000
#define WIN_FONT_HEIGHT 8
#define WIN_FONT_LENGHT 8
#define WIN_SPACE_LENGHT 8

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
#define WIN_FONT_FILE "font.font"

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


//cursor filename
#define WIN_CURSOR_FILE "cursor.pnm"

//directory size
#define WIN_BORDER_NORMAL_SIZE "normal/"
#define WIN_BORDER_MINI_SIZE "mini/"
 
#define WIN_BRIGHTNESS_LEVEL 180

#endif
