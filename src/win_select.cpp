/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT AN Y WARRANTY.

   See the COPYING file for more details
*/

#include <list>
#include "types.h"
#include "input.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_scrollbar.h"
#include "win_theme.h"
#include "win_container.h"
#include "win_scrolled.h"
#include "win_select.h"

//STATIC INIT
win_select * win_select::curselect_=NULL;
bool win_select::activate_keyboard_=true;

SDLKey win_select::next_key=WIN_SELECT_DEFAULT_KEY_NEXT;
SDLKey win_select::previous_key=WIN_SELECT_DEFAULT_KEY_PREVIOUS;
SDLKey win_select::activate_key=WIN_SELECT_DEFAULT_KEY_ACTIVATE;
SDLKey win_select::back_key=WIN_SELECT_DEFAULT_KEY_BACK;


win_select::win_select(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth):win_scrolled(tx,ty,tl,th,wth)
{

  type_obj_=WIN_OBJ_SELECT;//WARNING 
  //father select is the father of this object 
  fatherselect_=NULL;
  //if there are no select object attached to the static curselet, this object become the curselect
  if(!curselect_) curselect_=this;
  
  index_list=list_obj.begin();
  //the default mode select for this object is the border
  mode_selected_=WIN_SELECT_MODE_BORDER;
  //by default, the scroll bar is not visible
  visible_scrollbar_=false;
  //if when you select you change the first to the last
  select_circle_=false;
}


win_select::~win_select()
{
  //WARNING CHECK BACK
  if(curselect_==this) back();

  //WARNING DESTROY()
}


void win_select::add(win_base * wb)
{
  //call the add of wni_scrolled
  win_scrolled::add(wb);
  //prevent the new object is in select
  wb->set_in_select(true);
  //when you add an object, automatically the index become the first element of the list
  index_list=list_obj.begin();
  
  //a select can have just one mode to select each object in the list
  wb->set_select_mode_(mode_selected_);
}

void win_select::add(win_select * ws)
{
  //add a select object
  win_select::add((win_base*)ws);
  //add the object but prevent the new object that this objetc is the father
  ws->fatherselect_=this;
}

void win_select::remove(win_base * wb)
{
  //remove an objetc prevent that the object is not in_select
  win_scrolled::remove(wb);
  wb->set_in_select(false);
  wb->set_draw_brightness(false);
  index_list=list_obj.begin();
}

void win_select::remove_all()
{
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    {
      (*i)->set_in_select(false);
      (*i)->wb_father_=NULL;//part of win_container
    }
  list_obj.clear(); 
  index_list=list_obj.begin();
  //win scrolled part
  max_amplitude_=0;
  cur_amplitude_=0;
  theme_->scrollbar->update_scroll_bar(this);
}

void win_select::destroy()
{
  win_scrolled::destroy();
  index_list=list_obj.begin();
}


void win_select::on_next()
{
  if(callback_[WIN_SIG_NEXT_KEY]) (callback_[WIN_SIG_NEXT_KEY])();
}

void win_select::on_previous()
{
  if(callback_[WIN_SIG_PREVIOUS_KEY]) (callback_[WIN_SIG_PREVIOUS_KEY])();
}

void win_select::update()
{
  win_scrolled::update();
  if(activate_keyboard_ && activated_)
    {
      if(input::has_been_pushed(next_key)) next_();
      if(input::has_been_pushed(previous_key)) previous_();
      if(input::has_been_pushed(back_key)) back();
      if(input::has_been_pushed(activate_key)) activate___();
    } 
}



//WARNING: lots of confuse in next_ and previous maybe rewrite them
void win_select::next_()
{
  if(index_list==list_obj.end() || !activated_) return;
  (*index_list)->on_unselect();
  if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
    (*index_list)->set_draw_brightness(true);
  index_list++;
  
  //if(index_list==list_obj.end()) index_list--;
  if(index_list==list_obj.end()) 
    if(select_circle_) index_list=list_obj.begin();
    else index_list--;

  (*index_list)->on_select();
  (*index_list)->set_draw_brightness(false);
  update_position();
  on_next();
}

void win_select::previous_()
{
  if(index_list==list_obj.end() || !activated_) return;
  (*index_list)->on_unselect();
  if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
    (*index_list)->set_draw_brightness(true);
  //if(index_list!=list_obj.begin()) index_list--;
  if(index_list!=list_obj.begin()) index_list--;
  else if(select_circle_) 
    {

      index_list=list_obj.end();
      index_list--;
    }
  (*index_list)->on_select();
  (*index_list)->set_draw_brightness(false);
  update_position();
  on_previous();
}


void win_select::update_position()
{
  //this function is used to see the cur object which is selected
  static bool tmp_is_visible_;
  tmp_is_visible_=false;
  //if not amplitude --> all object in the list is visible
  if(!max_amplitude_) {tmp_is_visible_=true;return;}
  while(!tmp_is_visible_)
    {
      if(((*index_list)->y()+(*index_list)->pady()>space_between_border_) && 
	 ((*index_list)->y()+(*index_list)->height()<height_-space_between_border_)) tmp_is_visible_=true;
      else if((*index_list)->y()+(*index_list)->pady()<space_between_border_) up();
      else if((*index_list)->y()+(*index_list)->pady()+(*index_list)->height()>height_-space_between_border_) down();
      else tmp_is_visible_=true;
    }
}

//return the cur object which is selected
win_base * win_select::get()
{
  if(index_list==list_obj.end()) return NULL;
  else return (*index_list);
}

//if error return 0
u_int16 win_select::get_pos()
{
  u_int16 pos_=0;
  list<win_base *>::iterator i=list_obj.begin();
  if(i==list_obj.end()) return 0;
  while((*i++)!=(*index_list)) pos_++;
  return pos_+1;
}

void win_select::set_default(u_int16 nb)
{
  if(index_list==list_obj.end()) return;
  u_int16 j=1;
  (*index_list)->set_select(false);
  if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
    (*index_list)->set_draw_brightness(true);
  index_list=list_obj.begin();
  while(index_list!=list_obj.end() && j<nb) index_list++;
  if(index_list!=list_obj.end()) 
    {
      (*index_list)->set_select(true);
      if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
	(*index_list)->set_draw_brightness(false);
    }
}

void win_select::set_default(win_base * wb)
{
  if(index_list==list_obj.end()) return;
  (*index_list)->set_select(false);
  if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
    (*index_list)->set_draw_brightness(true);
  index_list=list_obj.begin();
  while(index_list != list_obj.end() && (*index_list)!=wb) index_list++;
  if(index_list!=list_obj.end()) 
    {
      (*index_list)->set_select(true);
      if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
	(*index_list)->set_draw_brightness(false);
    }
}


void win_select::set_select_mode(u_int8 mode)
{
  mode_selected_=mode;
  if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
    for(list<win_base*>::iterator i=list_obj.begin();i!=list_obj.end();i++)
      (*i)->set_draw_brightness(true);
  if(index_list!=list_obj.end()) (*index_list)->set_draw_brightness(false);
  for(list<win_base*>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->set_select_mode_(mode);
}

void win_select::activate()
{
  if(curselect_) curselect_->activate___();
}

void win_select::next()
{
  if(curselect_) curselect_->next_();
}

void win_select::previous()
{
  if(curselect_) curselect_->previous_();
}

bool win_select::activate___()
{
  if(index_list!=list_obj.end()) 
    {
      (*index_list)->on_activate();
      on_activate_key();
      return true;
    }
  return false;
}


void win_select::on_activate()
{
  win_scrolled::on_activate();
  if(!curselect_) {curselect_=this;return;}
  if(curselect_==this) return;
  //if curselect
  curselect_->on_unactivate();
  curselect_=this;
}

bool win_select::back()
{
  //if we can go to the father
  if(curselect_)
    {
      if(curselect_->fatherselect_)
	{
	  if(curselect_->index_list!=curselect_->list_obj.end())
	    {
	      curselect_->on_unactivate();
	      curselect_=curselect_->fatherselect_;//(win_select*)(*(curselect_->index_list));
	      curselect_->on_activate();
	      return true;
	    }
	}
    }
  return false;
}




/*
void win_select::on_activate()
{
  win_scrolled::on_activate();
  if(curselect_ && curselect_!=this)
    {
      curselect_->on_unactivate();
      curselect_=(win_select*)(*(curselect_->index_list));
      curselect_->on_activate();
    } 
  else curselect_=this;
}
*/
/*
void win_select::back()
{
  if(curselect_)
    {
      if(curselect_->fatherselect_)
	{
	  curselect_->on_unactivate();
	  curselect_=curselect_->fatherselect_;
	  curselect_->on_activate();
	}
      else curselect_=NULL;
    }  
}
*/

void win_select::set_cur_select(win_select * ws)
{
  //WARNING if cur select is not null
  curselect_=ws;
}

void win_select::init()
{
  curselect_=NULL;
}















