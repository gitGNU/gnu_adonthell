#ifndef WIN_IMAGE_H_
#define WIN_IMAGE_H_


class win_base;
class image;
class win_container;
class drawing_area;

class win_image : public win_base
{
  
  image * picture;
 public:
  win_image(u_int16,u_int16,image * tpic,win_container * wc);
  void set_image(image *);
  void draw();
  void update();

};
#endif
