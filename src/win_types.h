#ifndef TYPES_WIN_H_
#define TYPES_WIN_H_

#define NB_TABLE_CHAR 255
#define TEXT_MAX 500
#define WIN_FONT_HEIGHT 8
#define WIN_FONT_LENGHT 8
#define WIN_SPACE_LENGHT 8



//directory
#define WIN_DIRECTORY "gfxtree/window/"
#define WIN_FONT_DIRECTORY "font/"
#define WIN_BORDER_DIRECTORY "border/"
#define WIN_BACKGROUND_DIRECTORY "background/"

//font filename
#define WIN_FONT_FILE_IDX "font.idx"
#define WIN_FONT_FILE_PIC "font.pnm"

//border filename
#define WIN_V_BORDER_TEMPLATE_FILE "v_border.pnm"
#define WIN_H_BORDER_TEMPLATE_FILE "h_border.pnm"
#define WIN_CORNER_FILE "corner.pnm"
#define WIN_BACKGROUND_FILE "background.pnm"

//just a start to build a theme
#define WIN_THEME_ORIGINAL "original/" //dont forget '/'


typedef struct 
{
  char text[TEXT_MAX];
  u_int16 lenght;
  u_int16 pos;
  u_int16 pos_tmp;
  u_int16 len_fl;
  bool end_win; //if text reach the end of window
}text_w;


#endif










