#include <iostream.h>
#include <list>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_border.h"
#include "win_container.h"

void win_base::init_base(u_int16 tx,u_int16 ty, u_int16 tl,u_int16 th,win_container *twc,drawing_area * tda)
{
  win_base::x=tx;
  win_base::y=ty;
  win_base::length=tl;
  win_base::height=th;
  selected=false;
  visible=false;
  da=tda;
  wc=twc;
  h_border=NULL;
  v_border=NULL;
  corner=NULL;
  background=NULL;

  level_trans_back=180;
  
  wborder=NULL;
  wback=NULL;
}

win_base::win_base(u_int16 tx,u_int16 ty, u_int16 tl,u_int16 th)
{
  init_base(tx,ty,tl,th,NULL,NULL);
}

win_base::win_base(u_int16 tx,u_int16 ty, u_int16 tl,u_int16 th,win_container * twc)
{
  init_base(tx,ty,tl,th,twc,NULL);
}

win_base::win_base(u_int16 tx,u_int16 ty, u_int16 tl,u_int16 th,win_container * twc,drawing_area * tda)
{
  init_base(tx,ty,tl,th,twc,tda);
}

win_base::~win_base()
{
  wc=NULL;
  da=NULL;
  wborder=NULL;
  wback=NULL;
  if(h_border) delete h_border;
  if(v_border) delete v_border;
  if(corner) delete corner;
  if(background) delete background;
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

u_int16 win_base::get_x()
{
  return(x);
}

u_int16 win_base::get_y()
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
  resize_border();
  resize_background();
}

void win_base::move(u_int16 tx, u_int16 ty)
{
  x=tx;
  y=ty;
}

drawing_area * win_base::get_drawing_area()
{
  return(da);
}


void win_base::set_border(win_border * twb)
{
  wborder=twb;
  if(twb)
    {
      if(!h_border) h_border=new image();
      if(!v_border) v_border=new image();
      if(!corner) v_border=new image();
      corner=twb->corner; // copy corner
      resize_border();
    }
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
}




void win_base::draw_border()
{
  if(wborder)
    {
#define WIN_CORNER_RELX (corner->length>>1)
#define WIN_CORNER_RELY (corner->height>>1)
#define WIN_HBRELX (wborder->h_border_template->length>>1)
#define WIN_HBRELY (wborder->h_border_template->height>>1)
#define WIN_VBRELX (wborder->v_border_template->length>>1)
#define WIN_VBRELY (wborder->v_border_template->height>>1)
      if(wc)
	{
	  h_border->putbox(wc->get_x()+x,wc->get_y()+y-h_border->height,wc->get_drawing_area());
	  h_border->putbox(wc->get_x()+x,wc->get_y()+y+height,wc->get_drawing_area());
	  v_border->putbox(wc->get_x()+x-v_border->length,wc->get_y()+y,wc->get_drawing_area());
	  v_border->putbox(wc->get_x()+x+length,wc->get_y()+y,get_drawing_area());
	  
	  corner->putbox_mask(wc->get_x()+x-WIN_CORNER_RELX-((v_border->length)>>1),
			      wc->get_y()+y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
	  corner->putbox_mask(wc->get_x()+x+length-WIN_CORNER_RELX+(v_border->length>>1),
			      wc->get_y()+y-WIN_CORNER_RELY-(h_border->height>>1),wc->get_drawing_area());      
	  corner->putbox_mask(wc->get_x()+x-WIN_CORNER_RELX-((v_border->length)>>1),
			      wc->get_y()+y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());     
	  corner->putbox_mask(wc->get_x()+x+length-WIN_CORNER_RELX+(v_border->length>>1),
			      wc->get_y()+y+height-WIN_CORNER_RELY+(h_border->height>>1),wc->get_drawing_area());
	}
      else
	{
	  h_border->putbox(x,y-h_border->height);
	  h_border->putbox(x,y+height);
	  v_border->putbox(x-v_border->length,y);
	  v_border->putbox(x+length,y);
	  
	  corner->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
			 y-WIN_CORNER_RELY-(h_border->height>>1),NULL);      
	  corner->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
			      y-WIN_CORNER_RELY-(h_border->height>>1),NULL);      
	  corner->putbox_mask(x-WIN_CORNER_RELX-((v_border->length)>>1),
			      y+height-WIN_CORNER_RELY+(h_border->height>>1));     
	  corner->putbox_mask(x+length-WIN_CORNER_RELX+(v_border->length>>1),
			      y+height-WIN_CORNER_RELY+(h_border->height>>1));
	}
    }
}


void win_base::set_background(win_background * twb)
{
  wback=twb;
  if(!background) background = new image();
  resize_background();  
}


void win_base::draw_background()
{
  if(wback)
    {
      if(wc) background->putbox_trans(wc->get_x()+x,wc->get_y()+y,level_trans_back,wc->get_drawing_area());
      else background->putbox_trans(x,y,level_trans_back);
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







