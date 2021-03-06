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

#include "win_select.h"

#include "audio.h"

win_select::win_select()
{
  cur_select_ = list_wb_.begin();
  
  set_mode(MODE_BRIGHTNESS);
  
  border_select_ = NULL;

  set_circle(false);

  finish_scroll_ = true;
  //set_visible_scrollbar(false);
}


void win_select::add(win_base * w)
{
    win_scroll::add(w);
    cur_select_ = list_wb_.begin (); 
    //if select has a border for the selection, just set border to the win_base added
    if(border_select_ != NULL) w->set_border(*border_select_);
    
    //set the object to unselect
    rules(false, w);
    
    set_default();
    
    //update_cur_select_position();
}


void win_select::remove(win_base * w)
{
  rules(false, w);
  
  win_scroll::remove(w);
  
  set_default();
}


void win_select::remove_all()
{
 win_scroll::remove_all();
 cur_select_ = list_wb_.begin();
}


void win_select::next()
{
  //test if next possible
  if(cur_select_ == list_wb_.end() || list_wb_.size() == 0) return;

  audio::play_wave (-1, 1); 
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

  //update_cur_select_position();
}


void win_select::previous()
{
  if(cur_select_==list_wb_.end() || list_wb_.size() == 0) return;
  
  audio::play_wave (-1, 1); 
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


void win_select::activate()
{
  if(cur_select_ == list_wb_.end()) return;
  
  audio::play_wave (-1, 0); 
  
  //set_activate(false);  
  
  (*cur_select_)->set_activate(true);
  
  on_activate_key(); 
}


bool win_select::input_update()
{
  
  if(win_scroll::input_update())
    {
      if(focus_object_) return true;
      
      if(input::has_been_pushed(win_keys::KEY_NEXT)) next();
      if(input::has_been_pushed(win_keys::KEY_PREVIOUS)) previous();
      if(input::has_been_pushed(win_keys::KEY_ACTIVATE) ||
         input::has_been_pushed(win_keys::KEY_ACTIVATE_ENTRY)) activate();
      
      return true;
    }
  return false;
}

void win_select::rules(const bool b, win_base * wb)
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


void win_select::set_default()
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

void win_select::set_default_object(const win_base * wb)
{
  if(list_wb_.size() == 0) return;
  
  if(cur_select_ != list_wb_.end()) rules(false,*cur_select_);

  cur_select_ = list_wb_.begin();
  
  while(cur_select_ != list_wb_.end() && *cur_select_ != wb) cur_select_++;

  if(cur_select_ != list_wb_.end()) rules(true,*cur_select_);

  finish_scroll_ =false;
  //update_cur_select_position();
}


void win_select::set_default_position(const u_int16 pos)
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


win_base * win_select::get_selected_object()
{
  if(cur_select_ != list_wb_.end() || list_wb_.size() == 0) return *cur_select_;
  else return NULL;
}

u_int16 win_select::get_selected_position()
{
  u_int16 pos_=0;
 
  lwb::iterator i = list_wb_.begin();
  
  if(i==list_wb_.end()) return 0;
  
  while( (*i++) != (*cur_select_)) pos_++;
  
  return pos_+1;
}



void win_select::update_cur_select_position()
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


bool win_select::update()
{
  if(win_scroll::update())
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



/*
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

s_int32 win_select::next_key=WIN_SELECT_DEFAULT_KEY_NEXT;
s_int32 win_select::previous_key=WIN_SELECT_DEFAULT_KEY_PREVIOUS;
s_int32 win_select::activate_key=WIN_SELECT_DEFAULT_KEY_ACTIVATE;
s_int32 win_select::back_key=WIN_SELECT_DEFAULT_KEY_BACK;


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
	  if(input::has_been_pushed (SDL_Keycode (next_key))) next_();
	  if(input::has_been_pushed (SDL_Keycode (previous_key))) previous_();
	  if(input::has_been_pushed (SDL_Keycode (back_key))) back();
	  if(input::has_been_pushed (SDL_Keycode (activate_key))) activate___();
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

  
      
  if(select_circle_)
    {
     
      if(cur_i==list_obj.begin()) cur_i=list_obj.end();
      cur_i--;
    }
  else if(cur_i!=list_obj.begin()) cur_i--;
    



   


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

  //if(index_list==list_obj.end()){cout << "tets\n"; return;}

  switch(type_selected_)
    {
    case WIN_SELECT_TYPE_NORMAL:
      tmp_is_visible_=false;
      //if not amplitude --> all object in the list is visible
      if(!max_amplitude_) {tmp_is_visible_=true;return;}
      while(!tmp_is_visible_)
	{
	 
	  // if(((*index_list)->y()+(*index_list)->pady()>space_between_border_) && 
	  //   ((*index_list)->y()+(*index_list)->height()>height_-space_between_border_)) {tmp_is_visible_=true;cout << "16\n";}
	  if((*index_list)->height()>height_+(space_between_border_<<1)) tmp_is_visible_=true;

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











*/







