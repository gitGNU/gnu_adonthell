/*
   $Id$
   
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT AN Y WARRANTY.

   See the COPYING file for more details
*/

#include "select_list.h"
#include "types.h"

using namespace gui;

select_list::select_list()
{
  cur_select_ = list_wb_.begin();
  set_mode(MODE_BRIGHTNESS);
  border_select_ = NULL;
  set_circle(false);
  finish_scroll_ = true;
}

void select_list::add(base * w)
{
  scroll::add(w);
  cur_select_ = list_wb_.begin (); 
  //if select has a border for the selection, just set border to the base added
  if(border_select_ != NULL) w->set_border(*border_select_);
  //set the object to unselect
  rules(false, w);
  set_default();
}


void select_list::remove(base * w)
{
  rules(false, w);
  scroll::remove(w);
  set_default();
}


void select_list::remove_all()
{
 scroll::remove_all();
 cur_select_ = list_wb_.begin();
}


void select_list::next()
{
  //test if next possible
  if(cur_select_ == list_wb_.end() || list_wb_.size() == 0) return;
  //  audio::play_wave (-1, 1); 
  (*cur_select_)->on_unselect();
  
  //unselect cur element
  rules(false,*cur_select_);
  (*cur_select_)->set_activate (false);
  
  //create a temporary index
  lwb :: iterator i = cur_select_;
  
  //go next
  i++;
  
  //while not a the end, not be selected and different at old object go to the next
  while( i != list_wb_.end() && !(*i)->is_can_be_selected() && i != cur_select_) i++; 
  
  //if at end of list and select circle is activate
  if(i == list_wb_.end())
    {
      //cur is the begin of list
      if( circle_ )
	{
	  i = list_wb_.begin();
	  while(i != list_wb_.end() && !(*i)->is_can_be_selected() && i != cur_select_) i++;
	  if(i != list_wb_.end()) cur_select_ = i; 
	}
    }else cur_select_ = i;
  
  rules(true,*cur_select_);
  
  (*cur_select_)->on_select();
  
  on_next();
  
  finish_scroll_ =false;
}


void select_list::previous()
{
  if(cur_select_==list_wb_.end() || list_wb_.size() == 0) return;
  
  //  audio::play_wave (-1, 1); 
  (*cur_select_)->on_unselect();
  //set to unselect object
  rules(false,*cur_select_);
  (*cur_select_)->set_activate (false);
  lwb::iterator i=cur_select_;
  
  if(circle_)
    {
      if(i==list_wb_.begin()) i=list_wb_.end();
      i--;
    }
  else if(i!=list_wb_.begin()) i--;
  
  while(i != list_wb_.begin() && !(*i)->is_can_be_selected() && i != cur_select_) i--; 
  
  if( i== list_wb_.begin() && !(*i)->is_can_be_selected())
    {
      if(circle_)
	{
	  i = list_wb_.end();
	  i--;
	  while(i!=list_wb_.begin() && !(*i)->is_can_be_selected() && i!=cur_select_) i--;
	  if((*i)->is_can_be_selected()) cur_select_=i;
	}
    } else cur_select_=i;
 
  (*cur_select_)->on_select();
  
  //set to select object
  rules(true,*cur_select_);
  
  //update_position();
  on_previous();

  finish_scroll_ =false;
  //update_cur_select_position();
}


void select_list::activate()
{
  if(cur_select_ == list_wb_.end()) return;
  
  //audio::play_wave (-1, 0); 
  
  //set_activate(false);  
  
  (*cur_select_)->set_activate(true);
  
  on_activate_key(); 
}


bool select_list::input_update()
{
  
  if(scroll::input_update())
    {
      if(focus_object_) return true;
      
      if(input::has_been_pushed(keys::KEY_NEXT)) next();
      if(input::has_been_pushed(keys::KEY_PREVIOUS)) previous();
      if(input::has_been_pushed(keys::KEY_ACTIVATE) ||
         input::has_been_pushed(keys::KEY_ACTIVATE_ENTRY)) activate();
      
      return true;
    }
  return false;
}


void select_list::rules(const bool b, base * wb)
{
  if(!wb->is_can_be_selected()) return;
  
  switch(mode_)
    {
    case MODE_BORDER:
      wb->set_visible_border(b);
      break;
      
    case MODE_BRIGHTNESS:
      wb->set_brightness(!b);
      break;
    }
}

void select_list::set_default()
{
  if(list_wb_.size() == 0) return;

  if(cur_select_ != list_wb_.end())
  {
      rules(false,*cur_select_);
  }
  
  cur_select_ = list_wb_.begin();

  while(cur_select_ != list_wb_.end() && !(*cur_select_)->is_can_be_selected()) cur_select_++;
  
  if(cur_select_ != list_wb_.end()) rules(true,*cur_select_);

  finish_scroll_ =false;
}

void select_list::set_default_object(const base * wb)
{
  if(list_wb_.size() == 0) return;
  
  if(cur_select_ != list_wb_.end()) rules(false,*cur_select_);

  cur_select_ = list_wb_.begin();
  
  while(cur_select_ != list_wb_.end() && *cur_select_ != wb) cur_select_++;

  if(cur_select_ != list_wb_.end()) rules(true,*cur_select_);

  finish_scroll_ =false;
  //update_cur_select_position();
}


void select_list::set_default_position(const u_int16 pos)
{
  if(list_wb_.size() == 0 || pos > list_wb_.size()) return;
  
  if(cur_select_ != list_wb_.end()) rules(false,*cur_select_);

  cur_select_ = list_wb_.begin();

  u_int16 i = 0;

  while(cur_select_ != list_wb_.end() && i++ < pos) cur_select_++;

  if(cur_select_ != list_wb_.end()) rules(true,*cur_select_);

  finish_scroll_ =false;
  //update_cur_select_position();
}


base * select_list::get_selected_object()
{
  if(cur_select_ != list_wb_.end() || list_wb_.size() == 0) return *cur_select_;
  else return NULL;
}

u_int16 select_list::get_selected_position()
{
  u_int16 pos_=0;
 
  lwb::iterator i = list_wb_.begin();
  
  if(i==list_wb_.end()) return 0;
  
  while( (*i++) != (*cur_select_)) pos_++;
  
  return pos_+1;
}

void select_list::update_cur_select_position()
{
  if(!max_amplitude_) return;

  while( 1 )
    { 
        //       if((*cur_select_)->height() > height() + (space_with_border_ << 1) ) break;
        // Workaround if the object is bigger than the select (Alex).
      if((*cur_select_)->height() + (space_with_border_ << 1) > height()) break;
      else if((*cur_select_)->y() + (*cur_select_)->pad_y() < space_with_border_ ) up();
      else if((*cur_select_)->y() + (*cur_select_)->pad_y() + (*cur_select_)->height() > height() - space_with_border_) down();
      else break;
        }
}

bool select_list::update()
{
  if(scroll::update())
    {
      if(!finish_scroll_)
	{
	  if(!max_amplitude_) {finish_scroll_=true;return true;}
	  if((*cur_select_)->height() + (space_with_border_ << 1) > height()) {finish_scroll_ = true; return true;}
	  else if((*cur_select_)->y() + (*cur_select_)->pad_y() < space_with_border_ ) up();
	  else if((*cur_select_)->y() + (*cur_select_)->pad_y() + (*cur_select_)->height() > height() - space_with_border_) down();
	  else finish_scroll_ = true;
	}
       return true;
    }
  return false;
}
