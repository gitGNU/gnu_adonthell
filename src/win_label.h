#ifndef WIN_LABEL_H_
#define WIN_LABEL_H_

class win_base;
class win_font;

class win_label : public win_base
{
  win_font * font;
  bool ch_page;
  
 protected:
  text_w text;
  
 public:
  win_label(u_int16,u_int16,u_int16,u_int16,win_font * fo,win_container *);
  ~win_label();
  void set_font(win_font * fo);//change font
  void draw();//draw text
  void set_text(char *t);//set text
  void erase_text(); //erase text
  void update();
  void next_page(); //change page
  bool end_text(); //return true if you are at the end of the text
};

#endif


