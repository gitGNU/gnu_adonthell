/*
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
#include "types.h"
#include "gfx.h"
#include "window.h"
#include "input.h"
//#include "ggi.h"
#include "pnm.h"


/*

void window::event()
{
  if(keyboard::is_pushed(S_Key))
    {
      if(!visible)
	{
	  show();
	}
    }
  else
    {
      if(keyboard::is_pushed(H_Key))
	{
	  if(visible)
	    {
	      hide();
	    }
	}
      else 
	if(text_action==TEXT_CHOICE && keyboard::is_pushed(A_Key))
	  {
	    
	    char titi[3][300];
	    
	    strcpy(titi[0],"choix 1: un canari mange du grain ");
	    strcpy(titi[1],"choix 2: oiseau");
	    strcpy(titi[2],"choix 3: chien");
	    
	    
	    set_choice(titi,3);//3
	  }
    }
  
  if(text_action==TEXT_CHOICE)
    {
    if(keyboard::is_pushed(U_Key)) up_choice();
    if(keyboard::is_pushed(D_Key)) down_choice();
    if(keyboard::is_pushed(G_Key)) printf("%d\n",get_choice());
    }

  if(text_action==TEXT_WRITE)
    {
      if(keyboard::is_pushed(T_Key)) cout << is_get_text() << endl;
      if(keyboard::is_pushed(G_Key)) cout << get_text()<<endl;
    }
  if(text_action==TEXT_READ)
    {
      if(keyboard::is_pushed(N_Key)) continue_text=true;
      if(keyboard::is_pushed(P_Key)) cout << is_end_text() << endl;
    }

}

*/

void window::set_what_show(u_int8 wsh)
{
  what_show=wsh;
}


u_int8 window::get_what_show()
{
  return(what_show);
}


void window::set_action(u_int8 m)
{
  text_action=m;
}

u_int8 window::get_action()
{
  return(text_action);
}

void window::set_open_mode(u_int8 m)
{
 mode_win=m;
}

u_int8 window::get_open_mode()
{
 return(mode_win);
}



void window::close()
{
  if(mode_win==WIN_NORMAL) visible=false; 
        else if(mode_win==WIN_L_R) close_r_l();
            else if(mode_win==WIN_EXPLODE) close_implode();
                 else if(mode_win==WIN_U_L) close_l_u();
                     else  if(mode_win==WIN_CENTER) close_center();
}


void window::close_r_l()
{ if(tmpl>0)
  {tmpl--;
   update_background();
  }
 else visible=false;
}


void window::close_center()
{
  if(tmpl>0)
    {tmpx++;tmpl-=2;update_background();}
   else
     {if(tmph>0)
         {tmpy++;tmph-=2;}
       else visible=false;
     }
}


void window::close_l_u()
{
  if(tmph>0){tmph--;update_background();}
  else{visible=false;}
}


void window::close_implode()
{
 if((tmpl==0) && (tmph==0))
      visible=false;
    else
      {
	if(tmpl>0){tmpl-=2;tmpx++;}
        if(tmph>0){tmph-=2;tmpy++;}
        update_background();
      }
}

void window::down_choice()
{
  if(choice==0) return;
  if(choice==nbchoice) choice=1;
  else choice++;
}

void window::draw()
{
  if(visible)
    { 
      
      switch(what_show)
      {
      case WIN_SHOW_ALL:
	draw_background();
	draw_border();
	break;
      case WIN_SHOW_BACKGROUND:
	draw_background();
	break;
      case WIN_SHOW_BORDER:
	draw_border();
	break;
      };
	
	 


      if(finish_open)
	{
	  draw_text();
	}
    }
}


void window::draw_border()
{
#define WIN_CORNER_RELX (WIN_CORNER_LENGHT>>1)
#define WIN_CORNER_RELY (WIN_CORNER_HEIGHT>>1)
#define WIN_HBRELX (WIN_HBORDER_LENGHT>>1)
#define WIN_HBRELY (WIN_HBORDER_HEIGHT>>1)
#define WIN_VBRELX (WIN_VBORDER_LENGHT>>1)
#define WIN_VBRELY (WIN_VBORDER_HEIGHT>>1)
  
hborder.putbox(tmpx-WIN_HBRELX,tmpy-WIN_HBRELY);
hborder.putbox(tmpx-WIN_HBRELX,tmpy-WIN_HBRELY+tmph);
vborder.putbox(tmpx-WIN_VBRELX,tmpy-WIN_VBRELY);
vborder.putbox(tmpx+tmpl-WIN_VBRELX,tmpy-WIN_VBRELY);
  
corner.putbox_mask(tmpx-WIN_CORNER_RELX,tmpy-WIN_CORNER_RELY);
corner.putbox_mask(tmpx+tmpl-WIN_CORNER_RELX,tmpy-WIN_CORNER_RELY);
corner.putbox_mask(tmpx-WIN_CORNER_RELX,tmpy+tmph-WIN_CORNER_RELY);
corner.putbox_mask(tmpx+tmpl-WIN_CORNER_RELX,tmpy+tmph-WIN_CORNER_RELY);
}


void window::draw_background()
{
  background.putbox_trans(tmpx,tmpy,WIN_BACK_TRANS);
}


void window::draw_choice()
{
  if(choice==0)return; 
  register int i;
  u_int16 tmp=0;
  for( i=(choice-1);i<nbchoice;i++)   
    {
      draw_text_screen(sxtext+WIN_CURSOR_LENGHT,
		       sytext+(text_window.nbline+tmp)
		       *WIN_FONT_HEIGHT
		       ,tabchoice[i]);
      tmp+=tabchoice[i].nbline;
    }
  cursor.putbox(sxtext, sytext+(text_window.nbline)*
		WIN_FONT_HEIGHT); 
}



void window::draw_text()
{
  if(text_window.lenght>0) draw_text_screen(sxtext,sytext,text_window);
  if(text_action==TEXT_WRITE)
    { 
      if(text_write.endwindow)
	{
	  text_write.pos++;
	  text_write.endwindow=false;
	}
      else
	{
	  draw_text_screen(sxtext,
			   sytext+(text_window.nbline+text_write_tmp.nbline)*
			   WIN_FONT_HEIGHT
			   ,text_write);
	}
    }
  else if(text_action==TEXT_CHOICE)
    {
      draw_choice();
    }
}



void window::draw_text_screen(u_int16 sx,u_int16 sy,text_w & txt)
{
  u_int16 i=0;
  u_int16 j;
  u_int8 k;
  bool b;
  txt.pos_tmp=txt.pos;
  while(((sy+i*WIN_FONT_HEIGHT)<eytext) && (txt.pos_tmp<txt.lenght))
    { 
      j=0;
      b=true;
      while((b) && ((sx+j)<extext) && (txt.pos_tmp<txt.lenght))
	{
	  k=txt.text[txt.pos_tmp];
	  if(k==' ') 
	    {
	      j+=WIN_SPACE_LENGHT;
	    }
	  else
	    { 
	      if(k=='\n') b=false;
	      else
		{ 
		  tablechar[k].putbox_mask(sx+j,sy+i*WIN_FONT_HEIGHT);
		  j+= tablechar[k].get_lenght();
		}
	    }
	  txt.pos_tmp++;
	}
      i++;
    }
  txt.endwindow=(txt.pos_tmp<txt.lenght && txt.pos_tmp!=0);
  txt.nbline=i;
}


bool window::end_text(text_w a)
{
  return(a.pos_tmp==a.lenght);
}



int window::get_choice()
{
  return(choice);
}


char * window::get_text()
{
  if(gettext)
    {
      gettext=false;
      return(text_write_tmp.text);
    }
  return(NULL);
}


void window::hide()
{ 
  open_close=false;
  finish_open=false;
}

void window::init(u_int16 x, u_int16 y, u_int16 l, u_int16 h,u_int8 style)
{
 
  corner.load("gfxtree/window/corner.pnm");  
  hbordertemplate.load("gfxtree/window/h_border.pnm");
  vbordertemplate.load("gfxtree/window/v_border.pnm");
  backgroundtmp.load("gfxtree/window/wintex.pnm");
  cursor.load("gfxtree/window/cursor.pnm");
  
  tmpl=l;
  tmph=h;
  xpos=x;
  ypos=y;
  lenght=l;
  height=h; 
  
  //text_window=new char[TEXT_MAX];
  text_window.pos=0;
  text_window.pos_tmp=0;
  text_window.lenght=0;
  text_window.nbline=0;

  //text_write=new char[TEXT_MAX];
  
  text_write.pos=0;
  text_write.lenght=0;
  text_write.pos_tmp=0;
  text_write.nbline=0;
  text_write.endwindow=false;

  text_write_tmp.pos=0;
  text_write_tmp.lenght=0;
  text_write_tmp.pos_tmp=0;
  text_write_tmp.nbline=0;
  text_action = TEXT_READ;//action;
  
  mode_win=style;
  what_show=WIN_SHOW_ALL;//wsh;  
  
  tabchoice=new text_w[WIN_NB_CHOICE];
  nbchoice=0;
  choice=0;

  finish_open=true;
  visible=false;
  enterkey_push=false;
  continue_text=false;
  gettext=false;

  init_table_char();
  
  update_background();
  update_text(); 
}


void window::init_table_char()
{ 
  char i;int j;
  u_int16 pos,tl;
  image *font = new image();
  FILE *f;
 
  if((f=fopen("gfxtree/window/font.idx","rb"))==NULL)
                  {printf("font.idx not found\n");exit(1);}    
  font->load("gfxtree/window/font.pnm"); 
  
  tablechar=new image[NB_TABLE_CHAR];
  j=0;
  while(j<NB_TABLE_CHAR && !feof(f) )
    {
      fread(&i,sizeof(i),1,f);
      fread(&pos,sizeof(pos),1,f);
      fread(&tl,sizeof(tl),1,f);
      tablechar[i].resize(tl,WIN_FONT_HEIGHT);
      tablechar[i].putbox_part_img(font,0,0,tl,WIN_FONT_HEIGHT,pos,0);
      j++;
    }
  fclose(f);
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

void window::init_explode()
{
  tmpx=xpos + (lenght>>1);
  tmpy=ypos + (height>>1);
  tmpl=0;
  tmph=0;
  update_background();
}


void window::init_u_l()
{
  tmpx=xpos;
  tmpy=ypos;
  tmph=0;
  tmpl=lenght;
  update_background();
}


bool window::is_get_text()
{
  return (gettext);
}

bool window::is_end_text()
{
  return(text_window.pos==text_window.lenght);
}


bool window::keyboard()
{
  if(input::is_pushed(Enter_Key))
        {if(enterkey_push) return false;
	    else return(enterkey_push=true);       
         }
  else return(enterkey_push=false);
}


void window::move(u_int16 x, u_int16 y)
{
  xpos=x;
  ypos=y;
  update_text();
}

void window::next_page()
{
  continue_text=true;
}


void window::open()
{ 
   vborder.resize(WIN_VBORDER_LENGHT,tmph);
   vborder.putbox_tile_img(&vbordertemplate);
   hborder.resize(tmpl,WIN_HBORDER_HEIGHT);
   hborder.putbox_tile_img(&hbordertemplate);
   
   if(mode_win==WIN_NORMAL){finish_open=true;update_text();}
     else if(mode_win==WIN_L_R) open_l_r();
        else if(mode_win==WIN_EXPLODE) open_explode();
	   else if(mode_win==WIN_U_L) open_u_l();  
	     else  if(mode_win==WIN_CENTER) open_center();
}


void window::open_center()
{
  if(tmpy>ypos)
    {tmpy--;tmph+=2;update_background();}
   else
     {if(tmpx>xpos)
         {tmpx--;tmpl+=2;update_background();}
        else {finish_open=true;update_text();}
     }
}

void window::open_explode()
{
 if((tmpx==xpos) && (tmpy==ypos))
       {finish_open=true;update_text();}
    else
      { 
 if(tmpx>xpos)
    {tmpx--;tmpl+=2;}
     
  if(tmpy>ypos)
    {tmpy--;tmph+=2;}
      update_background(); 
     }
}



void window::open_l_r()
{
  if(tmpl<lenght) 
     {tmpl++;
      update_background(); 
     }
  else{finish_open=true;update_text();}
}


void window::open_u_l()
{
  if(tmph<height) {tmph++;update_background();}
     else{finish_open=true;update_text();} 
}


void window::resize(u_int16 l, u_int16 h)
{
  lenght=tmpl=l;
  height=tmph=h;
  update_background();
  update_text();
}


void window::set_choice(char ch[][TEXT_CHOICE_LENGHT],int nbc)
{
  set_choice(ch,nbc,1);
}


void window::set_choice(char ch[][TEXT_CHOICE_LENGHT],int nbc,int cho)
{
  nbchoice=nbc;
  choice=cho;
  for(int i=0;i<nbc;i++)
    {
      strcpy(tabchoice[i].text,ch[i]);
      tabchoice[i].pos=0;
      tabchoice[i].pos_tmp=0;
      tabchoice[i].lenght=strlen(ch[i]);
    }
}

void window::set_text(const char *string)
{ 
  
  text_window.pos=0;
  text_window.pos_tmp=0;
  text_window.lenght=strlen(string);
  strcpy(text_window.text,string);
  continue_text=false;
}


void window::show()
{  
 
 visible=true;
 finish_open=false;
 open_close=true;

 if(mode_win==WIN_NORMAL) init_normal();
 else if(mode_win==WIN_L_R) init_l_r();
 else if(mode_win==WIN_EXPLODE) init_explode();
 else if(mode_win==WIN_U_L) init_u_l();
 else  if(mode_win==WIN_CENTER) init_explode();
}


void window::update()
{   
  if(visible)
    {
      if(finish_open)
	{
	  if(text_action==TEXT_READ)
	    {
	      if(continue_text)
		{
		  text_window.pos=text_window.pos_tmp;
		  continue_text=false;
		}
              /*if(continue_text)
		continue_text=false;  
                else 
		if(end_text(text_window))
		{
		      if(keyboard()) 
		      hide();
		    } 
		  else
		    {
		      if(keyboard()) 
			{
			  continue_text=true;
			  text_window.pos=text_window.pos_tmp;
			}
		    }	    
	      */
	    }
	  else 
	    {
	      
	      if(text_action==TEXT_WRITE)
		{write();}
	      else
		{	    
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




void window::up_choice()
{
  if(choice==0) return;
  if(choice==1) choice=nbchoice;
  else choice--;

}


window::window()
{
 
}


window::window(u_int16 x, u_int16 y, u_int16 l, u_int16 h,u_int8 style)
{
 init(x,y,l,h,style);
}

window::~window()
{
 delete [] tablechar;
 //delete [] text_window.text;
 delete [] tabchoice;

}

void window::write()
{
  static u_int16 curletter;
  static bool alwayspush;
  u_int16 n=input::getkeypressed();  
  if(input::is_pushed(curletter))
    {
      if(!alwayspush)     
	{
	  if(text_write.lenght<TEXT_MAX)
	    {
	      if(n==Enter_Key)
		{
		  text_write.text[text_write.lenght++]='\n';
		  text_write.text[text_write.lenght]='\0';
		  gettext=true;
		  strcpy(text_write_tmp.text,text_write.text);
		  
		  text_write.lenght=0;
		  text_write.pos_tmp=0;
		  text_write.pos=0;
		  text_write.nbline=0;
		}
		  
	      else   
		{
		  if(n==Backspace_Key && text_write.lenght>0) text_write.lenght--;
		  else text_write.text[text_write.lenght++]=n;	    
		}
	    }
	  alwayspush=true;
	}
    }
  else
    {
      alwayspush=false;
    }
    curletter=n;
}
