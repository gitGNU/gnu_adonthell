/*   
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "theme.h"
#include "base.h"
#include "border.h"
#include "../game.h"

using namespace gui;


border::border()
{
  /* there is no objet which used this border /*/
  wb_=NULL;
  
  init();
  
  set_trans_border(false);

  set_visible_border( false );

  set_brightness_border( false );
  
}

border::border(base *wb)
{
  wb_=wb;
  
  init();

  set_trans_border(false);

  set_visible_border( false );

  set_brightness_border( false );
}

border::border(char * rep,char * size)
{
  wb_=NULL;
  
  init();
   
  set_visible_border( false );
   
  set_trans_border(false);
  
  set_brightness_border( false );
  
  border::load(rep,size);
  
  refresh();
}

border::border(border & wb)
{
  wb_=NULL;

  init();
  
  set_visible_border( false );
  
  set_trans_border(false);

  set_brightness_border( false );
  
  *this=wb;

  refresh();
}

border::~border()
{
  destroy();
}


void border::set_border(border & wb)
{
  *this=wb;

  refresh();
}

void border::set_border(theme & wt, u_int8 size )
{
  switch(size)
    {
    case NORMAL:
      *this=*(wt.my_border_normal);
      break;
      
    case MINI:
      *this=*(wt.my_border_mini);
      break;
    }
  refresh();
}

void border::init()
{
  h_border_template_=NULL;
  v_border_template_=NULL;
  for(u_int8 i=0;i<NB_BORDER_IMAGE;i++)
    border_[i]=border_brightness_[i]=NULL;  
}

border & border::operator=(border & wb)
{
  
  destroy();
  h_border_template_=new gfx::image();
  *h_border_template_=*(wb.h_border_template_);
  v_border_template_=new gfx::image();
  *v_border_template_=*(wb.v_border_template_);
  
  for(u_int8 i=0;i<NB_BORDER_IMAGE-2;i++)
    {
      border_[i]=new gfx::image();
      border_brightness_[i]=new gfx::image();
      *(border_[i])=*(wb.border_[i]);
      *(border_brightness_[i])=*(wb.border_brightness_[i]);
      border_[i]->set_mask(true);
      border_brightness_[i]->set_mask(true);
    }
  
  for(u_int8 i=NB_BORDER_IMAGE-2;i<NB_BORDER_IMAGE;i++)
    {
      border_[i]=new gfx::image();
      border_brightness_[i]=new gfx::image();
      border_[i]->set_mask(true);
      border_brightness_[i]->set_mask(true);
    }
  update();
  return *this;
}

void border::destroy()
{
  if(h_border_template_) delete h_border_template_;
  if(v_border_template_)  delete v_border_template_;
  
  for(u_int8 i = 0; i< NB_BORDER_IMAGE; i++)
    {delete border_[i];delete border_brightness_[i];}
  
  init();
}

void border::load(char * rep,char *size)
{
  destroy();
  
  string fullpath;
  string basepath = DIRECTORY;
  basepath += BORDER_DIRECTORY;
  basepath += rep;
  basepath += size; 

  basepath = game::find_directory (basepath);
  
  fullpath = basepath + H_BORDER_TEMPLATE_FILE; 
  h_border_template_=new gfx::image();
  h_border_template_->load_pnm(fullpath.c_str ());//new
  
  fullpath = basepath + V_BORDER_TEMPLATE_FILE;  
  v_border_template_=new gfx::image();
  v_border_template_->load_pnm(fullpath.c_str ());//new
  
  fullpath = basepath + CORNER_TOP_LEFT_FILE;  
  border_[0]=new gfx::image();
  border_[0]->load_pnm(fullpath.c_str ());
  border_brightness_[0]=new gfx::image();
  border_brightness_[0]->brightness(*(border_[0]),BRIGHTNESS_LEVEL);
  border_[0]->set_mask(true);
  border_brightness_[0]->set_mask(true);

  fullpath = basepath + CORNER_TOP_RIGHT_FILE;  
  border_[1]=new gfx::image();
  border_[1]->load_pnm(fullpath.c_str ());
  border_brightness_[1]=new gfx::image();
  border_brightness_[1]->brightness(*(border_[1]),BRIGHTNESS_LEVEL);
  border_[1]->set_mask(true);
  border_brightness_[1]->set_mask(true);


  fullpath = basepath + CORNER_BOTTOM_LEFT_FILE;  
  border_[2]=new gfx::image();
  border_[2]->load_pnm(fullpath.c_str ());
  border_brightness_[2]=new gfx::image();
  border_brightness_[2]->brightness(*(border_[2]),BRIGHTNESS_LEVEL);
  border_[2]->set_mask(true);
  border_brightness_[2]->set_mask(true);
  

  fullpath = basepath + CORNER_BOTTOM_RIGHT_FILE;  
  border_[3]=new gfx::image();
  border_[3]->load_pnm(fullpath.c_str ());
  border_brightness_[3]=new gfx::image();
  border_brightness_[3]->brightness(*(border_[3]),BRIGHTNESS_LEVEL);
  border_[3]->set_mask(true);
  border_brightness_[3]->set_mask(true);
  
  for(u_int8 i=NB_BORDER_IMAGE-2;i<NB_BORDER_IMAGE;i++)
    {
      border_[i]=new gfx::image();
      border_brightness_[i]=new gfx::image();
      border_[i]->set_mask(true);
      border_brightness_[i]->set_mask(true);
    }
}

void border::update()
{  
  if(!h_border_template_ || !v_border_template_ || !wb_) return;
  
  border_[4]->resize(v_border_template_->length(),wb_->height());
  border_[4]->tile(*v_border_template_);
  border_brightness_[4]->brightness(*(border_[4]),BRIGHTNESS_LEVEL);

  border_[5]->resize(wb_->length(),h_border_template_->height());
  border_[5]->tile(*h_border_template_);
  border_brightness_[5]->brightness(*(border_[5]),BRIGHTNESS_LEVEL);
}

u_int16 border::height_border()
{
  if(h_border_template_) return h_border_template_->height();
  return 0;
}

u_int16 border::length_border()
{
  if(v_border_template_) return v_border_template_->length();
  return 0;
}

void border::set_trans_border(bool b)
{
  if(!h_border_template_) return;
  if(b)
    for(u_int8 i=0; i<NB_BORDER_IMAGE;i++)
      {
	border_[i]->set_alpha(130);
	border_brightness_[i]->set_alpha(130);
      }
  else 
    for(u_int8 i=0; i<NB_BORDER_IMAGE;i++)
      {
	border_[i]->set_alpha(255);
	border_brightness_[i]->set_alpha(255);
      }
}

void border::draw(gfx::drawing_area * da)
{  
  if(!h_border_template_ || !visible_border_ || !wb_) return;
  
  u_int8 xodd = (border_draw_[0]->length () % 2);
  u_int8 yodd = (border_draw_[0]->length () % 2); 

  border_draw_[5]->draw(wb_->real_x(), wb_->real_y() - height_border(),da);
  
  border_draw_[5]->draw(wb_->real_x(), wb_->real_y() + wb_->height(),da);
  
  border_draw_[4]->draw(wb_->real_x() - length_border() , wb_->real_y(),da);
  
  border_draw_[4]->draw(wb_->real_x() + wb_->length(), wb_->real_y(),da);
  
  border_draw_[0]->draw(wb_->real_x() - (border_draw_[0]->length()>>1) - (length_border()>>1) - xodd,
			wb_->real_y() - (border_draw_[0]->height()>>1) - (height_border()>>1) - yodd, da);   
  
  border_draw_[1]->draw(wb_->real_x() + wb_->length() - (border_draw_[0]->length()>>1) + (length_border()>>1),
			wb_->real_y() - (border_draw_[0]->height()>>1) - (height_border()>>1) - yodd, da);
  
  border_draw_[2]->draw(wb_->real_x() - (border_draw_[0]->length()>>1) - (length_border()>>1) - xodd ,
			wb_->real_y() + wb_->height() - (border_draw_[0]->height()>>1) + (height_border()>>1), da); 
  
  border_draw_[3]->draw(wb_->real_x() + wb_->length() - (border_draw_[0]->length()>>1) + (length_border()>>1),
			wb_->real_y() + wb_->height() - (border_draw_[0]->height()>>1) + (height_border()>>1), da);  
}

void border::set_brightness_border(bool b)
{
  brightness_=b;
  refresh();
}

void border::refresh()
{
  border_draw_ = (brightness_) ? border_brightness_ : border_;
}
