/*   
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "theme.h"
#include "scrollbar.h"

using namespace gui;

scrollbar::scrollbar()
{
  wsc_=NULL;
  init();
  set_visible_scrollbar(true);
  set_trans_scrollbar(false);
  set_brightness_scrollbar(false);
  refresh();
}

scrollbar::scrollbar(scroll * wsc)
{
  wsc_=wsc;
  init();
  set_visible_scrollbar(true);
  set_trans_scrollbar(false);
  set_brightness_scrollbar(false);
  refresh();
}

scrollbar::scrollbar(scrollbar & ws)
{
  wsc_=NULL;
  init();
  set_visible_scrollbar(true);
  set_trans_scrollbar(false);
  set_brightness_scrollbar(false);
  *this=ws;
  refresh();
}

scrollbar::scrollbar(char * rep)
{
  wsc_=NULL;
  init();
  set_visible_scrollbar(true);
  set_trans_scrollbar(false);
  set_brightness_scrollbar(false);
  load(rep);
  refresh();
}

scrollbar::~scrollbar()
{
  destroy();
}


void scrollbar::set_scrollbar(scrollbar & ws)
{
  *this=ws;
  refresh();
}

void scrollbar::set_scrollbar(theme & wt)
{
  *this=*(wt.my_scrollbar);
  refresh();
}

void scrollbar::init()
{
  back_bot_=NULL;
  back_mid_=NULL;
  back_top_=NULL;
  bar_top_=NULL;
  bar_bot_=NULL;
  bar_mid_=NULL;
  bar_flex_=NULL;
  bar_=NULL;
  back_=NULL;
  bar_brightness_=NULL;
  back_brightness_=NULL;
  bar_draw_=NULL;
  back_draw_=NULL;
}

scrollbar & scrollbar::operator=(scrollbar & wb)
{
  destroy();
  bar_top_=new gfx::image();
  *bar_top_=*(wb.bar_top_);
  bar_mid_=new gfx::image();
  *bar_mid_=*(wb.bar_mid_);
  bar_bot_=new gfx::image();
  *bar_bot_=*(wb.bar_bot_);
  bar_flex_=new gfx::image();
  *bar_flex_=*(wb.bar_flex_);

  back_top_=new gfx::image();
  *back_top_=*(wb.back_top_);
  back_mid_=new gfx::image();
  *back_mid_=*(wb.back_mid_);
  back_bot_=new gfx::image();
  *back_bot_=*(wb.back_bot_);

  bar_=new gfx::image();
  back_=new gfx::image();
  bar_->set_mask(true);
  back_->set_mask(true);
  

  bar_brightness_ = new gfx::image();
  back_brightness_ = new gfx::image();
  bar_brightness_->set_mask(true);
  back_brightness_->set_mask(true);
  
  update_back();
  update_bar();
  
  return *this;
}



void scrollbar::load(char * theme)
{
  destroy();
  char path[255];char tmp[255];
  strcpy(path,DIRECTORY);
  strcat(path,SCROLLBAR_DIRECTORY);
  strcat(path,theme);
  
  bar_=new gfx::image();
  back_=new gfx::image();
  bar_->set_mask(true);
  back_->set_mask(true);

  bar_brightness_ = new gfx::image();
  back_brightness_ = new gfx::image();
  bar_brightness_->set_mask(true);
  back_brightness_->set_mask(true);

  bar_top_=new gfx::image();
  strcpy(tmp,path);
  strcat(tmp,SCROLLBAR_BAR_TOP);
  bar_top_->load_pnm(tmp);
  
  bar_mid_=new gfx::image();
  strcpy(tmp,path);
  strcat(tmp,SCROLLBAR_BAR_MID);
  bar_mid_->load_pnm(tmp);
  
  bar_bot_=new gfx::image();
  strcpy(tmp,path);
  strcat(tmp,SCROLLBAR_BAR_BOT);
  bar_bot_->load_pnm(tmp);
  
  bar_flex_=new gfx::image();
  strcpy(tmp,path);
  strcat(tmp,SCROLLBAR_BAR_FLEX);
  bar_flex_->load_pnm(tmp);

  back_top_=new gfx::image();
  strcpy(tmp,path);
  strcat(tmp,SCROLLBAR_BACK_TOP);
  back_top_->load_pnm(tmp);
  
  back_mid_=new gfx::image();
  strcpy(tmp,path);
  strcat(tmp,SCROLLBAR_BACK_MID);
  back_mid_->load_pnm(tmp);
  
  back_bot_=new gfx::image();
  strcpy(tmp,path);
  strcat(tmp,SCROLLBAR_BACK_BOT);
  back_bot_->load_pnm(tmp); 
}

void scrollbar::update_back()
{
  if(!wsc_ || !back_) return;
  
  back_->resize(back_mid_->length(),wsc_->height());
  back_->tile(*back_mid_);
  
  back_top_->draw(0,0,NULL,back_);
  back_bot_->draw(0,wsc_->height()-back_bot_->height(),NULL,back_);
  
  back_brightness_->brightness(*back_, BRIGHTNESS_LEVEL);
}

void scrollbar::refresh()
{
  if(brightness_)
    {
      bar_draw_=bar_brightness_;
      back_draw_=back_brightness_;
    }
  else
    {
     bar_draw_=bar_;
     back_draw_=back_;
    }
}


void scrollbar::destroy()
{
  if(back_bot_) delete back_bot_;
  if(back_top_) delete back_top_;
  if(back_mid_) delete back_mid_;
  if(bar_bot_) delete bar_bot_;
  if(bar_mid_) delete bar_mid_;
  if(bar_top_) delete bar_top_;
  if(bar_flex_) delete bar_flex_;
  if(bar_) delete bar_;
  if(back_) delete back_;
  if(bar_brightness_) delete bar_brightness_;
  if(back_brightness_) delete back_brightness_;
}


void scrollbar::update_bar()
{
  if(!wsc_ || !bar_) return;
  if (!(wsc_->height() + wsc_->amplitude()))  return; 
  
  u_int16 calcul =  (wsc_->height() * wsc_->height()) / (wsc_->height() + wsc_->amplitude()); 
  
  if( calcul > bar_top_->height() + bar_mid_->height() + bar_bot_->height())
    {
      bar_->resize(bar_top_->length(), calcul);
      bar_->tile(*bar_flex_);
      bar_top_->draw(0,0,NULL,bar_);
      bar_bot_->draw(0,bar_->height() - bar_bot_->height(),NULL,bar_);
      bar_mid_->draw(0,(bar_->height() - bar_mid_->height() ) >> 1, NULL,bar_);
    }
  else
    {
      bar_->resize(bar_top_->length(), bar_top_->height() + bar_mid_->height() + bar_bot_->height());
      bar_top_->draw(0,0,NULL,bar_);
      bar_bot_->draw(0,bar_->height() - bar_bot_->height(),NULL,bar_);
      bar_mid_->draw(0,bar_top_->height(),NULL,bar_);
    }
  bar_brightness_->brightness(*bar_,BRIGHTNESS_LEVEL);
}


void scrollbar::draw(gfx::drawing_area * da)
{
  if(!visible_ || !back_draw_ || !bar_draw_) return; 
  back_draw_->draw(wsc_->real_x() + wsc_->length() - back_->length(), wsc_->real_y() , da );
  bar_draw_->draw(1 + wsc_->real_x() + wsc_->length() - back_->length(), wsc_->real_y() + wsc_->cursor_y() , da);
}
