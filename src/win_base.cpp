/*   
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.
   
   See the COPYING file for more details
*/

#include <iostream.h>
#include <string.h>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_theme.h"
#include "win_border.h"
#include "win_background.h"
#include "win_base.h"

win_base::win_base(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth)
{
  activated_=false;
  selected_=false;
  in_select_=false;
  visible_=false;
  visible_border_=false;
  visible_background_=false;
  draw_brightness_=false;
  type_obj_=WIN_OBJ_BASE;
  wb_father_=NULL;
  level_trans_back_=180;
  level_brightness_=200;
  mode_select_=WIN_SELECT_MODE_BORDER;
  border_size_=WIN_SIZE_NORMAL;
  padx_=0;
  pady_=0;
  x_=tx;
  y_=ty;
  pda_=NULL;
  da_=new drawing_area();
  if(wth) theme_=new win_theme(*wth);
  else theme_=NULL;
  resize(tl,th);
  update_real_position();
}

win_base::~win_base()
{
  delete da_;
  delete theme_;
}

void win_base::resize(u_int16 tl,u_int16 th)
{
  length_=tl;
  height_=th;
  da_->w=tl;
  da_->h=th;
  if(theme_)theme_->update(this);  
}

void win_base::update()
{
  on_update();
}

void win_base::draw()
{
  on_draw();
}

void win_base::set_theme(win_theme * th)
{
  if(th) *theme_=*th;
  else theme_=NULL;
}

void win_base::move(s_int16 tx,s_int16 ty)
{
  x_=tx;y_=ty;
  update_real_position();
}

void win_base::update_drawing_area_position()
{
  da_->x=realx_;
  da_->y=realy_;
}

void win_base::update_real_position()
{
  if(wb_father_)
    {
      pda_=wb_father_->get_drawing_area();
      realx_=x_+wb_father_->realx()+padx_;
      realy_=y_+wb_father_->realy()+pady_;
    }
  else
    {
      pda_=NULL;
      realx_=x_+padx_;
      realy_=y_+pady_;
    }
  update_drawing_area_position();
}

void win_base::set_signal_connect(const Functor0 & func,u_int8 signal)
{
  callback_[signal]=func;
} 

void win_base::on_activate()
{
  activated_=true;
  if(callback_[WIN_SIG_ACTIVATE]) (callback_[WIN_SIG_ACTIVATE])();
}
 
void win_base::on_unactivate()
{
  activated_=false;
  if(callback_[WIN_SIG_UNACTIVATE]) (callback_[WIN_SIG_UNACTIVATE])();
}

void win_base::on_update()
{
  if(callback_[WIN_SIG_UPDATE]) (callback_[WIN_SIG_UPDATE])();
}

void win_base::on_draw()
{
  if(callback_[WIN_SIG_DRAW]) (callback_[WIN_SIG_DRAW])();
}

void win_base::on_select()
{
  selected_=true;
  if(callback_[WIN_SIG_SELECT]) (callback_[WIN_SIG_SELECT])();
}

void win_base::on_unselect()
{
  selected_=false;
  if(callback_[WIN_SIG_UNSELECT]) (callback_[WIN_SIG_UNSELECT])();
}
 
//this function is called when you push the activate key
void win_base::on_activate_key()
{
  if(callback_[WIN_SIG_ACTIVATE_KEY]) (callback_[WIN_SIG_ACTIVATE_KEY])();
}

/*******************************************************/
/************************ DRAW *************************/
/*******************************************************/

void win_base::draw_border()
{
#define WB_CORNER_MIDX (border->corner_top_left->length>>1)
#define WB_CORNER_MIDY (border->corner_top_left->height>>1)
#define WB_VBORDERX (border->v_border_template->length>>1)
#define WB_HBORDERY (border->h_border_template->height>>1) 
  if(!theme_) return;
  win_border * border=NULL;
  if(border_size_==WIN_SIZE_MINI) border=theme_->mini;
  else if(border_size_==WIN_SIZE_NORMAL) border=theme_->normal;
  
  if(in_select_)
    {
      if(!selected_)
	{
	  if(mode_select_==WIN_SELECT_MODE_BORDER) return;
	  else if(mode_select_==WIN_SELECT_MODE_BRIGHTNESS) draw_brightness_=true;
	}
      else
	{
	  if(mode_select_==WIN_SELECT_MODE_BORDER) border=theme_->mini;
	  else if(mode_select_==WIN_SELECT_MODE_BRIGHTNESS) 
	    {draw_brightness_=false;	    
	    }   
	}
    }
  else if(!visible_border_) return;

  static image imgbright;
  switch(draw_brightness_)
    {
    case true:
      imgbright.brightness(border->h_border,level_brightness_);
      imgbright.putbox_mask(realx_,realy_-border->h_border->height,pda_);
      imgbright.putbox_mask(realx_,+realy_+height_,pda_);
      imgbright.brightness(border->v_border,level_brightness_);
      imgbright.putbox_mask(realx_-border->v_border->length,realy_,pda_);
      imgbright.putbox_mask(realx_+length_,realy_,pda_);
      imgbright.brightness(border->corner_top_left,level_brightness_);
      imgbright.putbox_mask(realx_-WB_CORNER_MIDX-((border->v_border->length)>>1),
			    realy_-WB_CORNER_MIDY-(border->h_border->height>>1),pda_);   
      imgbright.brightness(border->corner_top_right,level_brightness_);
      imgbright.putbox_mask(realx_+length_-WB_CORNER_MIDX+(border->v_border->length>>1),
			    realy_-WB_CORNER_MIDY-(border->h_border->height>>1),pda_); 
      imgbright.brightness(border->corner_bottom_left,level_brightness_);
      imgbright.putbox_mask(realx_-WB_CORNER_MIDX-((border->v_border->length)>>1),
			    realy_+height_-WB_CORNER_MIDY+(border->h_border->height>>1),pda_);
      imgbright.brightness(border->corner_bottom_right,level_brightness_);
      imgbright.putbox_mask(realx_+length_-WB_CORNER_MIDX+(border->v_border->length>>1),
			    realy_+height_-WB_CORNER_MIDY+(border->h_border->height>>1),pda_);
      break;
    case false:
      border->h_border->putbox_mask(realx_,realy_-border->h_border->height,pda_);
      border->h_border->putbox_mask(realx_,+realy_+height_,pda_);
      border->v_border->putbox_mask(realx_-border->v_border->length,realy_,pda_);
      border->v_border->putbox_mask(realx_+length_,realy_,pda_);
      border->corner_top_left->putbox_mask(realx_-WB_CORNER_MIDX-((border->v_border->length)>>1),
					   realy_-WB_CORNER_MIDY-(border->h_border->height>>1),pda_);   
      border->corner_top_right->putbox_mask(realx_+length_-WB_CORNER_MIDX+(border->v_border->length>>1),
					    realy_-WB_CORNER_MIDY-(border->h_border->height>>1),pda_);
      border->corner_bottom_left->putbox_mask(realx_-WB_CORNER_MIDX-((border->v_border->length)>>1),
					      realy_+height_-WB_CORNER_MIDY+(border->h_border->height>>1),pda_); 
      border->corner_bottom_right->putbox_mask(realx_+length_-WB_CORNER_MIDX+(border->v_border->length>>1),
					       realy_+height_-WB_CORNER_MIDY+(border->h_border->height>>1),pda_);
      break;
    }
}

void win_base::draw_background()
{
  if(!theme_)return;
  static image imgbright;
  if(visible_background_ && in_select_ && (!selected_) && mode_select_==WIN_SELECT_MODE_BRIGHTNESS)
    {
      imgbright.brightness(theme_->background->background,level_brightness_);
      imgbright.putbox_trans(realx_,realy_,level_trans_back_,da_);
    }
  else if(visible_background_)theme_->background->background->putbox_trans(realx_,realy_,level_trans_back_,da_);
}










