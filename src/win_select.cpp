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

  //type of the select
  type_selected_=WIN_SELECT_TYPE_NORMAL;

}


win_select::~win_select()
{
  //WARNING CHECK BACK
  if(curselect_==this) 
    {
      back();
      curselect_=NULL;
    }
  //WARNING DESTROY()
}


void win_select::add(win_base * wb)
{
  //call the add of win_scrolled
  win_scrolled::add(wb);

  //prevent the new object is in select
  wb->set_in_select(true);
  
  //set the parameters for this object
  set_select_object(wb,false);

  //when you add an object, automatically the index become the first element of the list
  set_default();

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

bool win_select::update()
{
  if(win_scrolled::update())
    {
      if(focus_ && activate_keyboard_ && activated_)
	{
	  if(input::has_been_pushed(next_key)) next_();
	  if(input::has_been_pushed(previous_key)) previous_();
	  if(input::has_been_pushed(back_key)) back();
	  if(input::has_been_pushed(activate_key)) activate___();
	} 
      return true;
    }
  return false;
}

//if b is true, cur object take parameters to an selected object
void win_select::set_select_object(win_base * wb,bool b)
{
  switch(mode_selected_)
    {
    case WIN_SELECT_MODE_BRIGHTNESS:
      wb->set_draw_brightness(!b);
      break;
    case WIN_SELECT_MODE_BORDER:
      wb->set_border_visible(b);
    }

  switch(type_selected_)
    {
    case WIN_SELECT_TYPE_NORMAL:
      wb->set_visible(true);
      break;
    case WIN_SELECT_TYPE_SCROLL:
      wb->set_visible(b);
      break;
    }
}


//WARNING: lots of confuse in next_ and previous maybe rewrite them
void win_select::next_()
{
  //test if next is possible 
  if(index_list==list_obj.end() || !activated_) return;

  //unselect cur element
  (*index_list)->on_unselect();

  set_select_object(*index_list,false);

  //create a temporary index
  list<win_base*>::iterator cur_i=index_list;
  
  //to next elements
  cur_i++;

  //while not a the end, not be selected and different at old object go to the next
  while(cur_i!=list_obj.end() && !(*cur_i)->is_can_be_selected() && cur_i!=index_list) cur_i++; 
  
  //if at end of list and select circle is activate
  if(cur_i==list_obj.end())
    {
      //cur is the begin of list
      if(select_circle_)
	{
	  cur_i=list_obj.begin();
	  while(cur_i!=list_obj.end() && !(*cur_i)->is_can_be_selected() && cur_i!=index_list) cur_i++;
	  if(cur_i!=list_obj.end()) index_list=cur_i; 
	}
    }else index_list=cur_i;
  
  set_select_object(*index_list,true);
  
  (*index_list)->on_select();

  update_position();
  
  on_next();
}

void win_select::previous_()
{
  if(index_list==list_obj.end() || !activated_) return;
  
  (*index_list)->on_unselect();
  
  //set to unselect object
  set_select_object(*index_list,false);
  
  list<win_base*>::iterator cur_i=index_list;
  if(cur_i==list_obj.begin()) cur_i=list_obj.end();
  
  cur_i--;
   
  while(cur_i!=list_obj.begin() && !(*cur_i)->is_can_be_selected() && cur_i!=index_list) cur_i--; 

  if(cur_i==list_obj.begin() && !(*cur_i)->is_can_be_selected())
    {
      if(select_circle_)
	{
	  cur_i=list_obj.end();
	  cur_i--;
	  
	  while(cur_i!=list_obj.begin() && !(*cur_i)->is_can_be_selected() && cur_i!=index_list) cur_i--;
	  if((*cur_i)->is_can_be_selected()) index_list=cur_i;
	}
    } else index_list=cur_i;
  
  (*index_list)->on_select();
  
  //set to select object
  set_select_object(*index_list,true);
 
  update_position();
  
  on_previous();
}


void win_select::set_can_be_selected_all(bool b)
{
  for(list<win_base*>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->set_can_be_selected(b);
}

void win_select::set_default()
{
  if(list_obj.size()==0) return;

  if(index_list!=list_obj.end()) set_select_object(*index_list,false);

  index_list=list_obj.begin();

  while(index_list!=list_obj.end() && !(*index_list)->is_can_be_selected()) index_list++;
  
  if(index_list!=list_obj.end()) set_select_object(*index_list,true);
}


void win_select::update_position()
{

  //this function is used to see the cur object which is selected
  static bool tmp_is_visible_;

  switch(type_selected_)
    {
    case WIN_SELECT_TYPE_NORMAL:
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
      break;

    case WIN_SELECT_TYPE_SCROLL:
      //set position of the cur select object
      
      (*index_list)->move(space_between_border_,space_between_border_);
      
      break;
    default:
      break;
    }
}


void win_select::set_type(u_int8 t)
{
  type_selected_=t;
  //set all object to the new type set
  for(list<win_base*>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    set_select_object(*i,false);
  
  //select the first object
  set_default();
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
  else if(mode_selected_==WIN_SELECT_MODE_BORDER)
    (*index_list)->set_border_visible(false);

  index_list=list_obj.begin();
  while(index_list!=list_obj.end() && j++<nb) index_list++;
  if(index_list!=list_obj.end()) 
    {
      (*index_list)->set_select(true);
      if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
	(*index_list)->set_draw_brightness(false);
      else if(mode_selected_==WIN_SELECT_MODE_BORDER)
	(*index_list)->set_border_visible(true);
    }
}

void win_select::set_default(win_base * wb)
{
  if(index_list==list_obj.end()) return;
  (*index_list)->set_select(false);

  if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
    (*index_list)->set_draw_brightness(true);
  else if(mode_selected_==WIN_SELECT_MODE_BORDER)
    (*index_list)->set_border_visible(false);

  index_list=list_obj.begin();
  while(index_list != list_obj.end() && (*index_list)!=wb) index_list++;
  if(index_list!=list_obj.end()) 
    {
      (*index_list)->set_select(true);
      if(mode_selected_==WIN_SELECT_MODE_BRIGHTNESS)
	(*index_list)->set_draw_brightness(false);
      else if(mode_selected_==WIN_SELECT_MODE_BORDER)
	(*index_list)->set_border_visible(true);
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
      (*index_list)->set_activated(true);
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
  //if true we can go to the father
  if(curselect_)
    {
      if(curselect_->fatherselect_)
	{
	  if(curselect_->index_list!=curselect_->list_obj.end())
	    {
	      curselect_->on_unactivate();
	      curselect_=curselect_->fatherselect_;
	      curselect_->on_activate();
	      return true;
	    }
	}
    }
  return false;
}

void win_select::set_cur_select(win_select * ws)
{
  if(curselect_ && curselect_!=ws) curselect_->set_activated(false);
  curselect_=ws;
}

void win_select::init()
{
  if(curselect_) curselect_->set_activated(false);
  curselect_=NULL;
}



















