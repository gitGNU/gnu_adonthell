/*
   $Id$

   (C) Copyright 2000/2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "label.h"

u_int16 label::cursor_blink_speed_ = 50;

label::label()
{
  font_ = NULL;
  
  form_ = NOTHING;
  
  text_size_ = 0;

  text_index_begin_ = 0;
  
  text_eot_ = false;
  
  cursor_ = false;
  
  cursor_moveable_ = false;
  
  cursor_visible_ = false;

  cursor_blink_cur_ = 0;

  cursor_pos_ = 1;

  set_mask(true);
}


bool label::update()
{
  if(cursor_)
    {
      if(cursor_blink_cur_ > cursor_blink_speed_)
	{
	  cursor_blink_cur_ = 0;
	  cursor_visible_ = !cursor_visible_;
	  check_form();
	}
      cursor_blink_cur_++;
    }
  return true;
}


bool label::input_update()
{
  if( cursor_ && cursor_moveable_ )
    {
      if(input::has_been_pushed(KEY_CURSOR_NEXT)) 
      {
          cursor_blink_cur_ = 0;
          cursor_visible_ = true; 
          cursor_next();
          check_form (); 
      }
      else if(input::has_been_pushed(KEY_CURSOR_PREVIOUS)) 
      {
          cursor_blink_cur_ = 0; 
          cursor_visible_ = true; 
          cursor_previous();
          check_form (); 
      }
    }
  return true;
}


void label::set_font(win_font & font)
{
  font_=&font;
}

void label::set_text(const string & str)
{
  text_ = str;
  
  text_size_= text_.length();
  
  text_index_begin_ = 0;
  
  cursor_pos_ = text_size_ ;
  
  check_form();
}

void label::set_text(const char * str)
{
  text_ = str;
  
  text_size_= text_.length();
  
  text_index_begin_ = 0;

  cursor_pos_ = text_size_ ;

  check_form();
}


void label::add_text(const string & str)
{
  text_ += str;
  
  if(cursor_pos_ == text_size_) 
    {
      text_size_= text_.length();
      cursor_pos_ = text_size_ ;
    }
  else text_size_= text_.length();

  check_form();
}


void label::add_text(const char * str)
{
  text_ += str;
  
  if(cursor_pos_ == text_size_)
    {
      text_size_= text_.length();
      cursor_pos_ = text_size_ ;
    }
  else text_size_= text_.length();

  check_form();
}


void label::set_form(const u_int8 form)
{
  form_ = form;
  
  check_form();
}


void label::set_eot(const bool b)
{
  text_eot_ = b;
  check_form();
}


void label::resize(const u_int16 l,const u_int16 h)
{
  image::resize(l,h);
  check_form();
}


void label::find_word(u_int16 & begin, u_int16 &length, u_int16 & size_pix)
{
  while( (begin < text_size_) && (text_[begin]!=' ') && (text_[begin]!='\n') )
    {
      if(font_->in_table(text_[begin])) size_pix += (*font_)[text_[begin]].length();
      length++;
      begin++;
    }
}


u_int8 label::word_place( u_int16 & cur_line_size, u_int16 & word_size )
{
  if(word_size > length()) return 0;
  
  if(word_size > length() - cur_line_size) return 2;
  
  return 1;
}


void label::form_auto_height()
{
  u_int16 word_length;
  
  u_int16 j;

  u_int16 word_length_pix;

  u_int16 tmp_length_pix = 0;
  
  u_int16 tmp_height_pix = 0;
  
  u_int16 i = 0;
  
  while (i < text_size_)
    {
      if(text_[i] == '\n')
	{
	  tmp_length_pix = 0;
	  
	  tmp_height_pix += font_->height();
	  
	  i++;
	}
      else if(text_[i] == ' ')
	{
	  
	  if( (*font_)[' '].length() + tmp_length_pix > length()) 
	    {
	      tmp_length_pix = 0;
	      
	      tmp_height_pix += font_->height();
	      
	    } else tmp_length_pix += (*font_)[' '].length(); 
	  
	  i++;
	}
      else
	{
	  //word size
	  word_length = 0;
	  
	  word_length_pix = 0;
	  
	  find_word(i, word_length, word_length_pix); 
	  
	  //word place
	  switch( word_place(tmp_length_pix, word_length_pix) )
	    {
	      
	    case 0: //word is bigger than length()
	      
	      j = i - word_length; 
	      
	      while(  j < i )
		{
		  if(tmp_length_pix + (*font_)[text_[j]].length() > length()) 
		    {
		      tmp_length_pix = 0;
		      
		      tmp_height_pix += font_->height();
		      
		    } else tmp_length_pix += (*font_)[text_[j]].length();
		  j++;
		}
	      
	      break;
	      
	    case 1: //enough place
	      
	      tmp_length_pix += word_length_pix; 
	      
	      break;
	      
	    case 2://put the word at the next line
	      
	      tmp_height_pix += font_->height();
	      
	      tmp_length_pix = word_length_pix; 
	      
	      break;
	    }
	} 
    }  

  
  tmp_height_pix += font_->height();
  
  //Resize
  image::resize(length(), tmp_height_pix);
  
  //init image
  if(length() && height())
    {
      lock();
      fillrect(0,0,length(),height(),screen::trans_col());
      unlock();
    }
  
  //now draw
  i = 0;  
  tmp_height_pix = 0;
  tmp_length_pix = 0;
  
  while (i < text_size_)
    {
      if(text_[i] == '\n')
	{
	  tmp_length_pix = 0;
	  
	  tmp_height_pix += font_->height(); 
	
	  i++;
	}
      else if(text_[i] == ' ')
	{
	  if( i == cursor_pos_ && cursor_ && cursor_visible_)
	    font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this); 

	  if( (*font_)[' '].length() + tmp_length_pix > length()) 
	    {
	      tmp_length_pix = 0;
	      
	      tmp_height_pix += font_->height();
	    } 
	  else tmp_length_pix += (*font_)[' '].length();  
	  i++;
	}
      else
	{
	  find_word(i, word_length = 0, word_length_pix = 0); 
	  
	  j = i - word_length; 
	  
	  //word place
	  switch( word_place(tmp_length_pix, word_length_pix) )
	    {
	     
	    case 0: //word is bigger than length()
	      while(  j < i )
		{
		  if(tmp_length_pix + (*font_)[text_[j]].length() > length()) 
		    {
		      
		      tmp_length_pix = 0;
		      
		      tmp_height_pix += font_->height();
		      
		    } 
		  
		  if( j == cursor_pos_ && cursor_ && cursor_visible_) font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this); 
		  else (*font_)[text_[j]].draw( tmp_length_pix, tmp_height_pix, NULL, this);
 
		  tmp_length_pix += (*font_)[text_[j]].length();
		  
		  j++;
		}
	      
	      break;

	    case 1: //enough place
	      
	      while( j < i )
		{
		  if( j == cursor_pos_ && cursor_ && cursor_visible_) font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this); 
		  else (*font_)[text_[j]].draw( tmp_length_pix, tmp_height_pix, NULL, this); 

		  tmp_length_pix += (*font_)[text_[j]].length(); 
		  
		  j++;
		}
	      
	      break;
	      
	    case 2://put the word at the next line
	      tmp_height_pix += font_->height();
	      tmp_length_pix = 0;
      
	      while( j < i )
		{
		  if( j == cursor_pos_ && cursor_ && cursor_visible_) font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this); 
		  else (*font_)[text_[j]].draw( tmp_length_pix, tmp_height_pix, NULL, this); 

		  tmp_length_pix += (*font_)[text_[j]].length();
		  
		  j++;
		}
	      break;
	    }
	} 
    }  
  
  if(cursor_ && cursor_visible_ && cursor_pos_ == text_size_ )
    font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this);    
}




void label::form_nothing()
{
  u_int16 word_length;
  
  u_int16 j;
  
  u_int16 word_length_pix;
  
  u_int16 tmp_length_pix = 0;
  
  u_int16 tmp_height_pix = 0;
  
  u_int16 i = text_index_begin_;
  
  text_index_end_ = 0;
  
  //initialze the image
  if(length() && height())
    {
      lock();
      fillrect(0,0,length(),height(),screen::trans_col());
      unlock();
    }
  
  
  while ( (i < text_size_) && ( tmp_height_pix <= height() - font_->height()) )
    {
      if(text_[i] == '\n');
      
      if(text_[i] == '\n') 
	{
	  tmp_length_pix = 0;
	  
	  tmp_height_pix += font_->height(); 
	  
	  i++;
	}
      else if(text_[i] == ' ')
	{
	  
	  if( j == cursor_pos_ && cursor_ && cursor_visible_)
	    font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this); 
	  

	  if( (*font_)[' '].length() + tmp_length_pix > length()) 
	    {
	      tmp_length_pix = 0;
	      
	      tmp_height_pix += font_->height();
	    } 
	  else tmp_length_pix += (*font_)[' '].length();  
	  
	  i++;
	}
      else
	{
	  
	  find_word(i, word_length = 0, word_length_pix = 0); 
	  
	  j = i - word_length; 
	  
	  //word place
	  switch( word_place(tmp_length_pix, word_length_pix) )
	    {
	      
	    case 0: //word is bigger than length()
	      while(  j < i )
		{
		  if(tmp_length_pix + (*font_)[text_[j]].length() > length()) 
		    {
		      
		      tmp_length_pix = 0;
		      
		      tmp_height_pix += font_->height();
		      
		      if( tmp_height_pix > height() - font_->height() )
			{
			  text_index_end_ = j;
			  break;
			}
		    }
		  
		  (*font_)[text_[j]].draw( tmp_length_pix, tmp_height_pix, NULL, this);
		  
		  if( j == cursor_pos_ && cursor_ && cursor_visible_)
		    font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this); 
		  
		  tmp_length_pix += (*font_)[text_[j]].length();
		  
		  j++;
		}
	      
	      break;
	      
	    case 1: //enough place
	      
	      while( j < i )
		{
		  (*font_)[text_[j]].draw( tmp_length_pix, tmp_height_pix, NULL, this); 
		  
		  if( j == cursor_pos_ && cursor_ && cursor_visible_)
		    font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this); 
		  
		  tmp_length_pix += (*font_)[text_[j]].length();
		  
		  j++;
		}
	      
	      break;
	      
	    case 2://put the word at the next line
	      tmp_height_pix += font_->height();
	      tmp_length_pix = 0;
	      
	      if( tmp_height_pix > height() - font_->height()) text_index_end_ = j;
	      
	      while( j < i && tmp_height_pix <= height() - font_->height() )
		{
		  (*font_)[text_[j]].draw( tmp_length_pix, tmp_height_pix, NULL, this); 
		  
		  if( j == cursor_pos_ && cursor_ && cursor_visible_)
		    font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this); 
		  
		  tmp_length_pix += (*font_)[text_[j]].length();
		  
		  j++;
		}
	      
	      break;
	    }
	}  
    }
  
  
  if(cursor_ && cursor_visible_ && cursor_pos_ == text_size_ )
    font_->cursor->draw(tmp_length_pix, tmp_height_pix, NULL, this);  
  
  if( text_index_end_ == 0 ) text_index_end_ = i;
  
  if( text_eot_ )
    {
      if(text_index_end_ < text_size_)
	{
	  text_index_begin_ += ( text_size_ - text_index_end_ );
	  form_nothing();
	}
      // else if( (text_index_end_ == text_size_ ) && height()-tmp_height_pix >= font_->height() && text_index_begin_ >0)
      //{ text_index_begin_--;form_nothing();}
  
    }
  else if(cursor_ && cursor_moveable_)
    {
      if(cursor_pos_< text_index_begin_ && cursor_pos_ > 0) {text_index_begin_ =  cursor_pos_ - 1; form_nothing();} 
      else if (cursor_pos_ > text_index_end_ && cursor_pos_ != text_size_) {text_index_begin_ += (cursor_pos_ - text_index_end_) + 1;form_nothing(); }
    }
}


void label::form_auto_size()
{
  u_int16 tmp_length_pix = 0;
  
  u_int16 tmp_length_pix_max = 0;
  
  u_int16 tmp_height_pix = 0;
  
  u_int16 i = 0;
  
  while( i < text_size_ )
    {
      if(text_[i] == '\n') 
	{
	  
	  if(tmp_length_pix > tmp_length_pix_max ) tmp_length_pix_max=tmp_length_pix;
	  
	  tmp_length_pix = 0;
	  
	  tmp_height_pix += font_->height();
	  
	}
      else if(font_->in_table(text_[i])) tmp_length_pix += (*font_)[text_[i]].length(); 
      
      i++;     
    }
  
  if(tmp_length_pix > tmp_length_pix_max ) tmp_length_pix_max = tmp_length_pix;
  
  tmp_height_pix += font_->height();
  
  //resize
  image::resize(tmp_length_pix_max, tmp_height_pix);

  //init image
  if(length() && height())
    {
      lock();
      fillrect(0,0,length(),height(),255,0,255);
      unlock();
    }
  
  //draw
  i = 0;
  
  tmp_length_pix = 0;
  
  tmp_height_pix = 0;
  
  while(i < text_size_)
    {
      
      if(text_[i] == '\n') 
	{
	  
	  tmp_length_pix = 0;
	  
	  tmp_height_pix+=font_->height();
	  
	}
      else if(font_->in_table(text_[i])) 
	{
	  (*font_)[text_[i]].draw( tmp_length_pix, tmp_height_pix, NULL, this);
	  
	  tmp_length_pix += (*font_)[text_[i]].length();
	}
      i++;
    }
}


void label::check_form()
{
  if(!font_) return;
  
  switch(form_)
    {
    case AUTO_HEIGHT:
      form_auto_height();
      break;
      
    case AUTO_SIZE:
      form_auto_size();
      break;
    
    case NOTHING:
      form_nothing();
      break;
    }
}



void label::cursor_next()
{
  if(cursor_pos_ < text_size_ ) cursor_pos_++; 
}


void label::cursor_previous()
{
  if(cursor_pos_ > 1) cursor_pos_--;
}






