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

list<win_base*> win_manager::lmanage;
win_base * win_manager::wc=NULL;

//add a container 
void win_manager::add(win_base * tmp)
{
  lmanage.push_back(tmp);
}

void win_manager::add_after (win_base * toadd, win_base * after)
{
    for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    {
        if (*i == after) 
        {
            i++; 
            lmanage.insert (i, toadd);
            break; 
        }
    }
}

bool win_manager::exist(win_base * tmp)
{
  for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    {
      if(*i==tmp) return true;
    }
  return false;
}

//Remove base 
void win_manager::remove(win_base * tmp)
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
  for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
    delete *i;
  lmanage.clear();
  wc = NULL;
}

void win_manager::draw()
{
  for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
      (*i)->draw();
}


void win_manager::input_update()
{  
  if(wc) {wc->input_update();} 
}

void win_manager::update()
{
  for(list<win_base *>::iterator i=lmanage.begin();i!=lmanage.end();i++)
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

void win_manager::set_focus(win_base * tmp)
{
  if(!lmanage.empty())
    {
      if(wc) wc->set_focus(false);
      wc=tmp;
      wc->set_focus(true);
    }   
}
