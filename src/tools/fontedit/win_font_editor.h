#ifndef _WIN_FONT_EDIT_H_
#define _WIN_FONT_EDIT_H_
/*
#define WIN_FONT_EDITOR_MENU1_C1 "Create new font"
#define WIN_FONT_EDITOR_MENU1_C2 "Load font"
#define WIN_FONT_EDITOR_MENU1_C3 "Quit"
#define WIN_FONT_EDITOR_MENU2_C1 "Load PNM font"
#define WIN_FONT_EDITOR_MENU2_C2 "Load PNM Cursor"
#define WIN_FONT_EDITOR_MENU2_C3 "Set font"
#define WIN_FONT_EDITOR_MENU2_C4 "Save font"
#define WIN_FONT_EDITOR_MENU2_C5 "Back"


class win_container;
class win_file;
class win_select;
class win_image;


class win_font_editor : public win_container
{
 
 private:
  win_label * wlab_; //label
  win_file * wfile_; //win_file
  win_select * select_menu_; //list menu
  win_image * imgletter_; //image with cur letter
  
  win_font * font__; 
  
  image * new_font_; //new font image
  image * cursor_; //cursor image

  typedef struct Tinode_{
    char letter;
    u_int16 pos;
    u_int16 length;
  };

  Tinode_ letter_[255];//table with letter
  u_int8 nbletter_; //nb letter
  u_int16 curpos_; //position of the index in the image


  void update_font();
  void init();
  void set_menu1();
  void set_menu2();
  void set_edit_font_menu();
  void _on_menu1_c1_activate__();
  void _on_menu1_c2_activate__();
  void _on_menu1_c3_activate__();
  void _on_menu2_c1_activate__();
  void _on_menu2_c2_activate__();
  void _on_menu2_c3_activate__();
  void _on_menu2_c4_activate__();
  void _on_menu2_c5_activate__();
  void _on_activate__();
  void _on_draw__();
  void _on_update__(); 
  void _on_wf_bt_ok_load_pnm__();
  void _on_wf_bt_cancel_load_pnm__();
 public:

  bool update();
  win_font_editor(s_int16, s_int16,u_int16,u_int16,win_theme *,win_font*);
  // ~win_font_editor();
};
*/


class win_container;
class win_image;

class win_font_editor : public win_container
{
  typedef struct Tinode_{
    char letter;
    u_int16 pos;
    u_int16 length;
  };
  
  list<Tinode_*> l_;
  u_int16 lastpos_;
  u_int16 curpos_;
  
  image * newfont_;
  image * cursor_;
  image * tmp;

  char * destfile_;
  
  win_image * img;
  
  bool update_next_letter();

 public:
  win_font_editor(s_int16, s_int16,u_int16,u_int16,win_theme *,win_font*);
  void set_file(char * font, char * cursor, char * dest);
  bool update();
  void write();
};

#endif









