
/* 
   %include "win_font.h"
   %include "win_theme.h"
   %include "win_base.h"
   %include "win_container.h"
   %include "win_label.h"
   %include "win_image.h"
   %include "win_scrolled.h"
   %include "win_select.h"
   %include "win_manager.h"
*/ 

class win_font{

 private:
  void erase();
  void init_in_table();
  
  bool table_core[WIN_NB_TABLE_CHAR];

  image * table;  
  
  u_int8 height_;
  u_int8 length_; //just for space bar
 
 public:
  
  win_font(char *);
  
  ~win_font();
  
  void load(char *);
  
  bool in_table(u_int16 tmp);

  //win_font & operator=(win_font &);
  
  u_int16 height(){return height_;}
  
  u_int16 length(){return length_;}
  
  image * cursor;
  
};


#define win_event_ACTIVATE  1 ;
#define win_event_UNACTIVATE   2;
#define win_event_UPDATE   3;
#define win_event_DRAW   4;
#define win_event_DRAW_ON_VISIBLE   5;
#define win_event_ACTIVATE_KEY   6;
#define win_event_SELECT   7;
#define win_event_UNSELECT   8;
#define win_event_KEYBOARD   9;
#define win_event_SCROLL_UP   10;
#define win_event_SCROLL_DOWN   11;
#define win_event_NEXT   12;
#define win_event_PREVIOUS   13;
#define win_event_CLOSE  14;
#define win_event_DESTROY  15;

class win_event
{
 public:
  
  win_event(){return_code_=0;}

  //Use this function to set a callback function
  void set_return_code (int rc) 
    { return_code_ = rc; }
    
  bool update();
  
  void py_signal_connect (PyObject *pyfunc, int signal, PyObject *args = NULL); 


  /*****************************************************/
  /*****************************************************/
  //DESTRUCTOR
  virtual ~win_event();
  
 protected:
  // the python callbacks connected to the window
  vector<py_callback *> py_callbacks;   
  
  
  Functor0 callback_[20];
  Functor0wRet<bool> callback_destroy_;
  Functor1<int> callback_quit_;
  
  
  int return_code_;
  
  //execute the callback function
  virtual void on_activate(){ if(callback_[ACTIVATE]) (callback_[ACTIVATE])();}
  virtual void on_unactivate(){ if(callback_[UNACTIVATE]) (callback_[UNACTIVATE])();}
  
  virtual void on_update() { if(callback_[UPDATE]) (callback_[UPDATE])();}
  
  virtual void on_draw_visible(){ if(callback_[DRAW_ON_VISIBLE]) (callback_[DRAW_ON_VISIBLE])();}
  virtual void on_draw(){ if(callback_[DRAW]) (callback_[DRAW])();}
  
  virtual void on_activate_key(){ if(callback_[ACTIVATE_KEY]) (callback_[ACTIVATE_KEY])();}
  virtual void on_select(){ if(callback_[SELECT]) (callback_[SELECT])();}
  virtual void on_unselect(){ if(callback_[UNSELECT]) (callback_[UNSELECT])();}
  
  virtual void on_up(){if(callback_[SCROLL_UP]) (callback_[SCROLL_UP])();}
  virtual void on_down(){if(callback_[SCROLL_DOWN]) (callback_[SCROLL_DOWN])();}
  
  virtual void on_next(){if(callback_[NEXT]) (callback_[NEXT])();}
  virtual void on_previous(){if(callback_[PREVIOUS]) (callback_[PREVIOUS])(); }
}; 


#define win_border_MINI 0;
#define win_border_NORMAL 1;

class win_border
{
 public:
  
  win_border();

  ~win_border();
  
  void load(char *,char *);
  
  void update();
  
  void destroy();

  void draw(drawing_area * da);
  
  void set_visible_border(bool b){visible_border_=b;}
  
  void set_brightness_border(bool b);
  
  void set_border(win_theme & wth, u_int8 size = win_border::NORMAL);

  void set_trans_border(bool b);//{trans_=b;}

  u_int16 length_border();
  
  u_int16 height_border();

 private:
  
  void init();

  void refresh();
  
  image * h_border_template_;
  
  image * v_border_template_;
  
  image * border_[NB_BORDER_IMAGE];
  
  image * border_brightness_[NB_BORDER_IMAGE];

  image ** border_draw_;

  bool visible_border_;
  
  bool brightness_;

  u_int8 trans_;

  win_base * wb_;
};


class win_background
{
 public:
  
  win_background();
  
  ~win_background();
  
  void load(char *);
  
  void update();
  
  void destroy();

  void draw(drawing_area * da =NULL);

  void set_visible_background(bool b){visible_=b;}

  void set_background(win_theme &);
  
  void set_brightness_background(bool b);
  
  void set_trans_background(bool b);//{trans_=b;}
  
 private:   
  void refresh();

  void init();
  
  image * background_template_;
  
  image * background_;
  
  image * background_brightness_;
  
  image * background_draw_;
  
  win_base * wb_;
  
  bool visible_;

  bool brightness_;

  bool trans_;
};


  #define win_base_ALIGN_NONE  0;
  #define win_base_ALIGN_LEFT  1;
  #define win_base_ALIGN_CENTER  2;
  #define win_base_ALIGN_RIGHT  3;

class win_base: public win_event, public win_border, public drawing_area, public win_background
{
 public:
  
  /**
   * Default constructor:
   *    - not visible
   *    - x,y equals to 0
   *    - not focus
   *    - not activate
   *    - not brightness
   *    - not transluency
   *    - can be selected
   *    - alignement is ALIGN_NONE
   */
  win_base();
  
  /**
   * Return the relative horizontal position of the win_*.
   * @return horizontal position of the win_*.
   */
  s_int16 x() const 
    {return x_;}

  
  /**
   * Return the relative vertical position of the win_*.
   * @return vertical position of the win_*.
   */
  s_int16 y() const 
    {return y_;}
  

  /**
   * Return the pad horizontal position of the win_*.
   * @return the pad horizontal position of the win_*.
   */
  s_int16 & pad_x() 
    {return pad_x_;}
  
  
  /**
   * Return the pad vertical position of the win_*.
   * @return the pad vertical position of the win_*.
   */
  s_int16 & pad_y() 
    {return pad_y_;}

  
   /**
   * Return the horizontal position of the win_*.
   * @return the horizontal position of the win_*.
   */
  s_int16 real_x() const 
    {return drawing_area::x();}
  
  
  /**
   * Return the vertical position of the win_*.
   * @return the vertical position of the win_*.
   */
  s_int16 real_y() const 
    {return drawing_area::y();}
  

  /** Move the win_*.
   *  @param tx new horizontal position.
   *  @param ty new vertical position.
   */  
  virtual void move(s_int16 tx,s_int16 ty);
  

   /** Rezise the win_*.
    *  @param tl new horizontal position.
    *  @param th new vertical position.
    */  
  virtual void resize(u_int16 tl, u_int16 th);
  
  
  /** Test if win_* is visible
   *  @return true if visible else false 
   */
  bool is_visible() const 
    {return visible_;}
  
  
  /** Set the visible parameter
   * @param b true if the win_* should be visible, false otherwise
   */
  void set_visible(const bool b) 
    {visible_=b;}
  

  /** Test if win_* is activated
   *  @return true if activate else false 
   */
  bool is_activate() const
    {return activate_;}
  
  
  /** Set the activate parameter
   * When a win_* is setup on, the keys queue is cleared
   *
   * @param b true if the win_* should be visible, false otherwise
   */
  void set_activate(const bool b)
    {if(activate_=b) {on_activate();input::clear_keys_queue();}else on_unactivate();}
  
  
  /** Test if win_* has focus on
   *  @return true if focus on else false 
   */
  bool is_focus()const 
    {return focus_;}
  
  
  /** Set the focus parameter
   * @param b true if the win_* should be focus on, false otherwise
   */
  void set_focus(const bool b)
    {focus_=b;}
  
  
  /** Test if win_* has focus on
   *  @return true if focus on else false 
   */
  bool is_trans() const 
    {return trans_;}
  
  
  /** Set the transluency parameter
   * @param b true if the win_* should be ins transluency, false otherwise
   */
  virtual void set_trans(const bool b)
    {set_trans_border(trans_ = b);set_trans_background(b);}

  
  /** Test if win_* is in brightness
   *  @return true if in brightness else false 
   */
  bool is_brightness() const
    {return brightness_;}
  
  
  /** Set the transluency parameter
   * @param b true if the win_* should be in transluency, false otherwise
   */
  virtual void set_brightness(const bool b)
    {set_brightness_border(brightness_ = b);set_brightness_background(b);}

  
  /** Set alignement of win_*
   * @param a can be, ALIGN_NONE, ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
   */
  void set_align(const u_int8 a)
    {align_=a;update_align();}
  
  
  /** Return alignment of win_*
   * @return align_ parameter
   */
  u_int8 align() const {return align_;}
  
  
  /** Test if win_* can be selected
   * @return true if it can be selected, false otherwise
   */
  bool is_can_be_selected() const 
    {return can_be_selected_;}
  
  
  /** Set the object to be selected
   * A win_obj can be selectable or not when it is inside a win_select
   * @param b true if the object can be selected inside a win_select., false otherwise
   */
  void set_can_be_selected(const bool b) 
    {can_be_selected_ = b;}
  
  
  /** Update process
   *  @return true if update is successful, false otherwise 
   */
  virtual bool update();
  
  
  /** Draw process
   * @return true if draw is successful, false otherwise
   */
  virtual bool draw();
  
  
  /** Input Update process 
   * @ 
   */
  virtual bool input_update();

  virtual ~win_base();
      
 protected:

  virtual void update_position();
  
  void update_align();

  void set_container(win_container * wc);
  
  

  s_int16 x_;

  s_int16 y_;
  
  s_int16 pad_x_;
  
  s_int16 pad_y_;

  u_int8 align_;

  bool visible_;
  
  bool focus_;

  bool activate_;
  
  bool brightness_;

  bool trans_;

  bool can_be_selected_;

  win_container * wb_father_;
  
}; 
  #define win_container_SPACE_WITH_BORDER  10;
  #define win_container_SPACE_WITH_OBJECT  10;
  
  #define win_container_LIST_LAYOUT  1;
  #define win_container_NO_LAYOUT  0;

class win_container : public win_base
{
 public:
  
  win_container();
  
  void move(s_int16, s_int16);
  
  void resize(u_int16, u_int16);

  virtual void add(win_base *);

  virtual void remove(win_base *);

  virtual void remove_all();

  virtual void destroy();

  virtual ~win_container();
  
  virtual bool update();

  virtual bool input_update();
  
  virtual bool draw();
  
  void set_visible_all(bool b);
  
  virtual void set_brightness(bool b);

  virtual void set_trans(bool b);

  virtual void set_space_with_border(u_int16 b){space_with_border_=b;update_layout();}
  
  virtual void set_space_with_object(u_int16 o){space_with_object_=o;update_layout();}
  
  u_int16 space_with_border(){return space_with_border_;}
  
  u_int16 space_with_object(){return space_with_object_;}

  void set_layout(u_int8 l){layout_=l;update_layout();}
  
  void set_focus_object(win_base * f);
  
  win_base * focus_object(){return focus_object_;}

 protected:
  
  void update_position();
  void update_layout();
  
  u_int16 space_with_object_;
  u_int16 space_with_border_;
  u_int8 layout_;

  lwb list_wb_;

  win_base * focus_object_;
  
};



class win_image : public image, public win_base
{
public:
  win_image(); 
  ~win_image(); 
  bool draw(); 
  bool update(); 
  bool input_update(); 
  void set_brightness(bool b); 
  void set_trans(bool b); 
  void pack(); 
  void set_auto_refresh(bool b);   
}; 


#define label_NOTHING 0;
#define label_AUTO_HEIGHT 1;
#define label_AUTO_SIZE 2;

#define label_KEY_CURSOR_NEXT SDLK_RIGHT;
#define label_KEY_CURSOR_PREVIOUS SDLK_LEFT;

class label: public image
{
 public:
  label();
  
  const string & text_string(){return text_;}
  const char * text_char(){return text_.c_str();} 

  void set_text(string str);
  
  void add_text(const string & str);
  
  void set_font(win_font & font);
  
  void set_form(const u_int8);
  
  void set_eot(const bool b);
  
  void set_cursor(const bool b){cursor_ = b;}
  
  void set_cursor_moveable(const bool b){cursor_moveable_ = b;}

  void resize(const u_int16 l,const u_int16 h);
 
  bool update();

  bool input_update();
 
 protected:
  
  void check_form();
  
  void form_auto_size();
  
  void form_auto_height();
  
  void form_nothing();
  
  void cursor_next();
  
  void cursor_previous();
  
  u_int8 word_place( u_int16 & cur_line_size, u_int16 & word_size );
  
  void find_word(u_int16 & begin,u_int16 &length, u_int16 & size_pix);
  
  //the text
  string text_;
  
  //the size of text
  u_int16 text_size_;
  
  //start to begin at the index
  u_int16 text_index_begin_;
  
  //it's the last letter which was drawing
  u_int16 text_index_end_;
  
  u_int16 cursor_pos_;
  
  bool cursor_;
  
  //set cursor visible
  bool cursor_visible_;
  
  //set cursor moveable
  bool cursor_moveable_;

  static u_int16 cursor_blink_speed_;
  
  u_int16 cursor_blink_cur_;

  bool text_eot_;

  //the font
  win_font * font_;
  
  //the style text displayed
  u_int8 form_;

 private: 
};


class win_label : public label, public win_base
{
public:
  win_label(); 
  ~win_label(); 
  bool draw(); 
  bool update(); 
  bool input_update(); 
  void set_brightness(bool b); 
  void set_trans(bool b); 
  void pack(); 
  void set_auto_refresh(bool b);   
}; 


class label_input : public label
{
 public:
    
  bool input_update();
  
 protected:
  void insert(const u_int16 pos, const char * letter);

  void insert_char(const u_int16 pos, const char letter);
  
  
};

class win_write : public label_input, public win_base
{
public:
  win_write(); 
  ~win_write(); 
  bool draw(); 
  bool update(); 
  bool input_update(); 
  void set_brightness(bool b); 
  void set_trans(bool b); 
  void pack(); 
  void set_auto_refresh(bool b);   
}; 


class win_mapview : public mapview, public win_base
{
public:
  win_mapview(); 
  ~win_mapview(); 
  bool draw(); 
  bool update(); 
  bool input_update(); 
  void set_brightness(bool b); 
  void set_trans(bool b); 
  void pack(); 
  void set_auto_refresh(bool b);   
}; 

 
class win_manager
{
 public:
  static void add(win_base *);

  static void add_after (win_base *, win_base *);
    
  static bool exist(win_base *);
  
  static void remove(win_base *);
  
  static void update();
  
  static void input_update();
  
  static void draw();
  
  static void set_focus(win_base*);
  
  static void destroy();


 private:
  
  static list<win_base *> lmanage;
  //static list<win_container *> ::iterator ilm;
  static win_base * wc;
};



class win_scrollbar
{
 public:
    
  win_scrollbar(char *rep);
  
  ~win_scrollbar();
  
  void update_bar();
  
  void load(char *);
  
  void update_back();
  
  void destroy();
  
  void set_scrollbar(win_theme  & wt);
  
  void set_visible_scrollbar(bool b)
    {visible_=b;}
  
  void set_trans_scrollbar(bool b)
    {
      if(!bar_draw_) return;
      if(trans_=b){ bar_draw_->set_alpha(130);back_draw_->set_alpha(130); }
      else{bar_draw_->set_alpha(255);back_draw_->set_alpha(255); }
    }
  
  void set_brightness_scrollbar(bool b){brightness_=b;refresh();}
  
  void draw(drawing_area * da= NULL);
  
  u_int16 height_bar(){if(bar_) return bar_->height();return 0;}

 private:
  
  void init();
  
  void refresh();

  win_scrollbar & operator=(win_scrollbar &);
  
  image * back_bot_;
  image * back_mid_;
  image * back_top_;
  image * bar_top_;
  image * bar_bot_;
  image * bar_mid_;
  image * bar_flex_;
  
  image * bar_;
  image * back_;
  
  image * bar_brightness_;
  image * back_brightness_;
  
  image * bar_draw_;
  image * back_draw_;

  bool visible_;
  bool brightness_;
  bool trans_;

  win_scroll * wsc_;
  
};


#define win_scroll_PAD_DEFAULT 5

class win_scroll : public win_container, public win_scrollbar
{
  
 public:
  //constructor x,y,length,height,and a theme
  win_scroll();
  
  //return difference between the last object and the visual height, I think you don't understand, but i know you never use thisfunction just me 
  u_int16 amplitude(){return max_amplitude_;}
  
  virtual void add(win_base *);
  
  virtual void remove(win_base *);
  
  virtual void remove_all();
  
  void resize(u_int16 tl,u_int16 th);
  
  void destroy();
  
  bool draw();
  
  bool update();
  
  bool input_update();

  void set_space_between_border(u_int16 );
  
  void set_space_between_object(u_int16 );
 
  u_int16 cursor_y(){return cursor_y_;}
  
  void set_auto_scrollbar(bool b){auto_scrollbar_=b;}
  
  void set_auto_refresh(bool b){auto_refresh_=b;}

  void set_brightness(bool b) {win_container::set_brightness(b);set_brightness_scrollbar(b);}
    
  void set_trans(bool b) {win_container::set_trans(b); set_trans_scrollbar(b);}

 

 protected:

  bool up();
  
  bool down();

  void find_amplitude();
  
  void update_amplitude();
  
  u_int16 max_amplitude_;
  
  u_int16 cur_amplitude_;
  
  u_int16 index_pad_;
  
  u_int16 cursor_y_;
  
  bool auto_scrollbar_;

  bool auto_refresh_;
};



#define win_select_MODE_BORDER 0;
  
#define win_select_MODE_BRIGHTNESS 1;

class win_select : public win_scroll
{
 public:
  
  win_select();

  bool input_update();
  
  void add(win_base * w);
  
  void remove(win_base * w);

  void remove_all();
  
  void set_mode(const u_int8 mode){mode_ = mode;}
  
  void set_border_select(win_border * border){border_select_ = border;}
  
  void set_circle(const bool b) {circle_ = b;}
  bool is_circle(){return circle_;}

  void set_default();

  void set_default_object(const win_base * wb);

  void set_default_position(const u_int16 pos);

  win_base * get_selected_object();
  
  u_int16 get_selected_position();

  
};



class win_theme
{
  
 public:
  
  win_theme(char *);
  
  ~win_theme();
  
  void destroy();
 
  static char * theme;
  
  friend class win_border;
  friend class win_background;
  friend class win_scrollbar;

};








