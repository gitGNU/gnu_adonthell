#ifndef WIN_BORDER_H_
#define WIN_BORDER_H_
 
class image;
class win_container;

class win_border
{


 
  //image * h_border;
  //image * v_border;

 public:
  image * h_border_template;
  image * v_border_template;
  image * corner;

  win_border(char *);
  ~win_border();
  void load(char *);
};


class win_background
{
 public:
  image * background_template;
  win_background(char *);
  ~win_background();
  void load(char *);
};
#endif








