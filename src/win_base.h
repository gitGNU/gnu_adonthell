#ifndef WIN_BASE_H_
#define WIN_BASE_H_

class win_container;
class drawing_area;
class image;
class win_border;
class win_background;

class win_base
{
  void init_base(u_int16 tx,u_int16 ty, u_int16 tl,u_int16 th,win_container *,drawing_area *);

  image * corner;
  image * h_border;
  image * v_border;
  image * background;

  win_border * wborder;
  win_background * wback;
  
  u_int8 level_trans_back; //transparency of background

 protected: 
  
  win_container *wc;
  drawing_area *da;

  u_int16 x;
  u_int16 y;
  u_int16 length; //length
  u_int16 height; //height

  bool selected;
  bool visible;
  
  

 public:
  
  win_base(u_int16,u_int16,u_int16,u_int16);
  win_base(u_int16,u_int16,u_int16,u_int16,win_container *);
  win_base(u_int16,u_int16,u_int16,u_int16,win_container *,drawing_area *);
  ~win_base();

  //select && visible
  void select();
  void unselect();
  void show();
  void hide();
  
  //position && size
  u_int16 get_x();
  u_int16 get_y();
  u_int16 get_length();
  u_int16 get_height();
  void resize(u_int16,u_int16);
  void move(u_int16,u_int16);

  //drawing_are
  drawing_area * get_drawing_area();

  //border && backgropund
  void set_border(win_border *);
  void draw_border();
  void resize_border();
  void draw_background();
  void set_background(win_background * );
  void resize_background();
  
};
#endif




















