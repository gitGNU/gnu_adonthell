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

#define WIN_NB_TABLE_CHAR 255
#define WIN_TEXT_MAX_LENGTH 5000
#define WIN_FONT_HEIGHT 8
#define WIN_FONT_LENGHT 8
#define WIN_SPACE_LENGHT 8



//directory
#define WIN_DIRECTORY "gfxtree/window/"
#define WIN_FONT_DIRECTORY "font/"
#define WIN_BORDER_DIRECTORY "border/"
#define WIN_BACKGROUND_DIRECTORY "background/"
#define WIN_SCROLLBAR_DIRECTORY "scrollbar/"
#define WIN_CURSOR_DIRECTORY "cursor/"

//font filename
#define WIN_FONT_FILE_IDX "font.idx"
#define WIN_FONT_FILE_PIC "font.pnm"

//border filename
#define WIN_V_BORDER_TEMPLATE_FILE "v_border.pnm"
#define WIN_H_BORDER_TEMPLATE_FILE "h_border.pnm"
#define WIN_CORNER_FILE "corner.pnm"

//background filename
#define WIN_BACKGROUND_FILE "background.pnm"

//scrollbar filename
#define WIN_SCROLLBAR_BAR_FILE "scroll_bar.pnm"
#define WIN_SCROLLBAR_MIDDLE_FILE "scroll_mid.pnm"
#define WIN_SCROLLBAR_BOTTOM_FILE "scroll_bot.pnm"
#define WIN_SCROLLBAR_TOP_FILE "scroll_top.pnm"



//cursor filename
#define WIN_CURSOR_FILE "cursor.pnm"

//directory size
#define WIN_BORDER_NORMAL_SIZE "normal/"
#define WIN_BORDER_MINI_SIZE "mini/"

//just a start to build a theme
#define WIN_THEME_ORIGINAL "original/" //dont forget '/'

//win_OBJECT
#define WIN_OBJ_LABEL 0
#define WIN_OBJ_WRITE 1
#define WIN_OBJ_IMAGE 2
#define WIN_OBJ_CONTAINER 3

//win_select
#define WIN_SELECT_MODE_BRIGHTNESS 0
#define WIN_SELECT_MODE_BORDER 1
#define WIN_SELECT_MODE_CURSOR 2 //used with label!!!! 

//WIN SCROLL BAR
#define WIN_SCROLLBAR_PAD_DEFAULT 10;


typedef struct 
{
  char text[WIN_TEXT_MAX_LENGTH];
  u_int16 lenght;
  u_int16 pos;
  u_int16 pos_tmp;
  u_int16 len_fl;
  bool end_win; //if text reach the end of window
}text_w;


#endif










