/* 
   $Id$

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

  //by default window is desactivated
  activated_=false;
  //by default window is not selected
  selected_=false;
  //by default windows is not in select object
  in_select_=false;
  //by default window is not visible
  visible_=false;
  //by default border is no tvisible
  visible_border_=false;
  //by default backgroud is not visible
  visible_background_=false;
  //by default window is draw in brigthness
  draw_brightness_=false;
  //type of the object --->   WARNING I'must remove, find another solution, used to container....
  type_obj_=WIN_OBJ_BASE;
  //not have a father, -> window in(depend on) another window.
  wb_father_=NULL;
  //value used to transluency
  level_trans_back_=170;
  //value used to brightness
  level_brightness_=200;
  //by default the current object is selected by the border
  mode_select_=WIN_SELECT_MODE_BORDER;
  //by default border size used is normal size
  border_size_=WIN_SIZE_NORMAL;
  //padding X,Y
  padx_=0;
  pady_=0;
  //position of the windows, not real position, position in win_* into another win_container*
  x_=tx;
  y_=ty;
  //drawing area of the father object
  pda_=NULL;
  //drawing area of this object
  da_=new drawing_area();
  //create a theme for this object
  if(wth) theme_=new win_theme(*wth);
  else theme_=NULL;
  //resize the object
  resize(tl,th);
  //update the real position
  update_real_position();
}


win_base::~win_base()
{
  //delete cur drawing area
  delete da_;
  //delete theme
  delete theme_;
}

void win_base::resize(u_int16 tl,u_int16 th)
{
  //resize
  length_=tl;
  height_=th;
  //resize drawing area too.
  da_->w=tl;
  da_->h=th;
  //modify the theme object --> in theme object there are the border, background size  and another .....
  if(theme_)theme_->update(this);  
}


void win_base::update()
{
  //on update is called
  on_update();
}

void win_base::draw()
{
  //WARNING: i need create another signal to draw before
  on_draw();
  if(!visible_) return;
  //on draw is called is the object is visible
  ///on_draw();
}

void win_base::set_theme(win_theme * th)
{
  //set the new theme ---> WARNING I think theme->UPDATE(THIS) IS NEEDED
  if(th) *theme_=*th;
  else theme_=NULL;// ----> WARNING test if theme is not null before to do a null !!!!
}

void win_base::move(s_int16 tx,s_int16 ty)
{
  //move the position 
  x_=tx;y_=ty;
  //update position, if this object is a child
  update_real_position();
}

void win_base::update_drawing_area_position()
{
  //update drawing area to the real position
  da_->x=realx_;
  da_->y=realy_;
}

void win_base::update_real_position()
{
  
  if(wb_father_) //if the object is a child
    {
      //get reference to drawing area of the father
      pda_=wb_father_->get_drawing_area();
      //calcul the real position,---->> WARNING padx, pady maybe not add in realx_,realy_
      realx_=x_+wb_father_->realx()+padx_;
      realy_=y_+wb_father_->realy()+pady_;
    }
  else
    {
      //the object not a child
      pda_=NULL;
      realx_=x_+padx_; //----> Same than upper
      realy_=y_+pady_;
    }
  //update drawing area
  update_drawing_area_position();
}

//add a signal 
void win_base::set_signal_connect(const Functor0 & func,u_int8 signal)
{
  callback_[signal]=func;
} 

//on activate 
void win_base::on_activate()
{
  //activated become true 
  activated_=true;
  //if the activated signal is define execute the function
  if(callback_[WIN_SIG_ACTIVATE]) (callback_[WIN_SIG_ACTIVATE])();
}
 
void win_base::on_unactivate()
{
  //activated become false
  activated_=false;
  //call,if define the unactivate function
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
 
//this function is called when you push the activate key ---> WARNING i don't know the real use
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
  
  //if no theme, drawing nothing
  if(!theme_) return;
  //create a temp border
  win_border * border=NULL;
  //if border size is mini, border take the minborder of the theme
  if(border_size_==WIN_SIZE_MINI) border=theme_->mini;
  else if(border_size_==WIN_SIZE_NORMAL) border=theme_->normal;
 
  //if this object is in a select object 
  if(in_select_)
    {
      //if in select object but not selected
      if(!selected_)
	{
	  //if curmode of the select is win_border so draw nothing
	  if(mode_select_==WIN_SELECT_MODE_BORDER)
	  {
	    visible_border_=false;
	    return;
      }
	  //if mode is brightness draw the border on brigthness mode
	  else if(mode_select_==WIN_SELECT_MODE_BRIGHTNESS) draw_brightness_=true;
	}
      else
	{
	  //if the object is in select and the object is selected
	  //if selection is border ----> WARNING  I MUST CHECK WHY theme_->mini, why not use the cur size of border ????
	  if(mode_select_==WIN_SELECT_MODE_BORDER)
	  { 
        visible_border_=true;
	    border=theme_->mini;
      }
	  //if mode is brightness this object must not drawing with brightness
	  else if(mode_select_==WIN_SELECT_MODE_BRIGHTNESS) 
	    {draw_brightness_=false;	    
	    }   
	}
    }

  //if the is not in win_select and i not visible return 
  if(!visible_border_) return;

  //create a static image to draw border
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
  //if not theme return
  if(!theme_)return;
  static image imgbright;

  //if is a visible background and if in win_select and this object is not select and mode select is brigthness so drawthe background in brightness mode
  if(visible_background_ && in_select_ && (!selected_) && mode_select_==WIN_SELECT_MODE_BRIGHTNESS)
    {
      imgbright.brightness(theme_->background->background,level_brightness_);
      imgbright.putbox_trans(realx_,realy_,level_trans_back_,da_);
    }
  //draw the background
  else if(visible_background_)theme_->background->background->putbox_trans(realx_,realy_,level_trans_back_,da_);
}










