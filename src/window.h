/*
    Copyright (C) 1999 Joel Vennin.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.
    See the COPYING file for more details.

*/
#ifndef _window_h_
#define _window_h_

/* window.cc */

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
     
    bool visible;
    bool enterkey_push;
    bool continue_text;   
    bool finish_open;
    bool open_close; //true to open    


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
    char * text_window;
    u_int16 text_window_lenght;
    u_int16 text_window_pos;
    u_int16 text_window_pos_tmp;
    
    u_int8 mode_win;
    
    bool keyboard();
    bool end_text();
    
    void init_table_char();
    void update_background();
    void update_text();
    void draw_text();
    void init_normal();
    void draw_border();    
    void open();
    void close();
    void open_l_r();
    void init_l_r();
    void close_r_l();

 public:
    window();
    window(u_int16 x, u_int16 y, u_int16 l, u_int16 w,u_int8 style);
    ~window();
    void init(u_int16 x, u_int16 y, u_int16 l, u_int16 w,u_int8 style);
    void update();
    void draw();
    void resize(u_int16 l, u_int16 h);
    void move(u_int16 x, u_int16 y);
    void set_text(char string[]);
    void show();
    void hide();
    void change_mode(u_int8 m);     
  };

#define WIN_NORMAL 0
#define WIN_EXP_IMP 1
#define WIN_L_R 2

#define NB_TABLE_CHAR 130
#define WIN_SPACE_LENGHT 7

#define WIN_CORNER_LENGHT 16
#define WIN_CORNER_HEIGHT 16
#define WIN_HBORDER_LENGHT 1
#define WIN_HBORDER_HEIGHT 6
#define WIN_VBORDER_LENGHT 6
#define WIN_VBORDER_HEIGHT 1
#define WIN_BACK_LENGHT 70
#define WIN_BACK_HEIGHT 70
#define WIN_BACK_TRANS 75

#define WIN_FONT_HEIGHT 8
#define WIN_FONT_LENGHT 7


#endif
