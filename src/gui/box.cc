/*
   $Id$

   (C) Copyright 2002 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "box.h"

using namespace gui;

box::box () : geometry_ (box::HORIZONTAL), homogene_ (false), spacing_ (0)
{
    
}


void box::set_homogene(const bool b)
{
    homogene_ = b; 
}


bool box::get_homogene () const
{
    return homogene_; 
}


void box::set_spacing(const u_int16 b)
{
    spacing_ = b; 
}


u_int16 box::get_spacing () const
{
    return spacing_; 
}


void box::add (widget * w)
{
  /* by default add the widget at the end */
    add_end (w);     
}


void box::remove (widget * w)
{
    std::deque <box_struct* >::iterator it = v_widget_.begin ();
    /* find the widget */
    while (it != v_widget_.end () && (*it)->widget_ != w) it++;

    /* if there is a widget erase it */
    if (it != v_widget_.end ()) 
      {
        v_widget_.erase (it);
	/* call the remove method */
	on_remove (); 
      }
}


void box::clear ()
{
  /* destroy all objet */
  for (std::deque <box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
    {
      delete (*it)->widget_;
      delete *it; 
    }
  v_widget_.clear (); 
}
    

void box::add_start (widget * w, bool expand, bool fill, u_int16 padding)
{
  box_struct * tmp =  new box_struct;
  
  tmp->widget_ = w;
  tmp->fill_ = fill;
  tmp->expand_ = expand;
  tmp->padding_ = padding;
  
  v_widget_.push_front (tmp);  
  w->set_parent (this); 
  
  update_homogene (); 
  
  on_add (); 
}


void box::realize ()
{
  container::realize();
  if (geometry_ == HORIZONTAL) realize_horizontal ();
  else realize_vertical (); 
}


void box::realize_horizontal ()
{
  if (v_widget_.size () == 0) return; 

  /* init value with border width */
  u_int16 x_tmp = 0; // my_border_width;
  u_int16 y_tmp = 0; // my_border_width;

  /* calcul the max length for each widget*/
  u_int16 max_length = (get_length () - ((v_widget_.size () - 1)  * spacing_) - (my_border_width << 1) ) / v_widget_.size (); 
  
  for (u_int16 i = 0;i < v_widget_.size (); i++)  
    {
      if (v_widget_[i]->expand_ == true)
        {
	  if (v_widget_[i]->fill_ == true)
            {
	      v_widget_[i]->widget_->set_size (max_length, get_height () - (my_border_width << 1)); 
	      v_widget_[i]->widget_->set_position (x_tmp, y_tmp);
	      x_tmp += max_length; 
            }
	  else
            {
	      // before fill just add padding
	      v_widget_[i]->widget_->set_size ( max_length - (v_widget_[i]->padding_ << 1), 
						get_height () - (my_border_width << 1) - (v_widget_[i]->padding_ << 1));
	      v_widget_[i]->widget_->set_position (x_tmp + v_widget_[i]->padding_, y_tmp + v_widget_[i]->padding_);
	      x_tmp += max_length; 
            } 
        }
      else
	{
	  /* move */
	  v_widget_[i]->widget_->set_position (x_tmp, y_tmp); 
	  
	  v_widget_[i]->widget_->set_size ( (v_widget_[i]->widget_->get_length () > max_length) ? max_length : v_widget_[i]->widget_->get_length (),
					    get_height () - (my_border_width << 1) );   
	  
	  x_tmp += v_widget_[i]->widget_->get_length ();
	  
	  /* calcul new max_length */
	  if ((v_widget_.size () - i - 1) != 0) 
	    max_length = ((get_length () - ((v_widget_.size () - 1)  * spacing_) - my_border_width ) - x_tmp )
	      / ( v_widget_.size () - i - 1)   ;
	  else
            {
	      /*this is the last widget*/
	      /* we resize the container */
	      set_size (x_tmp + my_border_width, get_height ());  
            } 
        }  
        /*add space between each widget */
        x_tmp += spacing_; 
    }
}


void box::realize_vertical ()
{
  /* if nothing return */
  if (v_widget_.size () == 0) return; 
  
  s_int32 x_tmp = 0; // not border with because border width is added in widget::set_position
  s_int32 y_tmp = 0; 
  
  /* calcul the max height for each widget*/
  u_int32 max_height = (get_height () - ((v_widget_.size () - 1)  * spacing_) - (my_border_width << 1) ) / v_widget_.size (); 
  
  for (u_int16 i = 0;i < v_widget_.size (); i++)  
    {
      //if we set widget size at the box size 
      if (v_widget_[i]->expand_ == true)
        {
	  //if fill is false we strect the widget width padding value
	  if (v_widget_[i]->fill_ == true)
            {
                v_widget_[i]->widget_->set_size (get_length () - (my_border_width << 1),  max_height); 
                v_widget_[i]->widget_->set_position (x_tmp, y_tmp);
                y_tmp += max_height; 
            }
            else
            {
	      // before fill just add padding
	      v_widget_[i]->widget_->set_size (get_length () - (my_border_width << 1) - (v_widget_[i]->padding_ << 1),
					       max_height - (v_widget_[i]->padding_ << 1)); 
	      v_widget_[i]->widget_->set_position (x_tmp + v_widget_[i]->padding_,
						   y_tmp + v_widget_[i]->padding_);
	      y_tmp += max_height; 
            } 
        }
        else
        {
	  /* move */
	  v_widget_[i]->widget_->set_position (x_tmp, y_tmp); 
	  
	  v_widget_[i]->widget_->set_size (get_length () - (my_border_width << 1),
					   (v_widget_[i]->widget_->get_height () > max_height) ? max_height : v_widget_[i]->widget_->get_height ()
					   );   
            
            y_tmp += v_widget_[i]->widget_->get_height ();
            
            /* calcul new max_length */
            if ((v_widget_.size () - i - 1) != 0) 
                max_height = ((get_height () - ((v_widget_.size () - 1)  * spacing_) - my_border_width ) - y_tmp )
                    / ( v_widget_.size () - i - 1)   ;
            else
            {
	      /*this is the last widget*/
	      /* we resize the container */
	      set_size (get_length (), y_tmp + my_border_width);  
            } 
        }  
      /*add space between each widget */
      y_tmp += spacing_; 
    }
}



void box::add_end (widget * w, bool expand, bool fill, u_int16 padding)
{
    box_struct * tmp =  new box_struct;

    tmp->widget_ = w;
    tmp->fill_ = fill;
    tmp->expand_ = expand;
    tmp->padding_ = padding;

    v_widget_.push_back (tmp);  
    w->set_parent (this);
    
    update_homogene (); 
    
    on_add (); 
}


box::~box ()
{
    clear (); 
}


void box::update_homogene ()
{
    if (!homogene_) return;

    // WARNING : should be improve with keep in this max_length and max_height
    // find max length and max height
    u_int16 max_length = 0;
    u_int16 max_height = 0;
    
    for (std::deque <box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
    {
        if (max_length < (*it)->widget_->get_length ()) max_length = (*it)->widget_->get_length ();
        if (max_length < (*it)->widget_->get_height ()) max_height = (*it)->widget_->get_height ();
    }

    // now resize
    for (std::deque <box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
        (*it)->widget_->set_size (max_length, max_height);    
}




bool box::draw (gfx::drawing_area * da, gfx::surface * sf)
{    
  if (container::draw (da, sf))
    {
      assign_drawing_area (da);
      for (std::deque <box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
        (*it)->widget_->draw (this, sf);
      detach_drawing_area();
      return true;
    }
  return false;
}


void box::set_geometry (const u_int8 g)
{
    geometry_ = g; 
}



void box::update_position () 
{
    container::update_position (); 
    
    for (std::deque <box_struct* >::iterator it = v_widget_.begin (); it != v_widget_.end (); it++)
        (*it)->widget_->update_position ();
}


int box::input_update (input::event * ev)
{
  u_int32 i =0;
  int ret = 0;
  while ( i < v_widget_.size() && ret == 0)
    ret = v_widget_[i++]->widget_->input_update(ev);
  return ret;
}


