#include <string>
#include "types.h"
#include "input.h"
#include "image.h"
#include "win_types.h"
#include "win_font.h"
#include "win_border.h"
#include "win_base.h"
#include "win_label.h"
#include "win_write.h"

win_write::win_write(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth,win_font * wtf):win_label(tx,ty,tl,th,wth,wtf)
{
  ok_text_=false; 
  activate_keyboard_=true;
  type_obj_=WIN_OBJ_WRITE;
}

char * win_write::get_text_entry()
{
  if(ok_text_)
    {
      ok_text_=false;
      return texte_;
    }
  else return NULL;
}

void win_write::write()
{
  static s_int32 c;
  if(!activate_keyboard_ || !activated_) return;
  while((c=input::get_next_unicode())>0)
    {
      if(font_->in_table(c) || c==SDLK_BACKSPACE || c==SDLK_RETURN)
	if(c==SDLK_BACKSPACE && texte_size_>0) 
	  {
	    texte_[(texte_size_--)-1]='\0';
	    init_draw_surface();
	    init_draw();
	  }
	else 
	  {
	    if(c!=SDLK_BACKSPACE)
	      {
		if(c == SDLK_RETURN) 
		  {
		    if(callback_[WIN_SIG_ACTIVATE_KEY]) (callback_[WIN_SIG_ACTIVATE_KEY])();
		    else 
		      {
			ok_text_=true;
			//			add_text("\n");
		      }
		  }
		else add_text((char*)&c);
	      }
	  }
    } 
}

void win_write::update()
{
  win_label::update();
  write();
}
