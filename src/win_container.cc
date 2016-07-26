/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/


#include "win_container.h"

win_container::win_container()
{
  move(0,0);  

  set_layout(NO_LAYOUT);

  set_space_with_border(SPACE_WITH_BORDER);

  set_space_with_object(SPACE_WITH_OBJECT);

  focus_object_ = NULL;

}

win_container::~win_container()
{
  destroy();
}

void win_container::move(s_int16 tx, s_int16 ty)
{
  win_base::move(tx,ty);
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    (*i)->update_position();
}

void win_container::resize(u_int16 tl, u_int16 th)
{
  win_base::resize(tl,th);

  update_layout();
  
}

void win_container::update_position()
{
  win_base::update_position();
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    (*i)->update_position();
}

void win_container::add(win_base * w)
{
  list_wb_.push_back(w);
  
  w->set_container(this);
  
  update_layout();
}

void win_container::remove(win_base * w)
{
  list_wb_.remove(w);
  
  w->set_container(NULL);

  update_layout();
}

void win_container::remove_all()
{
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    {
      (*i)->set_container(NULL);
      list_wb_.erase(i);
    }
}

void win_container::destroy()
{
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    delete *i;
  list_wb_.clear();
}

bool win_container::update()
{
    if(win_base::update())
    {
      for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
	{
	  if(!(*i)->update())
	    {
	      list_wb_.erase(i);
	      delete *i--;
	    }
	}
      return true;
    } 
  return false;
}


bool win_container::input_update()
{
  if(win_base::input_update())
    {
      if(focus_object_) focus_object_->input_update();
      return true;
    }
  return false;
}


void win_container::set_focus_object(win_base * f)
{
  if(focus_object_) focus_object_->set_focus(false);
  if(f) f->set_focus(true);
  focus_object_ = f;
}

void win_container::set_brightness(bool b)
{
  win_base::set_brightness(b);
  
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    (*i)->set_brightness(b);
}

void win_container::set_trans(bool b)
{
  win_base::set_trans(b);
  
  for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
    (*i)->set_trans(b);
}

void win_container::set_visible_all(bool b)
{
  set_visible(b);
   for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
     (*i)->set_visible(b);
}



bool win_container::draw()
{
  if(win_base::draw()) 
    {
      assign_drawing_area(wb_father_); 

      win_background::draw(this);   
      
      for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
	(*i)->draw();
      
      win_border::draw(wb_father_);
      
      detach_drawing_area();
      
      return true;
    }
  return false;
}



void win_container::update_layout()
{
  
  u_int16 indice_h=space_with_border_;
  
  switch(layout_)
    {
    case LIST_LAYOUT:
      
      for(lwb::iterator i=list_wb_.begin();i!=list_wb_.end();i++)
	{
	  (*i)->move((*i)->x(),indice_h);
	  
	  indice_h+=(*i)->height()+space_with_object_;
	}
      
      break;
    }
}

/*
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_theme.h"
#include "win_container.h"


//constructor
win_container::win_container(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth):win_base(tx,ty,tl,th,wth)
{
  //clear the list og object
  list_obj.clear();

  //set the space between border and object
  space_between_border_=WIN_SPACE_BETWEEN_BORDER;

  //set space between object and another object
  space_between_object_=WIN_SPACE_BETWEEN_OBJECT;
*/
  /*
  //set justify ----> WARNING MAYBE IN WIN_BASE ????
  justify_=WIN_JUSTIFY_NO;

  */
/*
  //set layout equals no
  layout_=WIN_LAYOUT_NO;
}


void win_container::set_space_between_object(u_int16 tmp)
{
  //set the space between object and call update layout to redefine position of each element
  space_between_object_=tmp;
  update_layout();
}

void win_container::set_space_between_border(u_int16 tmp)
{
  //set the space between border and object and call update layout to redefine position of each element
  space_between_border_=tmp;
  update_layout();
}

win_container::~win_container()
{
  destroy();
}

void win_container::add(win_base * tmp)
{
  //add the object 
  list_obj.push_back(tmp);
  
  //set the new object
  tmp->wb_father_=this;
  tmp->update_real_position();
  
  
  tmp->update_align();

  //update layout
  update_layout();
  //if the win_container is brightness set the new object in brightness mode
  if(draw_brightness_) tmp->set_draw_brightness(true);
}


void win_container::remove(win_base * tmp)
{
  list<win_base *>::iterator i=list_obj.begin();
  while(i!=list_obj.end() && tmp!=(*i)) i++;
  if(i!=list_obj.end()) 
  {
    //list_obj.remove(tmp);
    tmp->wb_father_=NULL;
    list_obj.erase(i);
    update_layout();
  }
}

void win_container::remove_all()
{
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->wb_father_=NULL;
  list_obj.clear();
}

void win_container::destroy()
{
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    delete *i;
  list_obj.clear(); 
}

void win_container::resize(u_int16 tl,u_int16 th)
{
  win_base::resize(tl,th);
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->update_align();  
}

bool win_container::update()
{
  //call the win update ---> on update()
  if(win_base::update())
    {
      //update all the element in the list
      for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
	{
	  if(!(*i)->update())
	    {
	      remove(*i);
	      delete *i--;
	    }
	}
      return true;
    }
  return false;
}


bool win_container::draw()
{
  if(win_base::draw()) 
    {
      assign_drawing_area(); //assign drawing area
      //draw the background
      draw_background();
      //next draw all the element
      for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
	(*i)->draw();
      //draw the border
      draw_border();
      //detach the drawing area
      detach_drawing_area();
      return true;
    }
  return false;
}


//set visible mode for all element in list
void win_container::set_visible_all(bool b)
{
  visible_=b;
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->set_visible(visible_);
}

//set draw brightness for all element in the list
void win_container::set_draw_brightness(bool b)
{
  win_base::set_draw_brightness(b);
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->set_draw_brightness(b);
}
*/
/*

//set justify an object in this container
void win_container::set_justify(win_base * wb, u_int8 just)
{
  switch(just)
    {
    case WIN_JUSTIFY_LEFT:
      wb->move(space_between_border_,wb->y());
      break;
    case WIN_JUSTIFY_RIGHT:
      wb->move(length_-space_between_border_-wb->length(),wb->y());
      break;
    case WIN_JUSTIFY_CENTER:
      if(length_>wb->length()) 
	wb->move((length_-wb->length())>>1,wb->y());
      break;
    }
} 

//justify all the element

*/

/*
void win_container::set_align_all(u_int8 a)
{
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->set_align(a);
}




//sezt the layout and update
void win_container::set_layout(u_int8 lay)
{
  layout_=lay;
  update_layout();
}

//
void win_container::update_real_position()
{
  win_base::update_real_position();
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->update_real_position();
}

void win_container::move(s_int16 tx,s_int16 ty)
{
  win_base::move(tx,ty);
  update_real_position();
}


//just one layout actually but i whish add another --> grid layout
void win_container::update_layout()
{
  u_int16 old_h=0;
  u_int16 indice_h=space_between_border_;
  u_int16 indice_l=space_between_border_;  
  switch(layout_)
    {
    case WIN_LAYOUT_LIST:
      //u_int16 indice=space_between_border_;
      for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
	{
	  (*i)->move((*i)->x(),indice_h);
	  indice_h+=(*i)->height()+space_between_object_;
	}
      break;
      
    case WIN_LAYOUT_AUTO:
     
      for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
	{
	  
	  if(indice_l+(*i)->length()>length_)
	    {
	      indice_l=space_between_border_;
	      indice_h+=old_h+space_between_object_;
	      (*i)->move(indice_l,indice_h);
	    }
	  
	  (*i)->move(indice_l,indice_h);
	  indice_l+=(*i)->length()+space_between_object_;
	  old_h=(*i)->height();
	}
      break;

      


    default:
      break;
    }
}


void win_container::set_focus(bool b)
{
  for(list<win_base *>::iterator i=list_obj.begin();i!=list_obj.end();i++)
    (*i)->set_focus(b);
  focus_=b;
}






*/








