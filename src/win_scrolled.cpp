/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_scrollbar.h"
#include "win_theme.h"
#include "win_container.h"
#include "win_scrolled.h"


win_scrolled::win_scrolled(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth):win_container(tx,ty,tl,th,wth)
{
  //set the scrollbar to visible
  visible_scrollbar_=true;
  //set the position of the cursor zero
  cursory_=0;
  //ther max amplitude is 0 and cur too.
  max_amplitude_=0;
  cur_amplitude_=0;
  //index pad is the value to move the cursor
  index_pad_=WIN_SCROLLBAR_PAD_DEFAULT;
  //typ of object --> WARNING REMOVE THIS
  type_obj_=WIN_OBJ_SCROLLED;
}

win_scrolled::~win_scrolled()
{
  
}

void win_scrolled::on_up()
{
  //signal called on up if up is ok
  if(callback_[WIN_SIG_SCROLL_UP]) callback_[WIN_SIG_SCROLL_UP]();
}

void win_scrolled::on_down()
{
  //signal called on down if down is ok
  if(callback_[WIN_SIG_SCROLL_DOWN]) callback_[WIN_SIG_SCROLL_DOWN]();
}


bool win_scrolled::down()
{
  
  if(max_amplitude_==0 || !activated_) return false;
  if(cur_amplitude_+index_pad_>max_amplitude_)
    cur_amplitude_=max_amplitude_;
  else cur_amplitude_+=index_pad_;
  update_amplitude();
  on_down();
  return true;
}

void win_scrolled::resize(u_int16 tl,u_int16 th)
{
  win_container::resize(tl,th);
  find_amplitude();
  theme_->scrollbar->update_scroll_bar(this); //WARNING --> maybe on update theme to update scrollbar
}


bool win_scrolled::up()
{
  if(max_amplitude_==0 || !activated_) return false ;
  if(cur_amplitude_-index_pad_<0)
    cur_amplitude_=0;
  else cur_amplitude_-=index_pad_;
  update_amplitude();
  on_up();
  return true;
}

void win_scrolled::update_amplitude()
{
  //move all object with the variation of cur_amplitude
  for(list<win_base*>::iterator i=list_obj.begin();i!=list_obj.end();i++) 
    {
      (*i)->pady()=-cur_amplitude_;  //WARNING: like win_base maybe pady not needed to be add in realy
      (*i)->update_real_position();
    }
  //position the cursor
  cursory_=(u_int16)((float)((float)(height_-theme_->scrollbar->bar->height)/max_amplitude_)*cur_amplitude_);
}


void win_scrolled::set_space_between_border(u_int16 tmp)
{
  
  win_container::set_space_between_border(tmp);
  find_amplitude();
  theme_->scrollbar->update_scroll_bar(this);
}

void win_scrolled::set_space_between_object(u_int16 tmp)
{
  win_container::set_space_between_object(tmp);
  find_amplitude();
  theme_->scrollbar->update_scroll_bar(this);
}

void win_scrolled::add(win_base *wb)
{
  //add a new object 
  win_container::add(wb);
  //check the max y object
  find_amplitude();
  //update theme bar, jyust theme bar, is not necessary to update all the theme :)
  theme_->scrollbar->update_scroll_bar(this);
}

void win_scrolled::remove(win_base *wb)
{
  win_container::remove(wb);
  find_amplitude();
  theme_->scrollbar->update_scroll_bar(this);
}

void win_scrolled::remove_all()
{
  win_container::remove_all();
  max_amplitude_=0;
  cur_amplitude_=0;
  theme_->scrollbar->update_scroll_bar(this);
}

void win_scrolled::destroy()
{
  win_container::destroy();
  max_amplitude_=0;
  cur_amplitude_=0;
  theme_->scrollbar->update_scroll_bar(this);
}

void win_scrolled::draw()
{
  win_container::draw(); //WARNING !!!!

  if(!visible_)return;
 
  //return if scrollbar not visible 
  if(!visible_scrollbar_) return;
  if((in_select_ && !selected_) || draw_brightness_)
    {
      image img;
      img.brightness(theme_->scrollbar->back,level_brightness_);
      img.putbox_mask(realx_ + length_ - theme_->scrollbar->back->length,realy_,da_);
      img.brightness(theme_->scrollbar->bar,level_brightness_);
      img.putbox_mask(1 + realx_ + length_ - theme_->scrollbar->back->length,realy_+cursory_,da_);
    }
  else
    {
      theme_->scrollbar->back->putbox_mask(realx_ + length_ - theme_->scrollbar->back->length,realy_,da_);
      theme_->scrollbar->bar->putbox_mask(1 + realx_ + length_ - theme_->scrollbar->back->length,realy_+cursory_,da_);
    }
}

void win_scrolled::update()
{
  win_container::update();
}

void win_scrolled::find_amplitude()
{
  //search the max y object to calcul the amplitude
  max_amplitude_=cursory_=cur_amplitude_=0;
  for(list<win_base*>::iterator i=list_obj.begin();i!=list_obj.end();i++) 
    if((*i)->y()+(*i)->height()>height_-space_between_border_ && (*i)->y()+(*i)->height()-height_+space_between_border_>max_amplitude_)
      max_amplitude_=(*i)->y()+(*i)->height()-height_+space_between_border_; 
}










