/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/
#include <string>
#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"
#include "win_base.h"
#include "win_theme.h"
#include "win_label.h"

win_label::win_label(s_int16 tx ,s_int16 ty ,u_int16 tl,u_int16 th,win_theme * wthem, win_font * fo)
  :win_base(tx,ty,tl,th,wthem)
{

  //set blinkcursor_ to false.. no cursor for a label
  blinkcursor_=false;
  blinkcursortimetodraw_=true;
  blinkinc_=0;
  blinkingspeed_=WIN_CURSOR_BLINKING;

  //attach font object
  font_=fo;

  //window propertie
  auto_height_=false;
  auto_size_=false;
  
  //template, this image is representation of the contain of the label
  template_ = new image();
  template_->resize(tl,th);
  
  //set text to nothing
  set_text("");
}


win_label::~win_label()
{
  //destroy the template image
  if(template_) delete template_;
  font_=NULL;
}


//return real size of a word
s_int16 win_label::word_size(u_int16 begin,u_int16 & length)
{
  //if position of index is superior than text_size return -1 : error
  if(begin>=texte_size_)return -1;

  //set size to 0
  u_int16 size=0;

  //length to 0
  length=0;
  
  //size is real size of word, length the nb of charactere of the word

  //while size of length + begin word inferior at text size and charatere != '\n' !=' ' continue to calculate size of word
  while(begin+length<texte_size_ && texte_[begin+length]!='\n' && texte_[begin+length]!=' ')
    {
      if(font_->in_table(texte_[begin+length])) 
	//size+=font_->table[texte_[begin+length]].get_length();
	size+=(*font_)[texte_[begin+length]].length();

	length++;
    }

  //return real size of the word
  return size;
} 


//0: cut the word
//1: Ok
//2: Next Line
//this fonction determine what we can do with the cur word

s_int8 win_label::word_place(u_int16 cur_line_size,s_int16 w_size)
{
  if(w_size<0) return -1;
  if(w_size>length_) return 0;
  if(w_size>length_-cur_line_size) return 2;
  return 1;
}

void win_label::resize(u_int16 tl,u_int16 th)
{
  //resize win_base.....
  win_base::resize(tl,th);

  //resize template
  template_->resize(tl,th);

  //calculate surface
  init_draw_surface();

  //draw
  init_draw();
}

void win_label::init_draw_surface()
{
  u_int16 tmplength_=0,tmpheight_=0,tmpwsize_;
  u_int16 i=0,curligne_=0,wnbch_;
  if(!auto_size_ && auto_height_)
    {
      
      while(i<texte_size_)
	{
	  if(texte_[i]=='\n') {curligne_=0;tmpheight_+=font_->height();i++;}
	  else if(texte_[i]==' ')
	    {
	      if(curligne_+font_->length()<length_) curligne_+=font_->length();
	      else {
		curligne_=0;tmpheight_+=font_->height();
	      }
	      i++;
	    }
	  
	  switch(word_place(curligne_,(tmpwsize_=word_size(i,wnbch_))))
	    {
	    case 0:
	      for(int j=i;j<wnbch_+i;j++)
		{
		  if(font_->in_table(texte_[j]))
		    {
		      if(curligne_+(*font_)[texte_[j]].length()>length_) 
			{
			  curligne_=0;tmpheight_+=font_->height();
			}
		      curligne_+=(*font_)[texte_[j]].length();
		    }
		}
	      i+=wnbch_;
	      break;
	    case 1:
	      curligne_+=tmpwsize_;i+=wnbch_;
	      break;
	    case 2:
	      curligne_=tmpwsize_;i+=wnbch_;tmpheight_+=font_->height();
	      break;
	    default:
	      break;
	    }
	}
      
      //the blink cursor
        if(blinkcursor_)
	{
	  if(curligne_+font_->cursor->length()>length_) tmpheight_+=font_->height();
	}
      
      if(auto_height_ && height_!=tmpheight_+font_->height())
	{
	  win_base::resize(length_,tmpheight_+font_->height()); 
	  template_->clear();
	  template_->resize(length_,tmpheight_+font_->height()); 
	}
    }
  else
    { //if autosize
      if(auto_size_)
	{
	  while(i<texte_size_)
	    {
	      if(texte_[i]!='\n' && texte_[i]!=' ' && font_->in_table(texte_[i])) curligne_+=(*font_)[texte_[i]].length();
	      else if(texte_[i]=='\n')
		{
		  tmpheight_+=font_->height();
		  if(curligne_>tmplength_) tmplength_=curligne_;
		  curligne_=0;
		}
	      else if(texte_[i]==' ') curligne_+=font_->length();
	      i++;
	    }
	  

	  //the blink cursor
	  if(blinkcursor_)
	    {
	      if(curligne_+font_->cursor->length() > tmplength_) tmplength_=curligne_+font_->cursor->length();
	    }
	  else if(curligne_>tmplength_) tmplength_=curligne_;


	  
	  if(tmpheight_!=height_+font_->height() || tmplength_!=length_) 
	    {
	      win_base::resize(tmplength_,tmpheight_+font_->height()); 
	      template_->clear();
	      template_->resize(tmplength_,tmpheight_+font_->height());
	    }
	}
    }
}


void win_label::init_draw()
{
  u_int16 tmpheight_=0,tmpwsize_;
  u_int16 i=0,curligne_=0,wnbch_;
  static u_int16 j;
  for (u_int16 k = 0; k < height_; k++)
    for (j = 0; j < length_; j++)
      template_->put_pix(j, k,screen::trans_col());
  while(i<texte_size_)
    {
      if(texte_[i]=='\n') {curligne_=0;tmpheight_+=font_->height();i++;}
      else if(texte_[i]==' ')
	{
	  if(curligne_+font_->length()<length_) curligne_+=font_->length();
	  else 
	    {
	      curligne_=0;tmpheight_+=font_->height();
	    }
	  i++;
	}
      if(tmpheight_>=template_->height()) break;
      switch(word_place(curligne_,(tmpwsize_=word_size(i,wnbch_))))
	{
	case 0:
	  for(j=i;j<wnbch_+i;j++)
	    {
	      if(font_->in_table(texte_[j]))
		{
		  if(curligne_+(*font_)[texte_[j]].length()>length_) 
		    {curligne_=0;tmpheight_+=font_->height();}
		  if(tmpheight_>=template_->height()) break;
		  template_->putbox_img(&((*font_)[texte_[j]]),curligne_,tmpheight_);
		  curligne_+=(*font_)[texte_[j]].length(); 
		}
	    }
	  i+=wnbch_;
	  break;
	case 1:
	  for(j=i;j<wnbch_+i;j++)
	    {  
	      if(font_->in_table(texte_[j]))
		{
		  template_->putbox_img(&((*font_)[texte_[j]]),curligne_,tmpheight_);
		  curligne_+=(*font_)[texte_[j]].length();
		}
	    }
	  i+=wnbch_;
	  break;
	case 2:
	  tmpheight_+=font_->height();
	  curligne_=0;
	  if(tmpheight_>=template_->height()) break;
	  for(j=i;j<wnbch_+i;j++)
	    {
	      if(font_->in_table(texte_[j]))
		{
		  template_->putbox_img(&((*font_)[texte_[j]]),curligne_,tmpheight_);
		  curligne_+=(*font_)[texte_[j]].length();
		}
	    }
	  i+=wnbch_;
	  break;
	default:
	  
	  break;
	}
    }
  
 
  //insert the blink cursor
  if(focus_ && blinkcursortimetodraw_ && blinkcursor_ && template_->length()!=0)
    {
      //modify because error if not autosize
      if(auto_size_) template_->putbox_img(font_->cursor,curligne_,tmpheight_);
      else {
	if(curligne_+font_->cursor->length()>length_) {
	  tmpheight_+=font_->height();
	  curligne_=0;
	}
	template_->putbox_img(font_->cursor,curligne_,tmpheight_);
      }
    }
}



bool win_label::draw()
{
  if(win_base::draw())
    {
      assign_drawing_area();
      draw_background();
      if(template_)
	if(draw_brightness_)// && (!in_select_ ||(in_select_ && !selected_ && can_be_selected_)))
	  {
	    static image imgbright;
	    imgbright.brightness(template_,level_brightness_);
	    imgbright.putbox_mask(realx_,realy_,da_);
	  }
	else template_->putbox_mask(realx_,realy_,da_);
      draw_border();
      detach_drawing_area();
      return true;
    }
  return false;
}

void win_label::set_text(const char * tmp)
{
  strcpy(texte_,tmp);
  texte_size_=strlen(texte_);
  init_draw_surface();
  init_draw();
}

void win_label::add_text(const char * tmp)
{
  strcat(texte_,tmp);
  texte_size_=strlen(texte_);
  init_draw_surface();
  init_draw();
}

void win_label::set_auto_size(bool b)
{
  auto_size_=b;
  if(b) auto_height_=false;
  init_draw_surface();
  init_draw();
}

void win_label::set_auto_height(bool b)
{
  auto_height_=b;
  if(b) auto_size_=false;
  init_draw_surface();
  init_draw();
}

bool win_label::update()
{
  if(win_base::update())
    {
      bool old=blinkcursortimetodraw_;
      if(visible_ &&blinkcursor_)
	{
	  if(blinkinc_++>blinkingspeed_) blinkinc_=0;
	  blinkcursortimetodraw_=(blinkinc_>(blinkingspeed_>>1));
	  if(old!=blinkcursortimetodraw_) init_draw();
	}
      return true;
    }
  return false;
}
















