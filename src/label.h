#ifndef _LABEL_H_
#define _LABEL_H_

#include <string>
#include "input.h"
#include "win_font.h"
#include "image.h"

class label: public image
{
 public:
  label();
  
  const string & text_string(){return text_;}
  const char * text_char(){return text_.c_str();} 

  void set_text(const string & str);
  
  void set_text(const char * str);
  
  void add_text(const string & str);
  
  void add_text(const char * str);
  
  void set_font(win_font & font);
  
  void set_form(const u_int8);
  
  void set_eot(const bool b);
  
  void set_cursor(const bool b){cursor_ = b;}
  
  void set_cursor_moveable(const bool b){cursor_moveable_ = b;}

  void resize(const u_int16 l,const u_int16 h);
 
  bool update();

  bool input_update();

  const static u_int8 NOTHING = 0;
  const static u_int8 AUTO_HEIGHT = 1;
  const static u_int8 AUTO_SIZE = 2;
  
  const static SDLKey KEY_CURSOR_NEXT = SDLK_RIGHT;
  const static SDLKey KEY_CURSOR_PREVIOUS = SDLK_LEFT;

 protected:
  
  void check_form();
  
  void form_auto_size();
  
  void form_auto_height();
  
  void form_nothing();
  
  void cursor_next();
  
  void cursor_previous();
  
  u_int8 word_place( u_int16 & cur_line_size, u_int16 & word_size );
  
  void find_word(u_int16 & begin,u_int16 &length, u_int16 & size_pix);
  
  //the text
  string text_;
  
  //the size of text
  u_int16 text_size_;
  
  //start to begin at the index
  u_int16 text_index_begin_;
  
  //it's the last letter which was drawing
  u_int16 text_index_end_;
  
  u_int16 cursor_pos_;
  
  bool cursor_;
  
  //set cursor visible
  bool cursor_visible_;
  
  //set cursor moveable
  bool cursor_moveable_;

  static u_int16 cursor_blink_speed_;
  
  u_int16 cursor_blink_cur_;

  bool text_eot_;

  //the font
  win_font * font_;
  
  //the style text displayed
  u_int8 form_;

 private: 
};

#endif





