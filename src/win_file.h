#ifndef _WIN_FILE_H_
#define _WIN_FILE_H_

class win_select;
class win_font;
class win_label;
class win_write;

class win_file: public win_container
{
 protected:
  win_label * title_;
  win_label * button_ok_;
  win_label * button_cancel_;
  win_write * entry_;
  win_select * combo_file_;
  win_select * combo_;
  win_font * font_;

  char cur_dir_[500];
  char cur_file_[100];
  void list_directory();
  //callback
  void _on_activate__();
  void _on_activate_combo_file__();
  void _on_activate_key_entry__();

 public:
  win_file(s_int16 x,s_int16 y,u_int16 l,u_int16 h,win_theme * theme,win_font * wf);
  void resize(u_int16,u_int16);
  void set_title(char *);
  void set_curdirectory(char *);
  //choice what you want to do when these button are activated CALLBACK POWER
  void set_sig_activate_button_ok(Functor0 & func);
  void set_sig_activate_button_cancel(Functor0 & func);
  ~win_file();  
};
#endif




