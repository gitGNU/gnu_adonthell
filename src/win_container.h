#ifndef WIN_CONTAINER_H_
#define WIN_CONTAINER_H_

class win_base;
class win_write;
class win_label;
class win_font;
class win_border;
class drawing_area;
class win_background;

class win_container : public win_base
{
  
 private:
  win_container * wc;
  
  win_background * wback;
  win_border * wb;
  drawing_area * da;

 protected:
  list<win_label *> l_label;
  list<win_write *> l_write;
  list<win_container *> l_container;
  
 public:
  win_container();
  win_container(u_int16,u_int16,u_int16,u_int16);
  win_container(u_int16,u_int16,u_int16,u_int16,win_container *);
  ~win_container();
  
  win_label * add_label(u_int16,u_int16,u_int16,u_int16,win_font *);
  win_write * add_write(u_int16,u_int16,u_int16,u_int16,win_font *);
  win_container * add_container(u_int16,u_int16,u_int16,u_int16);
  win_border * set_border(char *);
  win_background * set_background(char *);
  
  void draw();
  void remove_label(win_label *);
  void remove_write(win_write *);
  void remove_container(win_container *);
  
  drawing_area * get_da();
  
  void update();
  
  void move(u_int16,u_int16);
  void resize(u_int16, u_int16);

  void show();
  void show_all();
  void hide();
  void hide_all();
};

#endif



