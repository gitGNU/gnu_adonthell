/* 
   $Id$ 

   (C) Copyright 2000, 2001 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.
   
   See the COPYING file for more details
*/

#include <iostream.h>
#include <string.h>
#include <list>
#include "types.h"
#include "image.h"
#include "input.h"
#include "win_types.h"
#include "win_theme.h"
#include "win_border.h"
#include "win_background.h"
#include "win_base.h"
#include "win_container.h"

#if defined (USE_PYTHON)
#include "py_callback.h"
#endif

#ifdef _DEBUG_WIN
long win_base::cptw=0;
#endif



win_base::win_base(s_int16 tx,s_int16 ty,u_int16 tl,u_int16 th,win_theme * wth)
{
#ifdef _DEBUG_WIN
  cout << "Number win object:" <<++cptw<< endl;
#endif
  //by default focus is off for all object
  focus_=false;
  
  //by default window is desactivated
  activated_=false;
  
  //by default window is not selected
  selected_=false;
  
  //by default windows is not in select object
  in_select_=false;
  
  //by default window is not visible
  visible_=false;
  
  //by default border is no visible
  visible_border_=false;
  
  //by default backgroud is not visible
  visible_background_=false;
  
  //by default window isn't draw in brigthness
  draw_brightness_=false;
  
  //by default an onbject can be selected
   can_be_selected_=true;
  
  //not have a father, -> window in(depend on) another window.
  wb_father_=NULL;
  
  //value used to transluency
  level_trans_back_=128;
  
  //value used to brightness
  level_brightness_=200;
  
  //by default the current object is selected by the border
  mode_select_=WIN_SELECT_MODE_BORDER;
  
  //by default border size used is normal size
  border_size_=WIN_SIZE_NORMAL;

  //set align object
  align_=WIN_ALIGN_NONE;

  //return code of the window
  return_code_ = 0;
  
  //padding X,Y
  padx_=0;
  pady_=0;
  
  //position of the windows, not real position, position in win_* into another win_container*
  x_=tx;
  y_=ty;
  
  //drawing area of the father object
  pda_=NULL;
  
  //drawing area of this object
  da_=new drawing_area();
  
  //create a theme for this object
  theme_=(wth?new win_theme(*wth):NULL);

  //resize the object
  resize(tl,th);

  //update the real position
  update_real_position();
}


win_base::~win_base()
{
  //notify that window is closing 
  if (callback_quit_) (callback_quit_) (return_code_);

#if defined (USE_PYTHON)
  //delete any python callbacks
  for (vector<py_callback *>::iterator i = py_callbacks.begin (); i != py_callbacks.end (); i++)
    delete *i;
#endif

  //delete cur drawing area
  delete da_;

  //delete theme
  delete theme_;

#ifdef _DEBUG_WIN
  cout << "Number win object:" << --cptw << endl;
#endif
}

void win_base::resize(u_int16 tl,u_int16 th)
{
  //resize
  length_=tl;
  height_=th;
  
  //resize drawing area too.
  da_->resize(tl,th);
  
  //modify the theme object --> in theme object there are the border, background size  and another .....
  if(theme_) theme_->update(this);  

  //update position
  update_align();
}


bool win_base::update()
{
  if(focus_ && activated_ && callback_[WIN_SIG_KEYBOARD])(callback_[WIN_SIG_KEYBOARD])();

  if(callback_destroy_ && !(callback_destroy_)()) return false;
  on_update();
  return true;
}


bool win_base::draw()
{
  on_draw();
  
  if(visible_) on_draw_visible();

  return visible_;
}

void win_base::set_theme(win_theme * th)
{
  //set the new theme ---> WARNING I think theme->UPDATE(THIS) IS NEEDED
  if (theme_ != NULL) 
  {
    if(th) *theme_=*th;
    else theme_=NULL;
  }
  else if (th) theme_ = new win_theme (*th);
  if (theme_) theme_->update (this);
}

void win_base::move(s_int16 tx,s_int16 ty)
{
  //move the position 
  x_=tx;y_=ty;

  //update position, if this object is a child
  update_real_position();
}

void win_base::update_drawing_area_position()
{
  //update drawing area to the real position
  da_->move(realx_,realy_);
}

void win_base::update_real_position()
{
  
  if(wb_father_) //if the object is a child
    {
      //get reference to drawing area of the father
      pda_=wb_father_->get_drawing_area();
      
      //calcul the real position,---->> WARNING padx, pady maybe not add in realx_,realy_
      realx_=x_+wb_father_->realx()+padx_;
      realy_=y_+wb_father_->realy()+pady_;
    }
  else
    {
      //the object not a child
      pda_=NULL;
      realx_=x_+padx_; //----> Same than upper
      realy_=y_+pady_;
    }
  //update drawing area
  update_drawing_area_position();
}

//add a signal 
void win_base::set_signal_connect(const Functor0 &func, u_int8 signal)
{
  callback_[signal] = func;
} 

#if defined (USE_PYTHON)
// Connect a signal to our window
void win_base::py_signal_connect (PyObject *pyfunc, int signal, PyObject *args)
{
  // create the callback
  py_callback *callback = new py_callback (pyfunc, args);
  py_callbacks.push_back (callback);

  // connect the signal
  switch (signal)
  {
    case WIN_SIG_CLOSE:
    {
      set_callback_quit (makeFunctor (*callback, &py_callback::callback_func1));
      break;
    }
    case WIN_SIG_DESTROY:
    {
      set_callback_destroy (
        MemberTranslator0wRet<bool, py_callback, bool (py_callback::*)()> (
          *callback, &py_callback::callback_func0ret));
        // makeFunctor (*callback, &py_callback::callback_func0ret));
      break;
    }
    default:
    {
      set_signal_connect (
        makeFunctor (*callback, &py_callback::callback_func0), signal);
    }
  }
}
#endif

//on activate 
void win_base::on_activate()
{
  if(callback_[WIN_SIG_ACTIVATE]) (callback_[WIN_SIG_ACTIVATE])();
}
 
void win_base::on_unactivate()
{
  if(callback_[WIN_SIG_UNACTIVATE]) (callback_[WIN_SIG_UNACTIVATE])();
}

void win_base::on_update()
{
  if(callback_[WIN_SIG_UPDATE]) (callback_[WIN_SIG_UPDATE])();
}

void win_base::on_draw()
{
  if(callback_[WIN_SIG_DRAW]) (callback_[WIN_SIG_DRAW])();
}

void win_base::on_draw_visible()
{
  if(callback_[WIN_SIG_DRAW_ONLY_VISIBLE]) (callback_[WIN_SIG_DRAW_ONLY_VISIBLE])();
}

void win_base::on_select()
{
  if(callback_[WIN_SIG_SELECT]) (callback_[WIN_SIG_SELECT])();
}

void win_base::on_unselect()
{
  if(callback_[WIN_SIG_UNSELECT]) (callback_[WIN_SIG_UNSELECT])();
}
 
void win_base::on_activate_key()
{
  if(callback_[WIN_SIG_ACTIVATE_KEY]) (callback_[WIN_SIG_ACTIVATE_KEY])();
}

void win_base::set_focus(bool b)
{
  if(focus_=b) input::clear_keys_queue();  
}

void win_base::set_select_mode_(u_int8 mode)
{
  mode_select_=mode;
  if(mode==WIN_SELECT_MODE_BRIGHTNESS) set_draw_brightness(true);
  else if(mode==WIN_SELECT_MODE_BORDER) set_border_visible(false);
} 


void win_base::update_align()
{
 
  switch(align_)
    {
    case WIN_ALIGN_LEFT:
      move((wb_father_)?((win_container*)wb_father_)->space_between_border():0,y());
      break;
    case WIN_ALIGN_RIGHT:
      move(((wb_father_)?wb_father_->length():screen::length())-((wb_father_)?((win_container*)wb_father_)->space_between_border():0)-length_,y());
      break;
    case WIN_ALIGN_CENTER:
      if(((wb_father_)?wb_father_->length():screen::length())>length()) 
	move((((wb_father_)?wb_father_->length():screen::length()) - length()) >>1,y());
      break;
    }
}




/*******************************************************/
/************************ DRAW *************************/
/*******************************************************/
void win_base::draw_border()
{
#define WB_CORNER_MIDX (border->corner_top_left->length()>>1)
#define WB_CORNER_MIDY (border->corner_top_left->height()>>1)
#define WB_VBORDERX (border->v_border_template->length()>>1)
#define WB_HBORDERY (border->h_border_template->height()>>1) 
  //if no theme, drawing nothing
  if(!theme_ || !visible_border_) return;
  
  //create a temp border
  win_border * border=NULL;
  
  //if border size is mini, border take the minborder of the theme, is normal tka normal border
  if(border_size_==WIN_SIZE_MINI) border=theme_->mini;
  else if(border_size_==WIN_SIZE_NORMAL) border=theme_->normal;
  
  //create a static image to draw border
  static image imgbright;  
  
  switch(draw_brightness_)
  {
      case true:
          imgbright.brightness(* (border->h_border),level_brightness_);
          imgbright.set_mask (true);  
          imgbright.draw(realx_,realy_-border->h_border->height(),pda_);
          imgbright.draw(realx_,+realy_+height_,pda_);
          
          imgbright.brightness(* (border->v_border) ,level_brightness_);
          imgbright.set_mask (true); 
          imgbright.draw(realx_-border->v_border->length(),realy_,pda_);
          imgbright.draw(realx_+length_,realy_,pda_);
          
          imgbright.brightness(* (border->corner_top_left) ,level_brightness_);
          imgbright.set_mask (true); 
          imgbright.draw(realx_-WB_CORNER_MIDX-((border->v_border->length())>>1),
                         realy_-WB_CORNER_MIDY-(border->h_border->height()>>1),pda_);   
          
          imgbright.brightness(* (border->corner_top_right) ,level_brightness_);
          imgbright.set_mask (true); 
          imgbright.draw(realx_+length_-WB_CORNER_MIDX+(border->v_border->length()>>1),
                         realy_-WB_CORNER_MIDY-(border->h_border->height()>>1),pda_); 
          
          imgbright.brightness(* (border->corner_bottom_left) ,level_brightness_);
          imgbright.set_mask (true); 
          imgbright.draw(realx_-WB_CORNER_MIDX-((border->v_border->length())>>1),
                         realy_+height_-WB_CORNER_MIDY+(border->h_border->height()>>1),pda_);
          
          imgbright.brightness(* (border->corner_bottom_right),level_brightness_);
          imgbright.set_mask (true); 
          imgbright.draw(realx_+length_-WB_CORNER_MIDX+(border->v_border->length()>>1),
                         realy_+height_-WB_CORNER_MIDY+(border->h_border->height()>>1),pda_);
          break;
      case false:
          border->h_border->set_mask (true);
          border->h_border->draw(realx_,realy_-border->h_border->height(),pda_);
          border->h_border->draw(realx_,+realy_+height_,pda_);
          
          border->v_border->set_mask (true);
          border->v_border->draw(realx_-border->v_border->length(),realy_,pda_);
          border->v_border->draw(realx_+length_,realy_,pda_);
          
          border->corner_top_left->set_mask (true); 
          border->corner_top_left->draw(realx_-WB_CORNER_MIDX-((border->v_border->length())>>1),
                                        realy_-WB_CORNER_MIDY-(border->h_border->height()>>1),pda_);   
          
          border->corner_top_right->set_mask (true); 
          border->corner_top_right->draw(realx_+length_-WB_CORNER_MIDX+(border->v_border->length()>>1),
                                         realy_-WB_CORNER_MIDY-(border->h_border->height()>>1),pda_);
          
          border->corner_bottom_left->set_mask (true); 
          border->corner_bottom_left->draw(realx_-WB_CORNER_MIDX-((border->v_border->length())>>1),
                                           realy_+height_-WB_CORNER_MIDY+(border->h_border->height()>>1),pda_); 
          
          border->corner_bottom_right->set_mask (true); 
          border->corner_bottom_right->draw(realx_+length_-WB_CORNER_MIDX+(border->v_border->length()>>1),
                                            realy_+height_-WB_CORNER_MIDY+(border->h_border->height()>>1),pda_);
      break;
    }
}


void win_base::draw_background()
{
  //if not theme return
  if(!theme_)return;
  static image imgbright;

  //if is a visible background and if in win_select and this object is not select and mode select is brigthness so drawthe background in brightness mode
  if(visible_background_ && in_select_ && (!selected_) && mode_select_==WIN_SELECT_MODE_BRIGHTNESS)
    {
      imgbright.brightness(* (theme_->background->background),level_brightness_);
      imgbright.set_alpha (level_trans_back_); 
      imgbright.draw(realx_,realy_,da_);
    }
  //draw the background
  else if(visible_background_)
  {
      theme_->background->background->set_alpha (level_trans_back_); 
      theme_->background->background->draw(realx_,realy_,da_);
  }
}
