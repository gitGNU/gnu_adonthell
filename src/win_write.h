#ifndef WIN_WRITE_H_
#define WIN_WRITE_H_


class win_base;
class win_font;
class win_container;
class drawing_area;

class win_write : public win_base
{
  win_font * font;
  
  void write();
  
 protected:
  
  char text_result[TEXT_MAX];
  
  text_w text;
  int beg_select;
  int end_select;
  bool ok_text;
  
 
 public:
  win_write(u_int16,u_int16,u_int16,u_int16,win_font *fo,win_container *);
  void set_font(win_font *fo);
  bool is_text();//return true if seltext/lenght!=0
  char * get_text();//return seltext if is_text
  void draw();
  void update();
};

#endif












