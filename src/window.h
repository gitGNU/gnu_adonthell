/*
     Copyright (C) 1999 Joel Vennin.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.
    See the COPYING file for more details.

*/


#define TEXT_MAX 5000    
#define WIN_NB_CHOICE 30     
#define TEXT_CHOICE_LENGHT 300

#ifndef _window_h_
#define _window_h_


/* window.cc */

typedef struct 
{
  char text[TEXT_MAX];
  u_int16 lenght;
  u_int16 pos;
  u_int16 pos_tmp;
  u_int16 nbline;
  bool endwindow;
}text_w;


class window
{
  image corner;
  image hbordertemplate;
  image vbordertemplate;
  image hborder;
  image vborder;
  image background;
  image backgroundtmp;
  image *tablechar;    
  image cursor;
  
  
  bool visible;//true if you want show 
  bool enterkey_push;//true if enter_key is pushed 
  bool continue_text;   
  bool finish_open;//true if window is end of opening
  bool open_close; //true to open    
  bool gettext;//if you can get_tetx

  
  u_int16 xpos;
  u_int16 ypos;
  u_int16 lenght;
  u_int16 height;
  u_int16 tmpx;
  u_int16 tmpy;
  u_int16 tmph;
  u_int16 tmpl; 
  
  u_int16 sxtext;
  u_int16 sytext;
  u_int16 extext;
  u_int16 eytext;
  
  text_w text_window; //text with set_text
  text_w text_write_tmp;//text was writting
  text_w text_write;//text is writting
  

  text_w * tabchoice; //tab with choice set with set_choice(char**,int)
  int  nbchoice;
  int choice;
  
  
  
  u_int8 mode_win;
  u_int8 text_action;
  u_int8  what_show;//if show just text, border and anything
  
  
  
  
  bool keyboard();
  bool end_text(text_w);
  
  
  void init_table_char();
  void update_background();
  void update_text();
  
  void write();
  
  void draw_text_screen(u_int16 sx,u_int16 sy,text_w&);
  void draw_text();
  void draw_border();    
  void draw_background();
  void draw_choice();
  
  void init_normal();
  void open();
  void close();
  void open_l_r();
  void init_l_r();
  void close_r_l();
  void open_center();
  void close_center();
  void init_u_l();
  void open_u_l();
  void close_l_u();
  void init_explode();
  void open_explode();
  void close_implode();
 public:
  
  window();
  
  window(u_int16 x, u_int16 y, u_int16 l, u_int16 w,u_int8 style);
  /*
    x: pos x of window
    y: pos y of window
    l: lenght of window
    w: height of window
    style: how to open window : NORMAL , EXPLODE , CENTER (height before lenght) ......
    
    
    wsh: What do you want to show : just text(WIN_SHOW_TEXT), text & border(WIN_SHOW_TEXT_BORDER),
    text & background(WIN_SHOW_TEXT_BACKGROUND),and all (WIN_SHOW_ALL)
  */
  
  ~window();
  
  void init(u_int16 x, u_int16 y, u_int16 l, u_int16 w,u_int8 style);   
  /*
    x: pos x of window
    y: pos y of window
    l: lenght of window
    w: height of window
    style: how to open window : NORMAL , EXPLODE , CENTER (height before lenght) ......
  */
  
  
  void update();
  //update all window can do
  
  void draw();
  //draw with propertie of window
  
  void resize(u_int16 l, u_int16 h);
  //change size of window
  
  void move(u_int16 x, u_int16 y);
  //move window
  
  void set_text(const char string[]);
  //set text,if you want that window notshow text,just NULL     
  
  int get_choice();
  //get choice, return 0 if not choice
  
  void set_choice(char ch[][TEXT_CHOICE_LENGHT],int);
  //ch is list of your choice, int is number of choice,default choice is choice number one
  //example
  /*      
	  char table[3][300];
	  strcpy(table[0],"Go");
	  strcpy(table[1],"Not me");
	  strcpy(table[2],"I'm waiting");
	 
	  set_choice(table,3);//3
  */
  
  void set_choice(char ch[][TEXT_CHOICE_LENGHT],int,int);//when you have choice
  //same that up, but second int is default choice
  
  void up_choice();
  //decrease choice
  
  void down_choice();
  //increase choice
  
  char * get_text();
  //get text was writting by user, return NULL if not text
  
  bool is_get_text();
  //if TRUE you can get_text,else FALSE
  
 
  void  next_page();
  //just in TEXT_READ, if more text then change page else you have an empty window
  
  bool is_end_text();
  //just in TEXT_READ
  //return TRUE if end text

  void show();
  //show window     
  
  void hide();
  //close window     
    

  void set_open_mode(u_int8 m);
  //change how window open and close (style)
  u_int8 get_open_mode();


  void set_what_show(u_int8 wsh);
  //wsh: What do you want to show : just text(WIN_SHOW_TEXT), text & border(WIN_SHOW_TEXT_BORDER),
  //                                  text & background(WIN_SHOW_TEXT_BACKGROUND),and all (WIN_SHOW_ALL)
  u_int8 get_what_show();


  void set_action(u_int8 m);
  //change if you want read,write or choice
  u_int8 get_action();

  
  
  //  void event();
  
  
};

//what show
#define WIN_SHOW_ALL 0
#define WIN_SHOW_TEXT 1
#define WIN_SHOW_BACKGROUND 2
#define WIN_SHOW_BORDER 3



//style to open and close
#define WIN_NORMAL 0
#define WIN_EXP_IMP 1
#define WIN_L_R 2
#define WIN_CENTER 3
#define WIN_EXPLODE 4
#define WIN_U_L 5

//action
#define TEXT_READ 0
#define TEXT_WRITE 1
#define TEXT_CHOICE 2


#define NB_TABLE_CHAR 255
#define WIN_SPACE_LENGHT 7

#define WIN_CORNER_LENGHT 16
#define WIN_CORNER_HEIGHT 16
#define WIN_HBORDER_LENGHT 1
#define WIN_HBORDER_HEIGHT 6
#define WIN_VBORDER_LENGHT 6
#define WIN_VBORDER_HEIGHT 1
#define WIN_BACK_LENGHT 70
#define WIN_BACK_HEIGHT 70
#define WIN_BACK_TRANS 180
#define WIN_FONT_HEIGHT 8
#define WIN_FONT_LENGHT 7
#define WIN_CURSOR_HEIGHT 8
#define WIN_CURSOR_LENGHT 8



#endif
