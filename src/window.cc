/*
    $Id$
    
    Copyright (C) 1999 Joel Vennin.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.
    See the COPYING file for more details.

    This file is a part of the Adonthell project.
*/

#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "gfx.h"
#include "window.h"
#include "keyboard.h"
#include "pnm.h"

void window::init(u_int16 x, u_int16 y, u_int16 l, u_int16 h,u_int8 style)
{
  corner.load("gfxtree/window/corner.pnm");  
  hbordertemplate.load("gfxtree/window/h_border.pnm");
  vbordertemplate.load("gfxtree/window/v_border.pnm");
  backgroundtmp.load("gfxtree/window/wintex.pnm");
    
  tmpl=l;
  tmph=h;
  xpos=x;
  ypos=y;
  lenght=l;
  height=h; 
  
  text_window=new char[5000];
  //  text_window=NULL;
  text_window_pos=0;
  text_window_lenght=0;
  
  mode_win=style;
  finish_open=true;
  visible=false;
  enterkey_push=false;
  init_table_char();
  update_background();
  update_text();
}


void window::init_table_char()
{ char i;int j=0;
  u_int16 pos,tl;

  image * font = new image();
  // u_int16 lenght,height;

  FILE *f;
  if((f=fopen("gfxtree/window/font.idx","rb"))==NULL)
                  {printf("font.idx not found\n");exit(1);}    
  //  FILE * f2=fopen("gfxtree/window/font.pnm","r");
  font->load("gfxtree/window/font.pnm"); 
  //font=read_pnm(f2,&lenght,&height); 
  tablechar=new image[NB_TABLE_CHAR];
  while(j<NB_TABLE_CHAR && !feof(f) )
    {
     fread(&i,sizeof(i),1,f);
     fread(&pos,sizeof(pos),1,f);
     fread(&tl,sizeof(tl),1,f);
     tablechar[i].resize(tl,WIN_FONT_HEIGHT);
     tablechar[i].putbox_part_img(font,0,0,tl,WIN_FONT_HEIGHT,pos,0);
     //     tablechar[i].size(tl,WIN_FONT_HEIGHT);
     //  tablechar[i].putbox_font_img(font,pos,lenght);
     j++;
    }
  fclose(f);
  // fclose(f2);
  delete(font);
}


void window::init_normal()
{
 tmpx=xpos;
 tmpy=ypos;
 tmpl=lenght;
 tmph=height;
 update_background(); 
}


void window::init_l_r()
{
  tmpx=xpos;
  tmpy=ypos;
  tmpl=0;
  tmph=height;
  update_background();
}


void window::update_text()
{
  sxtext=xpos+WIN_VBORDER_LENGHT;
  sytext=ypos+WIN_HBORDER_HEIGHT;
  extext=xpos+lenght-WIN_VBORDER_LENGHT-WIN_FONT_LENGHT;
  eytext=ypos+height-WIN_HBORDER_HEIGHT-WIN_FONT_HEIGHT;
}


void window::update_background()
{
  background.resize(tmpl,tmph);
  background.putbox_tile_img(&backgroundtmp);
}

void window::update()
{   
  if(visible)
  {
    if(finish_open)
	{
      if(continue_text) continue_text=false;  
      else 
        if(end_text())
	    {
	      if(keyboard()) hide();
	    } 
        else
	    {
	      if(keyboard()) 
          {
            continue_text=true;
		    text_window_pos=text_window_pos_tmp;
          }
        }
    }
    else
	{
	   if(open_close)open();
	   else close();
	}	               
  }  
}

void window::draw()
{
  if(visible)
    { draw_border();
      if(finish_open)
	{ draw_text(); }
    }
}

window::window()
{
  // init(50,10,150,100,WIN_NORMAL);
  visible = 0;
}



window::window(u_int16 x, u_int16 y, u_int16 l, u_int16 h,u_int8 style)
{
 init(x,y,l,h,style);
}



window::~window()
{
 delete [] tablechar;
 delete [] text_window;
}



bool window::keyboard()
{
  if(keyboard::is_pushed(Enter_Key))
        {if(enterkey_push) return false;
	    else return(enterkey_push=true);       
         }
  else return(enterkey_push=false);
}



bool window::end_text()
{
  return(text_window_pos_tmp==text_window_lenght);
}



void window::set_text(const char *string)
{ 
  
  //  delete[] text_window;
  text_window_pos=0;
  text_window_lenght=strlen(string);
  //  text_window=new char [text_window_lenght];
  strcpy(text_window,string);
  continue_text=true;
}



void window::show()
{  
 
 visible=true;
 finish_open=false;
 open_close=true;
 if(mode_win==WIN_NORMAL) init_normal();
    else
      if(mode_win==WIN_L_R) init_l_r();
}



void window::hide()
{ 
  open_close=false;
  finish_open=false;
   
}



void window::resize(u_int16 l, u_int16 h)
{
  lenght=tmpl=l;
  height=tmph=h;
  update_background();
  update_text();
}

void window::move(u_int16 x, u_int16 y)
{
  xpos=x;
  ypos=y;
  update_text();
}

void window::change_mode(u_int8 m)
{
 mode_win=m;
}


void window::draw_text()
  // The bug is somewhere here! >:]

{
  u_int16 i=0;
 u_int16 j;u_int8 k;
 text_window_pos_tmp=text_window_pos;
 while(((sytext+i*WIN_FONT_HEIGHT)<eytext) && (text_window_pos_tmp<text_window_lenght))
   { 
     j=0;
     while(((sxtext+j)<extext) && (text_window_pos_tmp<text_window_lenght))
       {k=text_window[text_window_pos_tmp];
        if(k==' ' || k>=NB_TABLE_CHAR) 
	    {j+=WIN_SPACE_LENGHT; }
          else if (tablechar[k].get_height() != 0)
	    { 
	      tablechar[k].putbox_mask(sxtext+j,sytext+i*WIN_FONT_HEIGHT);
	      j+= (tablechar[k].get_lenght()/tablechar[k].get_sizefactor());
             }
       text_window_pos_tmp++;
       }
     i++;
   }
}


void window::open()
{ 
  if(mode_win==WIN_NORMAL){finish_open=true;update_text();}
  if(mode_win==WIN_L_R) open_l_r();
}


void window::close()
{
  if(mode_win==WIN_NORMAL) visible=false; 
        else if(mode_win==WIN_L_R) close_r_l();
}


void window::open_l_r()
{
  if(tmpl<lenght) 
     {tmpl++;
      update_background(); 
     }
  else{finish_open=true;update_text();}
}

void window::close_r_l()
{ if(tmpl>0)
  {tmpl--;
   update_background();
  }
 else visible=false;
}

void window::draw_border()
{
  u_int16 i;
  #define WIN_CORNER_RELX (WIN_CORNER_LENGHT>>1)
  #define WIN_CORNER_RELY (WIN_CORNER_HEIGHT>>1)
  #define WIN_HBRELX (WIN_HBORDER_LENGHT>>1)
  #define WIN_HBRELY (WIN_HBORDER_HEIGHT>>1)
  #define WIN_VBRELX (WIN_VBORDER_LENGHT>>1)
  #define WIN_VBRELY (WIN_VBORDER_HEIGHT>>1)
  background.putbox_trans(tmpx,tmpy,WIN_BACK_TRANS);
      for(i=0;i<tmpl;i++)
          {hbordertemplate.putbox(tmpx+(i*WIN_HBORDER_LENGHT)-WIN_HBRELX,
                                        tmpy-WIN_HBRELY);
           hbordertemplate.putbox(tmpx+(i*WIN_HBORDER_LENGHT)-WIN_HBRELX,
                                        tmpy-WIN_HBRELY+tmph);
          }
      for(i=0;i<tmph;i++)
          {vbordertemplate.putbox(tmpx-WIN_VBRELX,tmpy+(i*WIN_VBORDER_HEIGHT)-WIN_VBRELY);
           vbordertemplate.putbox(tmpx+tmpl-WIN_VBRELX,tmpy+(i*WIN_VBORDER_HEIGHT)-
                                        WIN_VBRELY);
          }
      corner.putbox_mask(tmpx-WIN_CORNER_RELX,tmpy-WIN_CORNER_RELY);
      corner.putbox_mask(tmpx+tmpl-WIN_CORNER_RELX,tmpy-WIN_CORNER_RELY);
      corner.putbox_mask(tmpx-WIN_CORNER_RELX,tmpy+tmph-WIN_CORNER_RELY);
      corner.putbox_mask(tmpx+tmpl-WIN_CORNER_RELX,tmpy+tmph-WIN_CORNER_RELY);
}
