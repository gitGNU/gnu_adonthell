#ifndef WIN_CONTAINER_H_
#define WIN_CONTAINER_H_

class image;
class win_base;
class win_write;
class win_label;
class win_font;
class win_border;
class win_image;
class drawing_area;
class win_background;

class win_container : public win_base
{
 
 protected:
  list<win_label *> l_label;
  list<win_write *> l_write;
  list<win_container *> l_container;
  list<win_image *> l_image;

 public:
  win_container(u_int16,u_int16,u_int16,u_int16);
  win_container(u_int16,u_int16,u_int16,u_int16,win_container *);
  ~win_container();
  
  win_label * add_label(u_int16,u_int16,u_int16,u_int16,win_font *);
  win_write * add_write(u_int16,u_int16,u_int16,u_int16,win_font *);
  win_container * add_container(u_int16,u_int16,u_int16,u_int16);
  win_image * add_image(u_int16,u_int16,image *);


  /* win_border * set_border(char *);
     win_background * set_background(char *);*/
  
  void draw();
  void remove(win_label *);
  void remove(win_write *);
  void remove(win_container *);
  void remove(win_image *);
  void update();
  

  void move(u_int16,u_int16);
  void resize(u_int16,u_int16);

  void show_all();
  void hide_all();
};

#endif



