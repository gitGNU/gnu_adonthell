/*
   (C) Copyright 2000, 2001 Joel Vennin
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _WIN_EVENT_H_
#define _WIN_EVENT_H_

#include <locale>
#include "Python.h"
#include <vector>

class py_callback;


#include "types.h"
#include "callback.h"


using namespace std; 

class win_event
{
 public:
  
  win_event(){return_code_=0;}

  //Use this function to set a callback function
  void set_return_code (int rc) 
    { return_code_ = rc; }

#ifndef SWIG
  void set_signal_connect (const Functor0 &func, u_int8 signal)
    { callback_[signal] = func; } 
  void set_callback_destroy (const Functor0wRet<bool> &func)
    { callback_destroy_ = func; }
  void set_callback_quit (const Functor1<int> &func)
    { callback_quit_ = func;}
#endif
  
  bool update();
  
  void py_signal_connect (PyObject *pyfunc, int signal, PyObject *args = NULL); 
  
  const static u_int8 ACTIVATE =1 ;
  const static u_int8 UNACTIVATE = 2;
  const static u_int8 UPDATE = 3;
  const static u_int8 DRAW = 4;
  const static u_int8 DRAW_ON_VISIBLE = 5;
  const static u_int8 ACTIVATE_KEY = 6;
  const static u_int8 SELECT = 7;
  const static u_int8 UNSELECT = 8;
  const static u_int8 KEYBOARD = 9;
  const static u_int8 SCROLL_UP = 10;
  const static u_int8 SCROLL_DOWN = 11;
  const static u_int8 NEXT = 12;
  const static u_int8 PREVIOUS = 13;
  const static u_int8 CLOSE = 14;
  const static u_int8 DESTROY = 15;

  /*****************************************************/
  /*****************************************************/
  //DESTRUCTOR
  virtual ~win_event();
  
 protected:
  // the python callbacks connected to the window
  vector<py_callback *> py_callbacks;   
  
  
  Functor0 callback_[20];
  Functor0wRet<bool> callback_destroy_;
  Functor1<int> callback_quit_;
  
  
  int return_code_;
  
  //execute the callback function
  virtual void on_activate(){ if(callback_[ACTIVATE]) (callback_[ACTIVATE])();}
  virtual void on_unactivate(){ if(callback_[UNACTIVATE]) (callback_[UNACTIVATE])();}
  
  virtual void on_update() { if(callback_[UPDATE]) (callback_[UPDATE])();}
  
  virtual void on_draw_visible(){ if(callback_[DRAW_ON_VISIBLE]) (callback_[DRAW_ON_VISIBLE])();}
  virtual void on_draw(){ if(callback_[DRAW]) (callback_[DRAW])();}
  
  virtual void on_activate_key(){ if(callback_[ACTIVATE_KEY]) (callback_[ACTIVATE_KEY])();}
  virtual void on_select(){ if(callback_[SELECT]) (callback_[SELECT])();}
  virtual void on_unselect(){ if(callback_[UNSELECT]) (callback_[UNSELECT])();}
  
  virtual void on_up(){if(callback_[SCROLL_UP]) (callback_[SCROLL_UP])();}
  virtual void on_down(){if(callback_[SCROLL_DOWN]) (callback_[SCROLL_DOWN])();}
  
  virtual void on_next(){if(callback_[NEXT]) (callback_[NEXT])();}
  virtual void on_previous(){if(callback_[PREVIOUS]) (callback_[PREVIOUS])();}
};



#endif


