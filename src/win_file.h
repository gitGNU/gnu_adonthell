#ifndef _WIN_FILE_H_
#define _WIN_FILE_H_

#define WIN_FILE_SPACE_BETWEEN_OBJECT 10
#define WIN_FILE_COMBO_BUTTON_RATIO 2/3
#define WIN_FILE_DEFAULT_PATH "/home/jol"
#define WIN_FILE_BUTTON_OK_TEXT "Ok"
#define WIN_FILE_BUTTON_CANCEL_TEXT "Cancel"
#define WIN_FILE_TITLE_TEXT "Select Your File Or Your Directory"
#define WIN_FILE_MAX_PATH 255
#define WIN_FILE_DIR_NAME "<D>"
#define WIN_FILE_FILE_NAME "<F>"

class win_container;
class win_label;
class win_write;
class win_select;

class win_file
{
  win_container * wc_master; //father container
  win_container * window;
  win_container * combo; 
  
  win_label * button_ok;
  win_label * button_cancel;
  win_label * title;
  
  win_write * entry;
  
  win_border * bord_norm;
  win_border * bord_mini;
  
  win_background * background;
  
  win_font * font;
  
  
  char cur_dir[WIN_FILE_MAX_PATH];

 public:
  win_file(s_int16 x,s_int16 y,u_int16 l,u_int16 h,char * theme,win_container *);
  ~win_file();
  void move(s_int16 x,s_int16 y);
  void resize(u_int16 l,u_int16);
  void show();
  void hide();
  void set_theme(char *);
  void list_dir();
};
#endif
