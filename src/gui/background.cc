/*   
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#include <string.h>
#include "theme.h"
#include "background.h"
#include "base.h"

#include "../game.h"

using namespace gui;

background::background()
{
  wb_=NULL;

  init();

  set_visible_background(false);
  
  set_brightness_background(false);
  
  set_trans_background(true); 
}


background::background(base * wb)
{
  wb_=wb;

  init();

  set_visible_background(false);
  
  set_brightness_background(false);
  
  set_trans_background(true); 
}


background::background(char *rep)
{
  wb_=NULL;
 
  init();

  set_visible_background(false);
  
  set_brightness_background(false);
  
  set_trans_background(true);
  
  background::load(rep);
  
  refresh();
}


background::background(background & wb)
{
  wb_=NULL;

  init();
  
  set_visible_background(false);
  
  set_brightness_background(false);

  set_trans_background(true);

  *this=wb;
  
  refresh();
}


background::~background()
{
  destroy();
}


void background::destroy()
{
  if(background_template_) delete background_template_;
  if(background_) delete background_;
  if(background_brightness_) delete background_brightness_;
  init();
}


void background::init()
{
  background_template_=NULL;
  background_=NULL;
  background_brightness_=NULL;
  background_draw_=NULL;
}


background & background::operator=(background & wb)
{  
  destroy();
  background_template_=new gfx::image();
  *background_template_=*(wb.background_template_);
  background_=new gfx::image();
  background_->set_mask(false);
  background_brightness_= new gfx::image();
  background_brightness_->set_mask(true);
  
  update();
  
  return *this;
  
}


void background::set_background(background & wb)
{
  *this=wb;
  refresh();
}

void background::set_background(theme & wt)
{
  *this=*(wt.my_background);
  refresh();
}

void background::set_brightness_background(bool b)
{
  brightness_=b;
  refresh();
}

void background::refresh()
{
  background_draw_=(brightness_)? background_brightness_ : background_ ;
}

void background::load(char *rep)
{
  destroy();

  std::string basepath = DIRECTORY;
  basepath += BACKGROUND_DIRECTORY;
  basepath += rep;
  basepath += BACKGROUND_FILE; 

  basepath = game::find_file (basepath);
  
  background_template_=new gfx::image();
  background_template_->load_pnm(basepath.c_str ());//new
  
  background_= new gfx::image();
  background_->set_mask(true);
  
  background_brightness_= new gfx::image();
  background_brightness_->set_mask(true);
}


void background::update()
{
  if(!background_template_) return ;
  
  background_->resize(wb_->length(),wb_->height());
  background_->tile(*background_template_);
  background_brightness_->brightness(*background_,BRIGHTNESS_LEVEL);
}


void background::draw(gfx::drawing_area * da)
{
  if(!visible_ || !background_draw_ || !wb_) return; 
  background_draw_->draw(wb_->real_x(),wb_->real_y(),da);
}


void background::set_trans_background(bool b)
{
  if(!background_template_) return;
  if(b)
    {
      background_->set_alpha(130);
      background_brightness_->set_alpha(130);
    }
  else
    {
      background_->set_alpha(255);
      background_brightness_->set_alpha(255);
    }
}
