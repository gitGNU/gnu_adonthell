/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include "types.h"
#include "image.h"
#include "win_types.h"
#include "win_base.h"
#include "win_manager.h"
#include "win_container.h"

list<win_container*> win_manager::lmanage;
win_container * win_manager::wc=NULL;

//add a container 
void win_manager::add(win_container * tmp)
{
  lmanage.push_back(tmp);
}

bool win_manager::exist(win_container * tmp)
{
  for(list<win_container *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    {
      if(*i==tmp) return true;
    }
  return false;
}

//Remove container 
void win_manager::remove(win_container * tmp)
{
  //if tmp has focus on, we set focus off and put ilm at nothing
  if(tmp->is_focus()) 
    {
      tmp->set_focus(false);
      wc=NULL;
    }
  lmanage.remove(tmp);
  if (!wc) set_focus (lmanage.back ());
}


void win_manager::destroy()
{
  for(list<win_container *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    delete *i;
  lmanage.clear();
  wc = NULL;
}

void win_manager::draw()
{
  for(list<win_container *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
      (*i)->draw();
}


void win_manager::input_update()
{  
  if(wc) {wc->input_update();} 
}

void win_manager::update()
{
  for(list<win_container *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    {
      if(!(*i)->update())
	{
	  if(*i==wc) wc=NULL;
	  remove(*i);
	  delete *i--;
	}
    }
  if (!wc) set_focus (lmanage.back ());
}

void win_manager::set_focus(win_container * tmp)
{
  if(!lmanage.empty())
    {
      if(wc) wc->set_focus(false);
      wc=tmp;
      wc->set_focus(true);
    }   
}
