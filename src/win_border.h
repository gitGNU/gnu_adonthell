#ifndef WIN_BORDER_H_
#define WIN_BORDER_H_
 
class image;
class win_container;

class win_border
{
 private:
  bool visible;
  image * h_border_template;
  image * v_border_template;
  image * corner;
  image * h_border;
  image * v_border;

  win_container * wc;

 public:
  win_border(win_container *);
  win_border(char *,win_container *);
  ~win_border();
  void show();
  void update();
  void hide();
  void load(char *);
  void draw();
};


class win_background
{
 private:
  bool visible;
  image * background;
  image * background_template;

  u_int8 level_trans;

  win_container * wc;

 public:
  win_background(win_container *);
  win_background(char *,win_container *);
  ~win_background();
  void show();
  void update();
  void hide();
  void load(char *);
  void draw();
};
#endif








