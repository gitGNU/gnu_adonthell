/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#include <iostream.h>
#include <list>
#include "types.h"
#include "image.h"
#include "input.h"
#include "win_types.h"
#include "win_font.h"
#include "win_border.h"
#include "win_base.h"
#include "win_label.h"
#include "win_write.h"
#include "win_image.h"
#include "win_container.h"
#include "win_select.h"

win_select * win_select::cur_select=NULL;
SDLKey win_select::next_key=WIN_SELECT_DEFAULT_KEY_NEXT;
SDLKey win_select::previous_key=WIN_SELECT_DEFAULT_KEY_PREVIOUS;
SDLKey win_select::next_select_key=WIN_SELECT_DEFAULT_KEY_NEXT_SELECT;
SDLKey win_select::up_select_key=WIN_SELECT_DEFAULT_KEY_UP_SELECT;
SDLKey win_select::activate_key=WIN_SELECT_DEFAULT_KEY_ACTIVATE;
bool win_select::activate_selection=false;

#ifdef _DEBUG_
u_int16 win_select::cpt_win_select_debug=0;
#endif



win_select::win_select(win_container * tmp,bool b=WIN_SELECT_JUST_OBJECT)
{
#ifdef _DEBUG_
   cout << "win_select() called, "<< ++cpt_win_select_debug
       << " objects currently allocated\n";
#endif

  attached_container=tmp;
  type_select=b;
  l_list.clear();ite_list=l_list.begin();
  l_select.clear();ite_select=l_select.begin();
  border_to_select=NULL;
  cursor_to_select=NULL;
  cur_select=this;
  father_select=NULL;  
}

win_select::win_select(win_container * tmp,win_select *tws,bool b=WIN_SELECT_JUST_OBJECT)
{
#ifdef _DEBUG_
  cout << "win_select() called, "<< ++cpt_win_select_debug
       << " objects currently allocated\n";
#endif
  attached_container=tmp;
  type_select=b;
  l_list.clear();ite_list=l_list.begin();
  l_select.clear();ite_select=l_select.begin();
  border_to_select=NULL;
  cursor_to_select=NULL;
  father_select=tws;  
}

win_select::~win_select()
{
#ifdef _DEBUG_
  cout << "~win_select() called, "<< --cpt_win_select_debug
       << " objects currently allocated\n";
#endif
  remove_all();
  remove_all_select();
}

void win_select::add(win_base * twb, u_int8 m)
{  
  twb->attach_select(this,m);
  l_list.push_back(twb);
  ite_list=l_list.begin();
  if(m==WIN_SELECT_MODE_BORDER && border_to_select)
    twb->set_border_select(border_to_select);
  else if(m==WIN_SELECT_MODE_CURSOR && cursor_to_select)
    {
      twb->set_cursor(cursor_to_select);
      ((win_label*)twb)->init_draw();
    } 
}

void win_select::remove_all()
{
  list<win_base *>::iterator i=l_list.begin();
  while(i!=l_list.end())
    {     
      if(*i)(*i)->dettach_select();
      i++;
    }
  l_list.clear();
}

void win_select::remove_all_select()
{
  list<win_container *>::iterator i=l_select.begin();
  while(i!=l_select.end())
    { 
      if(*i) 
	{
	  (*i)->dettach_select();
	  delete (*i)->tree_select;
	  (*i)->tree_select=NULL;
	}
      i++;
    }
  l_list.clear();
}

void win_select::remove(win_base * twb)
{
  list<win_base *>::iterator i=l_list.begin();
  while(i!=l_list.end() && twb!=(*i)) i++;
  if(i!=l_list.end()) l_list.erase(i);
  twb->dettach_select();
} 

void win_select::set_border(win_border * tmp)
{
  border_to_select=tmp;
} 

void win_select::set_cursor(win_cursor * tmp)
{
  cursor_to_select=tmp;
} 

win_base * win_select::next()
{
  if(type_select)
    {
      if(ite_select!=l_select.end())
	{
	  win_container * tmpwc=*ite_select;
	  if(tmpwc)
	    {
	      if(tmpwc->tree_select)
		{
		  if(tmpwc->tree_select->ite_list==tmpwc->tree_select->l_list.end()) return NULL;
		  (*(tmpwc->tree_select->ite_list))->unselect();
		  tmpwc->tree_select->ite_list++;
		  if(tmpwc->tree_select->ite_list==tmpwc->tree_select->l_list.end())
		    {
		      tmpwc->tree_select->ite_list--;
		    }
		  (*(tmpwc->tree_select->ite_list))->select();
		  tmpwc->tree_select->adjust_visible();
		  return(*(tmpwc->tree_select->ite_list));
		}
	    }
	}
    }
  else
    {

      if(ite_list!=l_list.end())
	{
	  (*ite_list)->unselect();
	  ite_list++;
	  if(ite_list==l_list.end())
	    {
	      ite_list--;
	    }
	  (*ite_list)->select();
	  adjust_visible();
	  return *ite_list;
	}
    }
  

  return NULL;
}

win_base * win_select::previous()
{
  if(type_select)
    {
      if(ite_select!=l_select.end())
	{
	  win_container * tmpwc=*ite_select;
	  if(tmpwc)
	    {
	      if(tmpwc->tree_select)
		{
		  if(tmpwc->tree_select->ite_list==tmpwc->tree_select->l_list.end()) return NULL;
		  (*(tmpwc->tree_select->ite_list))->unselect();
		  if(tmpwc->tree_select->ite_list!=tmpwc->tree_select->l_list.begin())
		    { 
		      tmpwc->tree_select->ite_list--;    
		    }
		  (*(tmpwc->tree_select->ite_list))->select();
		  tmpwc->tree_select->adjust_visible();
		  return(*(tmpwc->tree_select->ite_list));  
		}
	    }
	}
    }
  else
    {
      if(ite_list==l_list.end()) return NULL;
      (*ite_list)->unselect();
      if(ite_list!=l_list.begin())
	{ 
	  ite_list--;    
	}
      (*ite_list)->select();
      adjust_visible();
      return(*ite_list);
    } 
  return NULL;
}

s_int16 win_select::get_y_move_to_be_visible()
{
  if((*ite_list)->real_y == attached_container->real_y || ((*ite_list)->real_y>=attached_container->real_y && (*ite_list)->real_y+(*ite_list)->height <= attached_container->real_y + attached_container->height))
    return 0;
  else
    if((*ite_list)->real_y < attached_container->real_y) 
      return (attached_container->real_y - (*ite_list)->real_y);
    else
      if((*ite_list)->real_y > attached_container->real_y+attached_container->height || (*ite_list)->real_y+(*ite_list)->height > attached_container->real_y+attached_container->height) 
	return ((attached_container->real_y+attached_container->height) - ((*ite_list)->real_y+(*ite_list)->height));
  return 0;
}

void win_select::adjust_visible()
{
  if(ite_list!=l_list.end())
    {
      s_int16 y_move=get_y_move_to_be_visible();
      if(y_move)
	{
	  list<win_base*>::iterator i=l_list.begin();
	  while(i!=l_list.end())
	    {
	      (*i)->move((*i)->x,(*i)->y+y_move);
	      i++;
	    }  
	}
    }
}

win_select *  win_select::add_select(win_container * twc,bool b=WIN_SELECT_JUST_OBJECT)
{
  win_select * tmp2 = new win_select(twc,this,b);
  twc->tree_select=tmp2;
  
  twc->attach_select(this,WIN_SELECT_MODE_BRIGHTNESS);
  l_select.push_back(twc);
  ite_select=l_select.begin();  
  return tmp2; 
}

void win_select::remove_select(win_select * tmp)
{
  list<win_container *>::iterator i=l_select.begin();
  while(i!=l_select.end() && tmp!=(*i)->tree_select) i++;
  if(i!=l_select.end()) 
    {
      l_select.erase(i);    
    }
}



void win_select::next_select()
{
  if(ite_select!=l_select.end())
    {
      (*ite_select)->unselect();
      ite_select++;
      if(ite_select==l_select.end())
	{ 
	  ite_select=l_select.begin();
	}
      (*ite_select)->select();
    }
}


void win_select::up_select()
{
  if(father_select) cur_select=father_select;
}

void win_select::update()
{
  if(cur_select)
    if(activate_selection)
      {	
	if(input::has_been_pushed(next_key)) cur_select->next();
	else if(input::has_been_pushed(previous_key)) cur_select->previous();
	else if(input::has_been_pushed(next_select_key)) cur_select->next_select();
	else if(input::has_been_pushed(up_select_key)) cur_select->up_select();
	else if(input::has_been_pushed(activate_key)) cur_select->activate_select(); 
      } 
}

void win_select::activate_select()
{
  if(type_select)
    {
      if(ite_select!=l_select.end())
	{
	  win_container * tmpwc=*ite_select;
	  if(tmpwc)
	    if(tmpwc->tree_select)
	      if(tmpwc->tree_select->type_select) cur_select=tmpwc->tree_select;
	      else if(tmpwc->tree_select->ite_list!=tmpwc->tree_select->l_list.end())
		(*(tmpwc->tree_select->ite_list))->activate();  
	}
    }
  else
    {
      if(ite_list!=l_list.end()) 
	{
	  (*ite_list)->activate();
	}
    }  
}

void win_select::set_default_obj(void * tmp)
{
  if(type_select)
    {
      list<win_container *>::iterator i=l_select.begin();
      while(i!=l_select.end() && tmp!=(*i)) i++;
      if(i!=l_select.end()) ite_select=i;
      (*ite_select)->select();
    }
  else
    {
      list<win_base *>::iterator i=l_list.begin();
      if(ite_list!=l_list.end()) (*ite_list)->unselect();
      while(i!=l_list.end() && tmp!=(*i)) i++;
      if(i!=l_list.end()) ite_list=i;
      (*ite_list)->select();
    }
}

win_base * win_select::get()
{
  return(type_select?*ite_list:*ite_select);
}

u_int8 win_select::get_pos()
{
  u_int8 cpt=1;
  if(type_select)
    {
      list<win_container *>::iterator i=l_select.begin();
      while(i!=l_select.end() && ite_select!=i) {i++;cpt++;}
      if(i==l_select.end()) return 0;
    }
  else
    {
      list<win_base *>::iterator i=l_list.begin();
      while(i!=l_list.end() && ite_list!=i) {i++;cpt++;}
      if(i==l_list.end()) return 0;
    }
  return cpt;
}


