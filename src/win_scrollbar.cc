/*   
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "win_theme.h"
#include "win_scrollbar.h"



win_scrollbar::win_scrollbar()
{
  wsc_=NULL;

  init();

  set_visible_scrollbar(true);
 
  set_trans_scrollbar(false);
  
  set_brightness_scrollbar(false);
  
  refresh();
}


win_scrollbar::win_scrollbar(win_scroll * wsc)
{
  wsc_=wsc;

  init();

  set_visible_scrollbar(true);
  
  set_trans_scrollbar(false);
  
  set_brightness_scrollbar(false);
  
  refresh();
}


win_scrollbar::win_scrollbar(win_scrollbar & ws)
{
  wsc_=NULL;

  init();

  set_visible_scrollbar(true);
  
  set_trans_scrollbar(false);
  
  set_brightness_scrollbar(false);
  
  *this=ws;

  refresh();
}


win_scrollbar::win_scrollbar(char * rep)
{
  wsc_=NULL;
  
  init();

  set_visible_scrollbar(true);
  
  set_trans_scrollbar(false);
  
  set_brightness_scrollbar(false);

  load(rep);

  refresh();
}

win_scrollbar::~win_scrollbar()
{
  destroy();
}


void win_scrollbar::set_scrollbar(win_scrollbar & ws)
{
  *this=ws;
  refresh();
}

void win_scrollbar::set_scrollbar(win_theme & wt)
{
  *this=*(wt.scrollbar);
  refresh();
}

void win_scrollbar::init()
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

win_scrollbar & win_scrollbar::operator=(win_scrollbar & wb)
{
  destroy();
  bar_top_=new image();
  *bar_top_=*(wb.bar_top_);
  bar_mid_=new image();
  *bar_mid_=*(wb.bar_mid_);
  bar_bot_=new image();
  *bar_bot_=*(wb.bar_bot_);
  bar_flex_=new image();
  *bar_flex_=*(wb.bar_flex_);

  back_top_=new image();
  *back_top_=*(wb.back_top_);
  back_mid_=new image();
  *back_mid_=*(wb.back_mid_);
  back_bot_=new image();
  *back_bot_=*(wb.back_bot_);

  bar_=new image();
  back_=new image();
  bar_->set_mask(true);
  back_->set_mask(true);
  

  bar_brightness_ = new image();
  back_brightness_ = new image();
  bar_brightness_->set_mask(true);
  back_brightness_->set_mask(true);
  
  update_back();
  update_bar();
  
  return *this;
}



void win_scrollbar::load(char * theme)
{
  destroy();
  char path[255];char tmp[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_SCROLLBAR_DIRECTORY);
  strcat(path,theme);
  
  bar_=new image();
  back_=new image();
  bar_->set_mask(true);
  back_->set_mask(true);

  bar_brightness_ = new image();
  back_brightness_ = new image();
  bar_brightness_->set_mask(true);
  back_brightness_->set_mask(true);

  bar_top_=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BAR_TOP);
  bar_top_->load_pnm(tmp);
  
  bar_mid_=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BAR_MID);
  bar_mid_->load_pnm(tmp);
  
  bar_bot_=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BAR_BOT);
  bar_bot_->load_pnm(tmp);
  
  bar_flex_=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BAR_FLEX);
  bar_flex_->load_pnm(tmp);

  back_top_=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BACK_TOP);
  back_top_->load_pnm(tmp);
  
  back_mid_=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BACK_MID);
  back_mid_->load_pnm(tmp);
  
  back_bot_=new image();
  strcpy(tmp,path);
  strcat(tmp,WIN_SCROLLBAR_BACK_BOT);
  back_bot_->load_pnm(tmp); 
}

void win_scrollbar::update_back()
{
  if(!wsc_ || !back_) return;
  
  back_->resize(back_mid_->length(),wsc_->height());
  
  
  back_->tile(*back_mid_);
  
  
  //back_->putbox_img(back_top_,0,0);
  back_top_->draw(0,0,NULL,back_);

  //back_->putbox_img(back_bot_,0,wsc_->height()-back_bot_->height());
  back_bot_->draw(0,wsc_->height()-back_bot_->height(),NULL,back_);
  
  
  back_brightness_->brightness(*back_, WIN_BRIGHTNESS_LEVEL);

}

void win_scrollbar::refresh()
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


void win_scrollbar::destroy()
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


void win_scrollbar::update_bar()
{
  if(!wsc_ || !bar_) return;
  if (!(wsc_->height() + wsc_->amplitude()))  return; 
  
  u_int16 calcul =  (wsc_->height() * wsc_->height()) / (wsc_->height() + wsc_->amplitude()); 
  
  //if(calcul == bar_->height() || bar_->height() == (bar_top_->height() + bar_mid_->height() + bar_bot_->height())) return;

  if( calcul > bar_top_->height() + bar_mid_->height() + bar_bot_->height())
    {
     

      bar_->resize(bar_top_->length(), calcul);
      
      //bar_->putbox_tile_img( bar_flex_ );
      bar_->tile(*bar_flex_);
      
      //bar_->putbox_img(bar_top_,0,0);
      bar_top_->draw(0,0,NULL,bar_);

      //bar_->putbox_img(bar_bot_, 0, bar_->height() - bar_bot_->height());
      bar_bot_->draw(0,bar_->height() - bar_bot_->height(),NULL,bar_);

      //bar_->putbox_img(bar_mid_,0,( bar_->height() - bar_mid_->height() ) >>1 );
      bar_mid_->draw(0,(bar_->height() - bar_mid_->height() ) >> 1, NULL,bar_);
    }
  else
    {
      bar_->resize(bar_top_->length(), bar_top_->height() + bar_mid_->height() + bar_bot_->height());
      
      //bar_->putbox_img(bar_top_,0,0);
      bar_top_->draw(0,0,NULL,bar_);
      
      //bar_->putbox_img(bar_bot_,0,bar_->height() - bar_bot_->height());
      bar_bot_->draw(0,bar_->height() - bar_bot_->height(),NULL,bar_);

      //bar_->putbox_img(bar_mid_,0,bar_top_->height());
      bar_mid_->draw(0,bar_top_->height(),NULL,bar_);
    }
  bar_brightness_->brightness(*bar_,WIN_BRIGHTNESS_LEVEL);
}


void win_scrollbar::draw(drawing_area * da)
{
  if(!visible_ || !back_draw_ || !bar_draw_) return; 
  
  back_draw_->draw(wsc_->real_x() + wsc_->length() - back_->length(), wsc_->real_y() , da );
  
  bar_draw_->draw(1 + wsc_->real_x() + wsc_->length() - back_->length(), wsc_->real_y() + wsc_->cursor_y() , da);
}


















