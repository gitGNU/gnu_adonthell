#ifndef WIN_BASE_H_
#define WIN_BASE_H_



class win_base
{
 protected: 
  
  u_int16 x;
  u_int16 y;
  u_int16 l; //lenght
  u_int16 h; //height

  bool select;
  bool visible;

 
 public:
  
  win_base();
  win_base(u_int16,u_int16,u_int16,u_int16);
 
  bool is_selected();
  void set_selected(bool);

  bool is_visible();
  void set_visible(bool);
  
  u_int16 getx();
  u_int16 gety();
  u_int16 getl();
  u_int16 geth();

};
#endif




















