#include "label_input.h"




void label_input::insert(const u_int16 pos,const char * letter)
{
  text_.insert(pos,letter);
  
  if(cursor_pos_ == text_size_)
    {
      text_size_= text_.length();
      cursor_pos_ = text_size_ ;
    }
  else text_size_= text_.length();

  check_form();
}


void label_input::insert_char(const u_int16 pos,const char letter)
{
  text_.insert(pos,&letter);
  
  if(cursor_pos_ == text_size_)
    {
      text_size_= text_.length();
      cursor_pos_ = text_size_ ;
    }
  else text_size_= text_.length();
  
  check_form();
}

bool label_input::input_update()
{
  
  label::input_update();
  
  if(!font_) return false;
  
  static s_int32 c;
  
  while((c=input::get_next_unicode())>0)
    {
      //cout << "Fuck1 \n";
      
      if(c==SDLK_BACKSPACE && text_size_>0)
	{
	  text_.erase(--cursor_pos_, 1);
	  text_size_ = text_.length();
	}
      else if(c == SDLK_RETURN) insert_char(cursor_pos_,'\n');
      else if(font_->in_table(c)) insert(cursor_pos_, (char*) &c);

      //cout << "Fuck2 \n";
    }
  return true;
}



























































































































