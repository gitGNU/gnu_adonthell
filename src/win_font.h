#ifndef _WIN_FONT_H_
#define _WIN_FONT_H_

class image;

class win_font{

 private:
  void erase();


 public:
  
  image * table;
  u_int8 height;
  u_int8 lenght; //just for space bar
  
  win_font();
  win_font(char *);
  ~win_font();
  void load(char *);
};

#endif
