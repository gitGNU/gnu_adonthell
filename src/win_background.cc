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
#include "win_theme.h"
#include "win_background.h"
#include "win_base.h"

win_background::win_background()
{
  wb_=NULL;

  init();

  set_visible_background(false);
  
  set_brightness_background(false);
  
  set_trans_background(true); 
}


win_background::win_background(win_base * wb)
{
  wb_=wb;

  init();

  set_visible_background(false);
  
  set_brightness_background(false);
  
  set_trans_background(true); 
}


win_background::win_background(char *rep)
{
  wb_=NULL;
 
  init();

  set_visible_background(false);
  
  set_brightness_background(false);
  
  set_trans_background(true);
  
  win_background::load(rep);
  
  refresh();
}


win_background::win_background(win_background & wb)
{
  wb_=NULL;

  init();
  
  set_visible_background(false);
  
  set_brightness_background(false);

  set_trans_background(true);

  *this=wb;
  
  refresh();
}


win_background::~win_background()
{
  destroy();
}


void win_background::destroy()
{
  if(background_template_) delete background_template_;
  if(background_) delete background_;
  if(background_brightness_) delete background_brightness_;
  init();
}


void win_background::init()
{
  background_template_=NULL;
  background_=NULL;
  background_brightness_=NULL;
  background_draw_=NULL;
}


win_background & win_background::operator=(win_background & wb)
{  
  destroy();
  background_template_=new image();
  *background_template_=*(wb.background_template_);
  background_=new image();
  background_->set_mask(false);
  background_brightness_= new image();
  background_brightness_->set_mask(true);
  
  update();
  
  return *this;
  
}


void win_background::set_background(win_background & wb)
{
  *this=wb;
  refresh();
}

void win_background::set_background(win_theme & wt)
{
  *this=*(wt.background);
  refresh();
}

void win_background::set_brightness_background(bool b)
{
  brightness_=b;
  refresh();
}

void win_background::refresh()
{
  background_draw_=(brightness_)? background_brightness_ : background_ ;
}

void win_background::load(char *rep)
{
  destroy();
  char path[255];
  strcpy(path,WIN_DIRECTORY);
  strcat(path,WIN_BACKGROUND_DIRECTORY);
  strcat(path,rep);
  strcat(path,WIN_BACKGROUND_FILE);
  background_template_=new image();
  background_template_->load_pnm(path);//new
  
  background_=new image();
  background_->set_mask(true);
  
  background_brightness_=new image();
  background_brightness_->set_mask(true);
}


void win_background::update()
{
  if(!background_template_) return ;
  
  background_->resize(wb_->length(),wb_->height());
  background_->tile(*background_template_);
  background_brightness_->brightness(*background_,WIN_BRIGHTNESS_LEVEL);
}


void win_background::draw(drawing_area * da = NULL)
{
  if(!visible_ || !background_draw_ || !wb_) return; 
  background_draw_->draw(wb_->real_x(),wb_->real_y(),da);
}


void win_background::set_trans_background(bool b)
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








