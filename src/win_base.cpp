
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
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_container.h"
#include "win_select.h"


#ifdef _DEBUG_
u_int16 win_base::cpt_win_obj_debug=0;
#endif



void win_base::init_base(s_int16 tx,s_int16 ty, u_int16 tl,u_int16 th,win_container *twc,drawing_area * tda)
{
   //set position
   win_base::x=tx;
   win_base::y=ty;
   win_base::length=tl;
   win_base::height=th;
   save_x=0;
   save_y=0;
   
   
   //set flags
   selected=false;
   visible=false;
   
   
   wc=twc;
   da=tda;

   //update position
   win_base::update_da();

   //initialize image
  
   h_border=NULL;
   v_border=NULL;
   corner_t_l=NULL;
   corner_t_r=NULL;
   corner_b_l=NULL;
   corner_b_r=NULL;
   background=NULL;
   wborder=NULL;
   wback=NULL;
   
   select_corner_t_l=NULL;
   select_corner_t_r=NULL;
   select_corner_b_l=NULL;
   select_corner_b_r=NULL;
   select_h_border=NULL;
   select_v_border=NULL;
   wborder_select=NULL;

   cursor=NULL;
   //initialize transluency of background
   level_trans_back=180;

   //initialize select
   wselect=NULL;

   //init_var
   act_function=NULL;

   //initialize pad
   x_pad_l =0;
}

win_base::win_base(s_int16 tx,s_int16 ty, u_int16 tl,u_int16 th,win_container * twc=NULL,drawing_area * tda=NULL)
{
#ifdef _DEBUG_
  cout << "win_base() called, "<< ++cpt_win_obj_debug
       << " objects currently allocated\n";
#endif
  init_base(tx,ty,tl,th,twc,tda);
}

win_base::~win_base()
{
  wc=NULL;
  da=NULL;
  wborder=NULL;
  wback=NULL;
  cursor=NULL;
  if(wselect) wselect->remove(this);
#ifdef _DEBUG_
  cout << "~win_base() called, "<< --cpt_win_obj_debug
       << " objects currently allocated\n";
#endif
  destroy_border();
  destroy_border_select();
  if(background) delete background;
}


void win_base::destroy_border()
{
  if(h_border) delete h_border;
  if(v_border) delete v_border;
  if(corner_t_l) delete corner_t_l;
  if(corner_t_r) delete corner_t_r;
  if(corner_b_l) delete corner_b_l;
  if(corner_b_r) delete corner_b_r;
}

void win_base::destroy_border_select()
{
  if(select_h_border) delete select_h_border;
  if(select_v_border) delete select_v_border;
  if(select_corner_t_l) delete select_corner_t_l;
  if(select_corner_t_r) delete select_corner_t_r;
  if(select_corner_b_l) delete select_corner_b_l;
  if(select_corner_b_r) delete select_corner_b_r;
}

void win_base::select()
{
  selected=true;
}

void win_base::unselect()
{
  selected=false;
}

void win_base::show()
{
  visible=true;
}

void win_base::hide()
{
  visible=false;
}

s_int16 win_base::get_x()
{
   return(x);
}

s_int16 win_base::get_y()
{
   return(y);
}

u_int16 win_base::get_length()
{
   return(length);
}

u_int16 win_base::get_height()
{
   return(height);
}

void win_base::resize(u_int16 tl, u_int16 th)
{
   length=tl;
   height=th;
   if(wc) wc->find_obj_max_y(); //for scrollbar
   resize_border();
   resize_background();
}

void win_base::move(s_int16 tx, s_int16 ty,bool move_by_scrollbar=false )
{
   x=tx;
   y=ty;
   if(!move_by_scrollbar)
     if(wc) wc->find_obj_max_y(); //for scrollbar
   win_base::update_da();
}

drawing_area * win_base::get_drawing_area()
{
   return(da);
}

win_border * win_base::get_border()
{
  return(wborder);
}

void win_base::set_border(win_border * twb)
{
   wborder=twb;
   if(wborder)
     {
       destroy_border();
       h_border=new image();
       v_border=new image();
       corner_t_l=new image();
       corner_t_r=new image();
       corner_b_l=new image();
       corner_b_r=new image();
       *corner_t_l=*(twb->corner_top_left);
       *corner_t_r=*(twb->corner_top_right);
       *corner_b_l=*(twb->corner_bottom_left);
       *corner_b_r=*(twb->corner_bottom_right);
       resize_border();
     }
}

void win_base::set_border_select(win_border * twb)
{
  if(wselect)
    {
      wborder_select=twb;
      if(wborder_select)
	{
	  destroy_border_select();
	  select_h_border=new image();
	  select_v_border=new image();
	  select_corner_t_l=new image();
	  select_corner_t_r=new image();
	  select_corner_b_l=new image();
	  select_corner_b_r=new image();
	  *select_corner_t_l=*(twb->corner_top_left);
	  *select_corner_t_r=*(twb->corner_top_right);
	  *select_corner_b_l=*(twb->corner_bottom_left);
	  *select_corner_b_r=*(twb->corner_bottom_right);
	  resize_border();
	}
    }
}

void
win_base::set_cursor (win_cursor * tmp)
{
  if (tmp)
    {
      if (cursor)delete cursor;
      cursor = new image ();
      *cursor = *(tmp->cursor);
      x_pad_l=cursor->length;
    } else x_pad_l=0;
}

void win_base::resize_border()
{
   if(wborder)
     {
	v_border->resize(wborder->v_border_template->length,height);
	v_border->putbox_tile_img(wborder->v_border_template);
	h_border->resize(length,wborder->h_border_template->height);
	h_border->putbox_tile_img(wborder->h_border_template); 
     }
   if(wselect && wborder_select)
     {
       select_v_border->resize(wborder_select->v_border_template->length,height);
       select_v_border->putbox_tile_img(wborder_select->v_border_template);
       select_h_border->resize(length,wborder_select->h_border_template->height);
       select_h_border->putbox_tile_img(wborder_select->h_border_template); 
     }
}




void win_base::draw_border()
{
#define WIN_CORNER_RELX (corner_b_l->length>>1)
#define WIN_CORNER_RELY (corner_b_l->height>>1)
#define WIN_HBRELX (wborder->h_border_template->length>>1)
#define WIN_HBRELY (wborder->h_border_template->height>>1)
#define WIN_VBRELX (wborder->v_border_template->length>>1)
#define WIN_VBRELY (wborder->v_border_template->height>>1)

#define WIN_CORNER_SELECT_RELX (select_corner_b_l->length>>1)
#define WIN_CORNER_SELECT_RELY (select_corner_b_l->height>>1)
#define WIN_HBRELX_SELECT (wborder_select->h_border_template->length>>1)
#define WIN_HBRELY_SELECT (wborder_select->h_border_template->height>>1)
#define WIN_VBRELX_SELECT (wborder_select->v_border_template->length>>1)
#define WIN_VBRELY_SELECT (wborder_select->v_border_template->height>>1)

  if(wselect)
    {
      if(selected)
	switch(select_mode)
	  {
	  case WIN_SELECT_MODE_BORDER:
	    //draw with select_border
	    if(wborder_select)
	      { 
		if(wc)
		  {
		    select_h_border->putbox_mask(real_x,real_y-select_h_border->height,wc->get_drawing_area());
		    select_h_border->putbox_mask(real_x,real_y+height,wc->get_drawing_area());
		    select_v_border->putbox_mask(real_x-select_v_border->length,real_y,wc->get_drawing_area());
		    select_v_border->putbox_mask(real_x+length,real_y,wc->get_drawing_area());
		    select_corner_t_l->putbox_mask(real_x-WIN_CORNER_SELECT_RELX-((select_v_border->length)>>1),
						   real_y-WIN_CORNER_SELECT_RELY-(select_h_border->height>>1),wc->get_drawing_area());   //t_l   
		    select_corner_t_r->putbox_mask(real_x+length-WIN_CORNER_SELECT_RELX+(select_v_border->length>>1),
						   real_y-WIN_CORNER_SELECT_RELY-(select_h_border->height>>1),wc->get_drawing_area());      //t_r
		    select_corner_b_l->putbox_mask(real_x-WIN_CORNER_SELECT_RELX-((select_v_border->length)>>1),
						   real_y+height-WIN_CORNER_SELECT_RELY+(select_h_border->height>>1),wc->get_drawing_area());    //b_l 
		    select_corner_b_r->putbox_mask(real_x+length-WIN_CORNER_SELECT_RELX+(select_v_border->length>>1),
						   real_y+height-WIN_CORNER_SELECT_RELY+(select_h_border->height>>1),wc->get_drawing_area()); //b_r
		  }
		else
		  {
		    select_h_border->putbox_mask(real_x,real_y-select_h_border->height,wc->get_drawing_area());
		    select_h_border->putbox_mask(real_x,real_y+height,wc->get_drawing_area());
		    select_v_border->putbox_mask(real_x-select_v_border->length,real_y,wc->get_drawing_area());
		    select_v_border->putbox_mask(real_x+length,real_y,wc->get_drawing_area());
		    select_corner_t_l->putbox_mask(real_x-WIN_CORNER_SELECT_RELX-((select_v_border->length)>>1),
						   real_y-WIN_CORNER_SELECT_RELY-(select_h_border->height>>1));   //t_l   
		    select_corner_t_r->putbox_mask(real_x+length-WIN_CORNER_SELECT_RELX+(select_v_border->length>>1),
						   real_y-WIN_CORNER_SELECT_RELY-(select_h_border->height>>1));      //t_r
		    select_corner_b_l->putbox_mask(real_x-WIN_CORNER_SELECT_RELX-((select_v_border->length)>>1),
						   real_y+height-WIN_CORNER_SELECT_RELY+(select_h_border->height>>1));    //b_l 
		    select_corner_b_r->putbox_mask(real_x+length-WIN_CORNER_SELECT_RELX+(select_v_border->length>>1),
						   real_y+height-WIN_CORNER_SELECT_RELY+(select_h_border->height>>1));
		  }
	      }
	    break;
	  default:
	    if(wborder)
	      {
		if(wc)
		  { 
		    h_border->putbox_mask(real_x,real_y-h_border->height,wc->get_drawing_area());
		    h_border->putbox_mask(real_x,real_y+height,wc->get_drawing_area());
		    v_border->putbox_mask(real_x-v_border->length,real_y,wc->get_drawing_area());
		    v_border->putbox_mask(real_x+length,real_y,wc->get_drawing_area());
		    
		    corner_t_l->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
					real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());   //t_l   
		    corner_t_r->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
					    real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      //t_r
		    corner_b_l->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
					    real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());    //b_l 
		    corner_b_r->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
					    real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area()); //b_r
		  }
		else
		  {
		    h_border->putbox_mask(x,y-h_border->height);
		    h_border->putbox_mask(x,y+height);
		    v_border->putbox_mask(x-v_border->length,y);
		    v_border->putbox_mask(x+length,y);
		    
		    corner_t_l->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
					    y-WIN_CORNER_RELY-(h_border->height>>1));      
		    corner_t_r->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
					    y-WIN_CORNER_RELY-(h_border->height>>1));      
		    corner_b_l->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
					    y+height-WIN_CORNER_RELY+(h_border->height>>1));     
		    corner_b_r->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
					    y+height-WIN_CORNER_RELY+(h_border->height>>1));
		  }
	      }
	    
	break;
	  }
      else
	{
	  switch(select_mode)
	    {
	    case WIN_SELECT_MODE_BRIGHTNESS:
	      if(wborder)
		{
		  static image tmp;
		  if(wc)
		    { 
		  
		      tmp.brightness(h_border,120);
		      tmp.putbox_mask(real_x,real_y-h_border->height,wc->get_drawing_area());
		      tmp.putbox_mask(real_x,real_y+height,wc->get_drawing_area());
		      tmp.brightness(v_border,120);
		      tmp.putbox_mask(real_x-v_border->length,real_y,wc->get_drawing_area());
		      tmp.putbox_mask(real_x+length,real_y,wc->get_drawing_area());
		      
		      tmp.brightness(corner_t_l,120);
		      tmp.putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
				      real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());    
		      tmp.brightness(corner_t_r,120);
		      tmp.putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
		      tmp.brightness(corner_b_l,120);
		      tmp.putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
				      real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());     
		      tmp.brightness(corner_b_r,120);
		      tmp.putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());
		    }
		  else
		    {
		      tmp.brightness(h_border,120);
		      tmp.putbox_mask(x,y-h_border->height);
		      tmp.putbox_mask(x,y+height);
		      
		      tmp.brightness(v_border,120);
		      tmp.putbox_mask(x-v_border->length,y);
		      tmp.putbox_mask(x+length,y);
		      
		      tmp.brightness(corner_t_l,120);
		      tmp.putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
				      y-WIN_CORNER_RELY-(h_border->height>>1));   
		      tmp.brightness(corner_t_r,120);
		      tmp.putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      y-WIN_CORNER_RELY-(h_border->height>>1));      
		      tmp.brightness(corner_b_l,120);
		      tmp.putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
				      y+height-WIN_CORNER_RELY+(h_border->height>>1));     
		      tmp.brightness(corner_b_r,120);
		      tmp.putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
				      y+height-WIN_CORNER_RELY+(h_border->height>>1));
		    }
		}
	      break;
	      
	    default:
	      if(wborder)
		if(wc)
		  { 
		    h_border->putbox_mask(real_x,real_y-h_border->height,wc->get_drawing_area());
		    h_border->putbox_mask(real_x,real_y+height,wc->get_drawing_area());
		    v_border->putbox_mask(real_x-v_border->length,real_y,wc->get_drawing_area());
		    v_border->putbox_mask(real_x+length,real_y,wc->get_drawing_area());
		    
		    corner_t_l->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
					    real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());   //t_l   
		    corner_t_r->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
					    real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      //t_r
		    corner_b_l->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
					    real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());    //b_l 
		    corner_b_r->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
					    real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area()); //b_r
		  }
		else
		  {
		    h_border->putbox_mask(x,y-h_border->height);
		    h_border->putbox_mask(x,y+height);
		    v_border->putbox_mask(x-v_border->length,y);
		    v_border->putbox_mask(x+length,y);
		    
		    corner_t_l->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
					    y-WIN_CORNER_RELY-(h_border->height>>1));      
		    corner_t_r->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
					    y-WIN_CORNER_RELY-(h_border->height>>1));      
		    corner_b_l->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
					    y+height-WIN_CORNER_RELY+(h_border->height>>1));     
		    corner_b_r->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
					    y+height-WIN_CORNER_RELY+(h_border->height>>1));
		  }
	      
	      break;
	    }
	  
	}
    }
  else
    {
      if(wborder)
	if(wc)
	  { 
	    h_border->putbox_mask(real_x,real_y-h_border->height,wc->get_drawing_area());
	    h_border->putbox_mask(real_x,real_y+height,wc->get_drawing_area());
	    v_border->putbox_mask(real_x-v_border->length,real_y,wc->get_drawing_area());
	    v_border->putbox_mask(real_x+length,real_y,wc->get_drawing_area());
	    
	    corner_t_l->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
				    real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());   //t_l   
	    corner_t_r->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				    real_y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      //t_r
	    corner_b_l->putbox_mask(real_x-WIN_CORNER_RELX-((v_border->length)>>1),
				    real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());    //b_l 
	    corner_b_r->putbox_mask(real_x+length-WIN_CORNER_RELX+(v_border->length>>1),
				    real_y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area()); //b_r
	  }
	else
	  {
	    h_border->putbox_mask(x,y-h_border->height);
	    h_border->putbox_mask(x,y+height);
	    v_border->putbox_mask(x-v_border->length,y);
	    v_border->putbox_mask(x+length,y);
	    
	    corner_t_l->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
				    y-WIN_CORNER_RELY-(h_border->height>>1));      
	    corner_t_r->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
				    y-WIN_CORNER_RELY-(h_border->height>>1));      
	    corner_b_l->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
				    y+height-WIN_CORNER_RELY+(h_border->height>>1));     
	    corner_b_r->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
				    y+height-WIN_CORNER_RELY+(h_border->height>>1));
	  }      
    }
}


void win_base::set_background(win_background * twb)
{
  wback=twb;
  if(background) delete background;
  background = new image();
  resize_background();  
}


void win_base::draw_background()
{
  if(wback)
    {
      if(wselect && select_mode==WIN_SELECT_MODE_BRIGHTNESS)
	{
	  if(selected)
	    {
	      if(wc) background->putbox_trans(real_x,real_y,level_trans_back,wc->get_drawing_area());
	      else background->putbox_trans(real_x,real_y,level_trans_back);
	    } 
	  else
	    {
	      image tmp;
	      tmp.brightness(background,120);
	      if(wc) tmp.putbox_trans(real_x,real_y,level_trans_back,wc->get_drawing_area());
	      else tmp.putbox_trans(real_x,real_y,level_trans_back);
	    }
	}
      else
	{
	  if(wc) background->putbox_trans(real_x,real_y,level_trans_back,wc->get_drawing_area());
	  else background->putbox_trans(real_x,real_y,level_trans_back);
	}
    }
}


void win_base::resize_background()
{
  if(wback) 
    {
      background->resize(length,height);
      background->putbox_tile_img(wback->background_template);
    }
}

void win_base::update_da()
{
  if(wc)
    {
      real_x=x+wc->real_x;
      real_y=y+wc->real_y;
    }
  else 
    {
      real_x=x;
      real_y=y;
    }
}


void win_base::attach_activate_function(void (*f)(void*,void*),void * d1,void * d2)
{
  act_function=f;
  param_func1=d1;
  param_func2=d2;
}

void win_base::detach_activate_function()
{
  act_function=NULL;
  param_func1=NULL;
  param_func2=NULL;
}
void win_base::execute_activate_function()
{
  if(act_function) 
    {
      act_function(param_func1,param_func2);
    }
}

void win_base::activate()
{
  execute_activate_function();
}

void win_base::set_select_mode(u_int8 tmp)
{
  select_mode=tmp;
}

u_int8 win_base::get_select_mode()
{
  return select_mode;
}



void win_base::save_position()
{
  save_x=x;
  save_y=y;
}

void win_base::reload_position()
{
  move(save_x,save_y);
}


void win_base::attach_select(win_select * tws, u_int8 tmode)
{
  wselect=tws;
  select_mode=tmode;
}

void win_base::dettach_select()
{
  wselect=NULL;
}














